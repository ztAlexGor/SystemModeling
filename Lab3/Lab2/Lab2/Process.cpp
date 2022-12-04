#include "Process.h"

Process::Process()
{
	maxqueue = 0;
	currTask = nullptr;
	queue = std::vector<Task*>();
	selectTaskFunc = [](std::vector<Task*> q) {return 0; };
	changeQueueFunc = [](std::vector<Task*> q) {return -1; };
}

Process::Process(std::string nameOfElement, RandGenerator* delayGenerator, int maxQueue) :Element(nameOfElement, delayGenerator)
{
	maxqueue = maxQueue;
	currTask = nullptr;
	queue = std::vector<Task*>();
	selectTaskFunc = [](std::vector<Task*> q) {return 0; };
	changeQueueFunc = [](std::vector<Task*> q) {return -1; };
}

std::vector<Task*> Process::getQueue() {
	return queue;
}

int Process::getQueueSize()
{
	return queue.size();
}

void Process::setQueue(std::vector<Task*>& queue) {
	this->queue = queue;
}

void Process::setCurrTask(Task* t)
{
	currTask = t;
}

void Process::setSelectTaskFunction(int(*selectTask)(std::vector<Task*>)) {
	this->selectTaskFunc = selectTask;
}

void Process::setChangeQueueFunction(std::function<int(std::vector<Task*>)> changeQueueFunc)
{
	this->changeQueueFunc = changeQueueFunc;
}

int Process::getMaxqueue() {
	return maxqueue;
}
void Process::setMaxqueue(int maxqueue) {
	this->maxqueue = maxqueue;
}

void Process::init(Task* task, std::vector<Task*> queue)
{
	inAct(task);
	this->queue = queue;
}

Task* Process::deleteFromQueue(int index)
{
	if (index < 0 && index >= queue.size())return nullptr;

	Task* t = queue[index];
	queue.erase(queue.begin() + index);
	return t;
}

bool Process::isFull()
{
	return (state == IN_PROGRESS && queue.size() == maxqueue);
}

void Process::inAct(Task* task) {
	Element::inAct(task);

	if (state == IDLE) {
		state = IN_PROGRESS;
		currTask = task;
		tnext = tcurr + calculateDelay(currTask);
	}
	else {
		if (queue.size() < maxqueue) {
			queue.push_back(task);
		}
		else {
			statistics.eventFailure(tcurr - task->getCreationTime());
			delete task;
		}
	}
}

void Process::outAct() {
	if (state == IN_PROGRESS) {
		Element::outAct();
		activateNextElement(currTask);
		startNextTask();
	}
	else {
		throw "IDLE";
	}
}

void Process::startNextTask() {
	if (queue.empty()) {
		currTask = nullptr;
		state = IDLE;
		tnext = INFINITY;
	}
	else {
		int index = selectTaskFunc(queue);
		currTask = queue[index];
		queue.erase(queue.begin() + index);

		state = IN_PROGRESS;
		tnext = tcurr + calculateDelay(currTask);
	}
}

bool Process::checkChanges()
{
	int index = changeQueueFunc(queue);

	Task* t = deleteFromQueue(index);

	if (t) {
		prevElement->activateNextElement(t);
		statistics.eventQueueChanging();
		return true;
	}
	return false;
}





//void Create::printResult(double tcurr) {
//	Element::printResult(tcurr);
//	statistics.printName();
//	statistics.print(Info::KEYS::QUANTITY | Info::KEYS::WORKLOAD | Info::KEYS::TIME_OUT);
//}




void Process::printInfo() {
	Element::printInfo();
	statistics.printParametr("queue_sz = " + std::to_string(queue.size()));
	statistics.printParametr("failures = " + std::to_string(int(statistics.failure)));
	std::cout << std::endl;
}

void Process::printResult(double tcurr) {
	Element::printResult(tcurr);

	statistics.printName();

	statistics.print(Info::KEYS::QUANTITY | Info::KEYS::WORKLOAD | Info::KEYS::TIME_IN
		| Info::KEYS::TIME_OUT | Info::KEYS::QUEUE_SZ | Info::KEYS::FAILURES | Info::KEYS::QUEUE_CH, 28);

	//std::cout << "quantity = " << quantity
	//	<< "\t\t  mean workload = " << meanWorkload
	//	<< "\t\tincome time interval = " << meanTimeIn
	//	<< "\t\toutcome time interval = " << meanTimeOut
	//	<< "\nmean queueSize = " << meanQueueSize
	//	<< "\t  failure chance = " << failure << "% "
	//	<< "\tqueue changing = " << queueChanging;
}

void Process::doStatistics(double delta) {
	Element::doStatistics(delta);
	statistics.meanQueueSize += queue.size() * delta;
	//info.TimeIn;
	//info.TimeOut;
	//info.meanTaskLifetime;
	//info.failure
	//info.queueChanging
}
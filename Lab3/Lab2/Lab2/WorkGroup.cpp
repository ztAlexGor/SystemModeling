#include "WorkGroup.h"


WorkGroup::WorkGroup(std::vector<Process*> processes, int maxQueue) : maxQueue(maxQueue)
{
	for (auto e : processes) {
		this->processes.push_back(e);
	}
	selectTaskFunc = [](std::vector<Task*> q) {return 0; };
	changeQueueFunc = [](std::vector<Process*> processes, std::vector<Task*> queue) {return -1; };
}

void WorkGroup::setQueue(std::vector<Task*> queue) {
	queue.clear();

	for (auto e : queue)
	{
		this->queue.push_back(e);
	}
}

std::vector<Task*> WorkGroup::getQueue()
{
	return queue;
}

void WorkGroup::setMaxqueue(int maxQueue) {
	this->maxQueue = maxQueue;
}

int WorkGroup::getMaxqueue() {
	return maxQueue;
}

int WorkGroup::getQueueSize() {
	return queue.size();
}

int WorkGroup::getProcessCount()
{
	return processes.size();
}

const Info& WorkGroup::getFinalInfo()
{
	Info totalStats;
	for (Process* p : processes) {
		totalStats.accumulateInfo(p->getInfo());
	}
	totalStats.meanQueueSize *= processes.size();
	totalStats.stopModeling(processes.size());
	/*totalStats.meanWorkload /= processCount;
	totalStats.meanQueueSize /= processCount;
	totalStats.meanTimeIntervalBetweenTasks /= totalStats.quantity;
	totalStats.failure /= (totalStats.quantity + totalStats.failure)/100.0;
	totalStats.meanTaskLifetime /= totalStats.quantity;*/
	return totalStats;
}

void WorkGroup::setTcurr(double t)
{
	Element::setTcurr(t);
	for (auto p : processes) {
		p->setTcurr(t);
	}
}

void WorkGroup::addNextElement(Element* nextElement)
{
	for (auto p : processes) {
		p->addNextElement(nextElement);
	}
	Element::addNextElement(nextElement);
}

void WorkGroup::setSelectTaskFunction(int(*selectTask)(std::vector<Task*>))
{
	this->selectTaskFunc = selectTask;
}

void WorkGroup::setChangeQueueFunction(int (*changeQueueFunc)(std::vector<Process*> processes, std::vector<Task*> queue))
{
	this->changeQueueFunc = changeQueueFunc;
}

void WorkGroup::doStatistics(double delta) {
	Element::doStatistics(delta);
	statistics.meanQueueSize += queue.size() * delta;
}

bool WorkGroup::checkChanges()
{
	bool isQueueChanged = false;

	for (auto p : processes) {
		int index = changeQueueFunc(processes, p->getQueue());

		Task* t = p->deleteFromQueue(index);
		if (t) {
			inAct(t);
			statistics.eventQueueChanging(p->getInfo().name);
			isQueueChanged = true;
		}
	}
	return isQueueChanged;
}

void WorkGroup::inAct(Task* task) {
	Element::inAct(task);
	state = IN_PROGRESS;

	Process* bestCandidate = nullptr;

	for (auto p : processes) {
		if (p->getState() == IDLE) {
			bestCandidate = p;
			break;
		}
		else if (!p->isFull() && (!bestCandidate || p->getQueueSize() < bestCandidate->getQueueSize())) {
			bestCandidate = p;
		}
	}

	if (bestCandidate){
		bestCandidate->inAct(task);
		if (bestCandidate->getTnext() < tnext)tnext = bestCandidate->getTnext();
		task = nullptr;
	}
	else {
		if (queue.size() < maxQueue) {
			queue.push_back(task);
		}
		else {
			statistics.eventFailure(tcurr - task->getCreationTime());
			delete task;
		}
	}
}

void WorkGroup::outAct() {
	if (state == IN_PROGRESS) {
		for (auto p : processes) {
			if (p->getTnext() == tcurr) {
				Element::outAct();
				
				p->outAct();

				if (!queue.empty()) {
					int index = selectTaskFunc(queue);
					p->inAct(queue[index]);
					queue.erase(queue.begin() + index);
				}
			}
		}

		state = IDLE;
		tnext = INFINITY;

		for (auto p : processes) {
			if (p->getState() == IN_PROGRESS) {
				state = IN_PROGRESS;
				if (tnext > p->getTnext())tnext = p->getTnext();
			}
		}
	}
	else {
		throw "IDLE";
	}
}

void WorkGroup::printInfo()
{
	Info::printCentral("  WorkGroup " + statistics.name + "  ", '#');
	for (auto p : processes) {
		p->printInfo();
	}
	Info::printCentral("", '-');
	Element::printInfo();
	Info::printParametr("queue_sz = " + std::to_string(queue.size()));
	Info::printParametr("failures = " + std::to_string(int(statistics.failure)));
	std::cout << std::endl;
	Info::printCentral("", '#');
	std::cout << std::endl;

	//std::cout << statistics.name
	//	<< " \tstate = " << (state == IDLE ? "IDLE" : "BUSY")
	//	<< "\tquantity = " << statistics.quantity
	//	<< "\ttnext = " << std::fixed << std::setprecision(1) << tnext
	//	<< "\tfailure = " << int(statistics.failure)
	//	<< "\tqueue size = " << queue.size() << std::endl
	//	<< "##########################################################################################" << std::endl << std::endl << std::endl;
}

void WorkGroup::printResult(double tcurr) {
	for (auto p : processes) {
		const Info& i = p->getInfo();
		statistics.tasksEndNum += i.tasksEndNum;
		statistics.meanTaskLifetime += i.meanTaskLifetime;
	}
	Element::printResult(tcurr);

	Info::printCentral("  WorkGroup " + statistics.name + "  ", '#');
	for (auto p : processes) {
		p->printResult(tcurr);
		std::cout << std::endl;
	}
	Info::printCentral("TOTAL", '-');

	statistics.print(Info::KEYS::QUANTITY | Info::KEYS::WORKLOAD | Info::KEYS::TIME_IN
		| Info::KEYS::TIME_OUT | Info::KEYS::QUEUE_SZ | Info::KEYS::FAILURES | Info::KEYS::QUEUE_CH, 28);
	std::cout << std::endl;
	Info::printCentral("", '#');
	std::cout << std::endl;
}






//WorkGroup::WorkGroup(std::vector<Process*> processes, int maxQueue, std::function<bool(Task* left, Task* right)>comparator): maxQueue(maxQueue),
//	queue(comparator)
//{
//	for (auto e : processes) {
//		this->processes.push_back(e);
//	}
//}

//std::priority_queue<Task*, std::vector<Task*>, std::function<bool(Task* left, Task* right)>> WorkGroup::getQueue() {
//	return queue;
//}

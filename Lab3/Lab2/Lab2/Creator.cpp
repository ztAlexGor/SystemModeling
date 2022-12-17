#include "Creator.h"


Create::Create(std::string nameOfElement, RandGenerator* delayGenerator, Task* task)
	:Element(nameOfElement, delayGenerator)
{
	state = IN_PROGRESS;
	tnext = 0.0;
	distribution = { std::pair<Task*, double>{(task ? new Task(*task, tnext) : new Task(tnext)), 1} };
}
Create::Create(std::string nameOfElement, RandGenerator* delayGenerator, std::vector<std::pair<Task*, double>> distribution)
	:Element(nameOfElement, delayGenerator)
{
	state = IN_PROGRESS;
	tnext = 0.0;
	this->distribution = distribution;
}

Create::~Create()
{
	for (auto pair : distribution) {
		delete pair.first;
	}
}

void Create::outAct() {
	Element::outAct();
	activateNextElement(createNewTask());
	tnext = tcurr + calculateDelay(nullptr);
}

Task* Create::createNewTask()
{
	double rand = (double)std::rand() / RAND_MAX;

	for (auto i : distribution) {
		if (rand < i.second)return new Task(*i.first, tnext);
		else rand -= i.second;
	}
	return new Task(*distribution[0].first, tnext);
}

void Create::printInfo()
{
	Element::printInfo();
	std::cout << std::endl;
}


void Create::printResult(double tcurr)
{
	Element::printResult(tcurr);
	statistics.printName();
	statistics.print(Info::KEYS::QUANTITY | Info::KEYS::WORKLOAD | Info::KEYS::TIME_OUT, 28);
}

//void Create::printResult(double tcurr) {
//	Element::printResult(tcurr);
//	statistics.printResultForCreator();
//}

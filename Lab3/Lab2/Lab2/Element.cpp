#include "Element.h"

int Element::nextId = 0;

Element::~Element()
{
	if (delayGenerator)delete delayGenerator;
	//for (auto e : nextElements)delete e;
	//if (prevElement)delete prevElement;
}

Element::Element() :statistics("element" + std::to_string(nextId))
{
	delayGenerator = new Uniform(10, 10);
	nextElements = std::vector<Element*>();
	transitionCondition = [](std::vector<Element*> list, Task* task) {return list.empty() ? nullptr : list[0]; };
	prevElement = nullptr;
	state = IDLE;
	id = nextId++;
	tcurr = 0.0;
	tnext = INFINITY;
}

Element::Element(std::string nameOfElement, RandGenerator* delayGenerator) :statistics(nameOfElement)
{
	this->delayGenerator = delayGenerator;
	nextElements = std::vector<Element*>();
	transitionCondition = [](std::vector<Element*> list, Task* task) {return list.empty() ? nullptr : list[0]; };
	prevElement = nullptr;
	state = IDLE;
	id = nextId++;
	tcurr = 0.0;
	tnext = INFINITY;
}

RandGenerator* Element::getDelayGenerator()
{
	return delayGenerator;
}

void Element::setDelayGenerator(RandGenerator* delayGenerator)
{
	this->delayGenerator = delayGenerator;
}

const Info& Element::getInfo() {
	return statistics;
}

double Element::getTcurr()
{
	return tcurr;
}

void Element::setTcurr(double tcurr)
{
	doStatistics(tcurr - this->tcurr);
	this->tcurr = tcurr;
}

Element::States Element::getState()
{
	return state;
}

std::vector<Element*> Element::getNextElements()
{
	return nextElements;
}

void Element::addNextElement(Element* nextElement)
{
	this->nextElements.push_back(nextElement);
	nextElement->setPreviousElement(this);
}

void Element::setNextElements(std::vector<Element*> nextElements)
{
	for (auto i : nextElements)addNextElement(i);
}

void Element::setTransitionCondition(Element* f(std::vector<Element*>, Task*))
{
	transitionCondition = f;
}

void Element::setPreviousElement(Element* el)
{
	if (!prevElement)prevElement = el;
}

void Element::setTnext(double t)
{
	tnext = t;
}

double Element::getTnext()
{
	return tnext;
}

int Element::getId()
{
	return id;
}

std::string Element::getName()
{
	return statistics.name;
}

void Element::setName(std::string name)
{
	statistics.name = name;
}

void Element::printResult(double tcurr)
{
	statistics.stopModeling(tcurr);
}

void Element::printInfo()
{
	std::ostringstream oss;
	oss << "tnext = " << std::fixed << std::setprecision(1) << tnext;

	statistics.printParametr(statistics.name, ' ', std::max((int)(statistics.name.size() + 4), 14));
	statistics.printParametr(std::string("state = ") + (state == IDLE ? "IDLE" : "BUSY"));
	statistics.printParametr(oss.str());
	statistics.print(Info::KEYS::QUANTITY);
}

void Element::doStatistics(double delta)
{
	statistics.meanWorkload += (state == IN_PROGRESS ? 1 : 0) * delta;
}

void Element::activateNextElement(Task* task) {
	if (nextElements.size() && transitionCondition) {
		Element* next = transitionCondition(nextElements, task);
		if (next) {
			next->inAct(task);
			return;
		}
	}
	statistics.eventEndTask(tcurr - task->getCreationTime());
	delete task;
}

void Element::outAct()
{
	statistics.eventOut(tcurr);
}

void Element::inAct(Task* task)
{
	statistics.eventIn(tcurr);
}

bool Element::checkChanges()
{
	return false;
}

double Element::calculateDelay(Task* t){
	CustomGenerator* customGen = dynamic_cast<CustomGenerator*>(delayGenerator);

	if (customGen)return customGen->generate(t);
	return delayGenerator->generate();
}

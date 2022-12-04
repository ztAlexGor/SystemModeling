#include "Transition.h"

Transition::Transition(std::string nameOfElement, RandGenerator* delayGenerator):
	Element(nameOfElement, delayGenerator), schedule([](std::pair<Task*, double>left, std::pair<Task*, double>right)->bool {return left.second > right.second; }) {}

void Transition::inAct(Task* t)
{
	Element::inAct(t);
	state = IN_PROGRESS;
	schedule.push(std::pair<Task*, double>(t, tcurr + calculateDelay(t)));
	tnext = schedule.top().second;
}


void Transition::outAct()
{
	if (state == IN_PROGRESS) {
		Element::outAct();
		activateNextElement(schedule.top().first);
		schedule.pop();

		if (schedule.empty()) {
			state = IDLE;
			tnext = INFINITY;
		}
		else {
			tnext = schedule.top().second;
		}
	}
	else {
		throw "IDLE";
	}
}


void Transition::printInfo()
{
	Info::printCentral("  Transition " + statistics.name + "  ", '#');

	std::vector<std::pair<Task*, double>>* list = getListOfTasks();
	for (int i = 0; i < list->size(); i++) {
		statistics.printParametr(std::to_string(i + 1), ' ', 4);
		std::cout << "Task " << (*list)[i].first->getType() << " \tends transiton at: " << (*list)[i].second << std::endl;
	}
	
	Info::printCentral("", '-');
	Element::printInfo();
	Info::printParametr("transitions = " + std::to_string(list->size()));
	std::cout << std::endl;
	Info::printCentral("", '#');
	std::cout << std::endl;
	delete list;
}

void Transition::printResult(double tcurr) {
	Element::printResult(tcurr);

	statistics.printName();

	statistics.print(Info::KEYS::QUANTITY | Info::KEYS::WORKLOAD | Info::KEYS::TIME_IN | Info::KEYS::TIME_OUT, 28);
}




std::vector<std::pair<Task*, double>>* Transition::getListOfTasks()
{
	std::vector<std::pair<Task*, double>>* list = new std::vector<std::pair<Task*, double>>();
	while (!schedule.empty()) {
		list->push_back(schedule.top());
		schedule.pop();
	}
	for (auto p : *list) {
		schedule.push(p);
	}
	return list;
}

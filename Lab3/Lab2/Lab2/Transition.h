#pragma once
#include <queue>
#include <functional>
#include "Element.h"

class Transition : public Element
{
	std::priority_queue <std::pair<Task*, double>, std::vector<std::pair<Task*, double>>,
		std::function<bool(std::pair<Task*, double>left, std::pair<Task*, double>right)>> schedule;
public:
	Transition(std::string nameOfElement, RandGenerator* delayGenerator);

	void inAct(Task*) override;
	void outAct() override;

	void printInfo() override;
	void printResult(double tcurr) override;

private:
	std::vector<std::pair<Task*, double>>* getListOfTasks();
};


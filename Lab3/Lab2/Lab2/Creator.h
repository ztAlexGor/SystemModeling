#pragma once
#include "Element.h"

class Create : public Element {
private:
	std::vector<std::pair<Task*, double>> distribution;
public:
	Create(std::string nameOfElement, RandGenerator* delayGenerator, Task* task = nullptr);
	Create(std::string nameOfElement, RandGenerator* delayGenerator, std::vector<std::pair<Task*, double>> distribution);
	void outAct() override;
	void printInfo() override;
	void printResult(double tcurr) override;
private:
	Task* createNewTask();
};
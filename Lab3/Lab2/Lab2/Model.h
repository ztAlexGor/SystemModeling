#pragma once
#include <vector>
#include <iostream>
#include "Process.h"
#include "Creator.h"
#include "WorkGroup.h"
#include "Transition.h"

class Model {
private:
	std::vector<Element*> list;
	double tnext, tcurr;
	Element* event;
public:
	Model(const std::vector<Element*>& elements);
	void simulate(double time);
private:
	Info CollectInfo();
	void printInfo();
	void printResult();
};



#pragma once
#include <vector>
#include <functional>
#include <iomanip>
#include "RandomGenerator.h"
#include "Task.h"
#include "Info.h"
//using namespace std;


class Element {
public:
	enum States { IDLE, IN_PROGRESS };
private:
	static int nextId;
protected:
	std::vector<Element*> nextElements;
	Element* (*transitionCondition) (std::vector<Element*>, Task*);
	Element* prevElement;
	RandGenerator* delayGenerator;
	States state;
	int id;
	double tcurr, tnext;
	Info statistics;
public:
	virtual ~Element();
	Element();
	Element(std::string nameOfElement, RandGenerator* delayGenerator);
	RandGenerator* getDelayGenerator();
	void setDelayGenerator(RandGenerator* delayGenerator);
	const Info& getInfo();
	double getTcurr();
	virtual double getTnext();
	virtual void setTcurr(double tcurr);
	virtual void setTnext(double t);
	Element::States getState();
	std::vector<Element*> getNextElements();
	virtual void addNextElement(Element* nextElement);
	void setNextElements(std::vector<Element*> nextElements);
	void setTransitionCondition(Element* f(std::vector<Element*>, Task*));
	void setPreviousElement(Element* el);
	int getId();
	std::string getName();
	void setName(std::string name);

	virtual void outAct();
	virtual void inAct(Task* task);
	virtual bool checkChanges();
	virtual void activateNextElement(Task* task);
	virtual void printResult(double tcurr);
	virtual void printInfo();
	
protected:
	double calculateDelay(Task* t);
	virtual void doStatistics(double delta);
};

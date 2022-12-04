#pragma once
#include "Element.h"

class Process : public Element {
private:
	Task* currTask;
	int maxqueue;
	std::vector<Task*> queue;
	int (*selectTaskFunc)(std::vector<Task*>);
	std::function<int(std::vector<Task*>)> changeQueueFunc;
public:
	Process();
	Process(std::string nameOfElement, RandGenerator* delayGenerator, int maxQueue = INT_MAX);

	std::vector<Task*> getQueue();
	int getQueueSize();
	void setQueue(std::vector<Task*>& queue);
	void setCurrTask(Task* t);
	void setSelectTaskFunction(int (*selectTask)(std::vector<Task*>));
	void setChangeQueueFunction(std::function<int(std::vector<Task*>)> changeQueueFunc);
	int getMaxqueue();
	void setMaxqueue(int maxqueue);
	void init(Task* task, std::vector<Task*> queue);
	Task* deleteFromQueue(int index);
	bool isFull();

	void printInfo() override;
	void printResult(double tcurr) override;

	void inAct(Task* task) override;
	void outAct() override;
	bool checkChanges() override;
private:
	void startNextTask();
	void doStatistics(double delta) override;
};


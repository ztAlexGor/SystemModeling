#pragma once
#include <queue>
#include <functional>
#include "Element.h"
#include "Process.h"


class WorkGroup : public Element {
private:
	std::vector<Process*> processes;
	std::vector<Task*> queue;
	int maxQueue;
	int (*selectTaskFunc)(std::vector<Task*>);
	int (*changeQueueFunc)(std::vector<Process*> processes, std::vector<Task*> queue);

public:
	WorkGroup(std::vector<Process*> processes, int maxQueue = INT_MAX);
	void setQueue(std::vector<Task*> queue);
	std::vector<Task*> getQueue();
	void setMaxqueue(int maxqueue);
	int getMaxqueue();
	int getQueueSize();
	int getProcessCount();
	const Info& getFinalInfo();

	void setTcurr(double t) override;
	void addNextElement(Element* nextElement) override;
	bool checkChanges() override;
	void setChangeQueueFunction(int (*changeQueueFunc)(std::vector<Process*> processes, std::vector<Task*> queue));
	void setSelectTaskFunction(int (*selectTask)(std::vector<Task*>));

	void inAct(Task* task) override;
	void outAct() override;
	void printInfo() override;
	void printResult(double tcurr) override;

private:
	void doStatistics(double delta) override;

	//std::priority_queue<Task*, std::vector<Task*>, std::function<bool(Task* left, Task* right)>> queue;
	//Task* currTask;


	//WorkGroup(std::vector<Process*> processes, int maxQueue = INT_MAX, std::function<bool(Task* left, Task* right)>comparator = [](Task* left, Task* right)->bool {return false; });
	//WorkGroup(std::string nameOfElement, RandGenerator* delayGenerator, int maxQueue = INT_MAX);
	//std::priority_queue<Task*, std::vector<Task*>, std::function<bool(Task* left, Task* right)>> getQueue();
	//void setCurrTask(Task* t);
	//void startNextTask();
};


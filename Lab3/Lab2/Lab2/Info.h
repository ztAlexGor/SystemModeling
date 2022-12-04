#pragma once
#include <string>
#include <iostream>
#include <sstream>

struct Info {
	enum KEYS {
		QUANTITY = 1<<0,
		WORKLOAD = 1<<1,
		QUEUE_SZ = 1<<2,
		FAILURES = 1<<3,
		LIFETIME = 1<<4,
		TIME_IN  = 1<<5,
		TIME_OUT = 1<<6,
		QUEUE_CH = 1<<7
	};

	std::string name;
	int quantity = 0;				//specific + total
	double meanWorkload = 0;		//specific + total
	double meanQueueSize = 0;		//specific + total
	double failure = 0;				//specific + total
	double meanTaskLifetime = 0;	//total
	double meanTimeIn = 0;			//specific + total
	double meanTimeOut = 0;			//specific + total
	int queueChanging = 0;			//specific + total
	int tasksEndNum = 0;
private:
	double lastOutTime = 0;
	double lastInTime = 0;
public:
	Info() = default;

	Info(std::string name) :name(name) {}

	void stopModeling(double finishTime);
	
	void accumulateInfo(const Info& info);

	void eventOut(double time);
	void eventIn(double time);
	void eventEndTask(double time);
	void eventFailure(double time);
	void eventQueueChanging(std::string name = "");




	//void printTotalResults();
	static void printParametr(std::string text, char symbol = ' ', int width = 18);
	static void printCentral(std::string text, char symbol = ' ', int width = 100);
	void printName(char symbol = '-', int width = 100);
	
	std::string getParametrString(int param);

	//void printResultForProcess();
	//
	//void printResultForCreator();

	void printExtended(int param);
	void print(int param, int width = 18);
	//void PrintAll() {
	//	std::cout << "mean workload = " << meanTaskCount
	//		<< "\nmean time interval between tasks = " << meanTimeIntervalBetweenTasks
	//		<< "\nmean taskLifetime = " << meanTaskLifetime
	//		<< "\nmean queueSize = " << meanQueueSize
	//		<< "\nfailure probability = " << failure
	//		<< "\nqueue changing = " << queueChanging;
	//}
	//
	//void printInfo() {
	//	std::cout << name
	//		<< " \tstate = " << (state == IDLE ? "IDLE" : "BUSY")
	//		<< "\tquantity = " << statistics.quantity
	//		<< "\ttnext = " << std::fixed << std::setprecision(1) << tnext;
	//}
	//
};
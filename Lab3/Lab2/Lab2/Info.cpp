#include "Info.h"

void Info::stopModeling(double finishTime)
{
	meanWorkload /= finishTime;
	meanQueueSize /= finishTime;

	if (quantity != 0) {
		meanTimeIn /= (double)quantity;
		meanTimeOut /= (double)quantity;
		failure = failure * 100 / (double)(quantity + failure);
	}
	else {
		meanTimeIn = 0;
		meanTimeOut = 0;
		meanTaskLifetime = 0;
		failure = 0;
	}

	if (tasksEndNum != 0) {
		meanTaskLifetime /= (double)tasksEndNum;
	}
	else {
		meanTaskLifetime = 0;
	}
}

void Info::accumulateInfo(const Info& info)
{
	quantity += info.quantity;
	tasksEndNum += info.tasksEndNum;
	meanWorkload += info.meanWorkload;
	meanTimeIn += info.quantity * info.meanTimeIn;
	meanTimeOut += info.quantity * info.meanTimeOut;
	meanQueueSize += info.meanQueueSize;
	meanTaskLifetime += info.tasksEndNum * info.meanTaskLifetime;
	failure += info.quantity * info.failure / (100 - info.failure);
	queueChanging += info.queueChanging;
}

void Info::eventOut(double time)
{
	quantity++;
	if (lastOutTime != 0) {
		meanTimeOut += time - lastOutTime;
	}
	lastOutTime = time;
}

void Info::eventIn(double time)
{
	if (lastInTime != 0) {
		meanTimeIn += time - lastInTime;
	}
	lastInTime = time;
}

void Info::eventEndTask(double time)
{
	if (time != 0) {
		tasksEndNum++;
		meanTaskLifetime += time;
	}
}

void Info::eventFailure(double time)
{
	failure++;
	eventEndTask(time);
}

void Info::eventQueueChanging(std::string name)
{
	queueChanging++;

	if (name == "")name = this->name;
	std::cout << std::endl << "TASK LEAVES QUEUE " << name << std::endl;
}




//void Info::printTotalResults()
//{
//	std::cout << "\nquantity = " << quantity
//		<< "\t\t  mean workload = " << meanWorkload
//		<< "\t\tmean time interval between tasks = " << meanTimeOut
//
//		<< "\nmean tasks = " << meanQueueSize
//		<< "\t  failure chance = " << failure << "% "
//		<< "\tqueue changing = " << queueChanging
//
//
//		<< "\tmean taskLifetime = " << meanTaskLifetime << std::endl << std::endl;
//}


void Info::printParametr(std::string text, char symbol, int width)
{
	std::cout << text;
	for (int i = text.size(); i < width; i++)std::cout << symbol;
}

void Info::printCentral(std::string text, char symbol, int width)
{
	int num = width - text.size();
	for (int i = 0; i < num / 2; i++)std::cout << symbol;
	std::cout << text;
	for (int i = 0; i < num - num / 2; i++)std::cout << symbol;
	std::cout << std::endl;
}

void Info::printName(char symbol, int width)
{
	printCentral(name, symbol, width);
}

std::string Info::getParametrString(int param)
{
	std::ostringstream oss;
	oss.setf(std::ios::fixed);
	oss.precision(1);

		 if (param & QUANTITY) oss << "quantity = " << quantity;
	else if (param & WORKLOAD) oss << "workload = " << meanWorkload * 100 << "%";
	else if (param & QUEUE_SZ) oss << "queue_sz = " << meanQueueSize;
	else if (param & FAILURES) oss << "failures = " << failure << "%";
	else if (param & LIFETIME) oss << "lifetime = " << meanTaskLifetime;
	else if (param & TIME_IN ) oss << "time_in  = " << meanTimeIn;
	else if (param & TIME_OUT) oss << "time_out = " << meanTimeOut;
	else if (param & QUEUE_CH) oss << "queue_ch = " << queueChanging;
	else throw "param";

	return oss.str();
}


//void Info::printResultForProcess()
//{
//	printName();
//	std::cout << "quantity = " << quantity
//		<< "\t\t  mean workload = " << meanWorkload
//		<< "\t\tincome time interval = " << meanTimeIn
//		<< "\t\toutcome time interval = " << meanTimeOut
//		<< "\nmean queueSize = " << meanQueueSize
//		<< "\t  failure chance = " << failure << "% "
//		<< "\tqueue changing = " << queueChanging;
//}
//
//void Info::printResultForCreator()
//{
//	printName();
//
//	std::cout << "quantity = " << quantity
//		<< "\t\t  mean workload = " << meanWorkload
//		<< "\t\tmean time interval between tasks = " << meanTimeOut;
//}

void Info::printExtended(int param)//
{
	if (param & QUANTITY)std::cout << "\nquantity = " << quantity;
	if (param & WORKLOAD)std::cout << "\nmean workload = " << meanWorkload * 100 << "%";
	if (param & QUEUE_SZ)std::cout << "\nmean queue size = " << meanQueueSize;
	if (param & FAILURES)std::cout << "\nfailure chance = " << failure << "%";
	if (param & LIFETIME)std::cout << "\nmean task lifetime = " << meanTaskLifetime;
	if (param & TIME_IN )std::cout << "\nincome time interval = " << meanTimeIn;
	if (param & TIME_OUT)std::cout << "\noutcome time interval = " << meanTimeOut;
	if (param & QUEUE_CH)std::cout << "\nqueue changings = " << queueChanging;
	std::cout << std::endl;
}


void Info::print(int param, int width)//
{
	int count = 0;
	
	for (int key = 1; key <= 1<<7; key <<= 1) {
		if (key & param) {
			if (++count == 5)std::cout << std::endl;
			printParametr(getParametrString(key), ' ', width);
		}
	}
	//std::cout << std::endl;


	//if (param & QUANTITY)std::cout << "quantity: " << quantity			<< "\t";
	//if (param & WORKLOAD)std::cout << "workload: " << meanWorkload		<< "\t";
	//if (param & QUEUE_SZ)std::cout << "queue_sz: " << meanQueueSize	<< "\t";
	//if (param & FAILURES)std::cout << "failures: " << failure << "%"	<< "\t";
	//if (param & LIFETIME)std::cout << "lifetime: " << meanTaskLifetime << "\t";
	//if (param & TIME_IN )std::cout << "time_in : " << meanTimeIn		<< "\t";
	//if (param & TIME_OUT)std::cout << "time_out: " << meanTimeOut		<< "\t";
	//if (param & QUEUE_CH)std::cout << "queue_ch: " << queueChanging	<< "\t";
}




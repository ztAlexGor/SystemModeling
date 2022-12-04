#pragma once
#include <string>

class Task
{
	std::string type;
	double creationTime;
public:
	Task(double creationTime = 0.0) : type(""), creationTime(creationTime) {};
	Task(std::string type, double creationTime = 0.0) :type(type), creationTime(creationTime) {};
	Task(const Task&, double creationTime = 0.0);
	std::string getType();
	void setType(std::string type);
	double getCreationTime();

};



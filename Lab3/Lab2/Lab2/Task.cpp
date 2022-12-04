#include "Task.h"

Task::Task(const Task& other, double creationTime)
{
    this->creationTime = creationTime;
    this->type = other.type;
}

std::string Task::getType()
{
    return type;
}

void Task::setType(std::string type)
{
    this->type = type;
}

double Task::getCreationTime()
{
    return creationTime;
}
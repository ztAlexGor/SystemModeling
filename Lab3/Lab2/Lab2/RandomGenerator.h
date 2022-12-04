#pragma once
#include<cmath>
#include <random>
#include "Task.h"


class RandGenerator {
protected:
	double randomNum();
	int factorial(int n);
public:
	virtual double generate() = 0;
};


class Uniform : public RandGenerator {
private:
	double min, max;
public:
	Uniform(double min, double max) : min(min), max(max) {};
	double generate() override;
};


class Gaussian : public RandGenerator {
private:
	double mean, deviation;
public:
	Gaussian(double mean, double deviation) : mean(mean), deviation(deviation) {};
	double generate() override;
};


class Exponential : public RandGenerator {
private:
	double mean;
public:
	Exponential(double mean) : mean(1/mean) {};
	double generate() override;
};

class Erlang : public RandGenerator {
private:
	std::gamma_distribution<>distribution;
	std::mt19937 gen{ std::random_device()() };
public:
	Erlang(double m, int k);
	double generate() override;
};


class CustomGenerator : public RandGenerator {
private:
	double (*distribution)(Task*);
public:
	CustomGenerator(double (*distribution)(Task*)):distribution(distribution) {};
	double generate() override;
	double generate(Task* task);
};	
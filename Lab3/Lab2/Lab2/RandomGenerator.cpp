#include "RandomGenerator.h"


double RandGenerator::randomNum()
{
	return double(rand() % (RAND_MAX - 2) + 1) / RAND_MAX;
}

int RandGenerator::factorial(int n)
{
	if (n == 0)return 1;
	return n * factorial(n - 1);
}

double Exponential::generate()
{
	return -log(randomNum()) / mean;
}

double Uniform::generate()
{
	return min + randomNum() * (max - min);
}

double Gaussian::generate()
{
	double mu = -6;
	for (int i = 0; i < 12; i++)
		mu += double(rand()) / RAND_MAX;
	return deviation * mu + mean;
}

Erlang::Erlang(double m, int k) : distribution(k, m / k) {}

double Erlang::generate()
{
	return distribution(gen);
}

double CustomGenerator::generate()
{
	throw "Custom generator should`t be invoked like this";
	return 0;
}

double CustomGenerator::generate(Task* task)
{
	return distribution(task);
}

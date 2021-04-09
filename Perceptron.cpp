#include "Perceptron.hpp"
#define _SQR(x) x*x
Perceptron::Perceptron(int n)
{
	weight.resize(n);
	for (int a = 0; a < n; a++)
	{
		weight[a] = (double(rand() % 11) - 0.5f);
	}
	result = 0.f;
	summ = 0.f;
	b = double(rand() % 11) / 10.f - 0.5f;
}
double Perceptron::activationFunction(double summ1)
{
	//return (exp(summ) - exp(-summ)) / (exp(summ) + exp(-summ));
	return 1. / (1. + exp(-summ1));
	//return summ1;
}
double Perceptron::dactivationFunction(double summ1)
{
	//return 1 - _SQR(activationFunction(summ));
	return activationFunction(summ1) * (1 - activationFunction(summ1));
	//return 1.f;
}
double Perceptron::process(vector <double> x)
{
	summ = b;
	for (int i = 0; i < x.size(); i++)
		summ += double(x[i]) * weight[i];
	sm = x;
	result = activationFunction(summ);
	return result;
}
void Perceptron::learning(double input)
{
	double error = input - result;
	double tetta = error * dactivationFunction(summ) * 0.3f;
	for (int a = 0; a < weight.size(); a++)
		weight[a] += tetta * double(sm[a]);
	b += tetta;
}
double Perceptron::getWeight(int n)
{
	return weight[n];
}
double Perceptron::getResult()
{
	return result;
}
#ifndef Perc_HPP
#define Perc_HPP
#include <vector>
using namespace std;
class Perceptron
{
private:
	double result;
	double b;
	double summ;
	vector<double> sm;
public:
	vector<double> weight;
	Perceptron(int n);
	~Perceptron(){}
	double activationFunction(double summ1);
	double dactivationFunction(double summ1);
	double process(vector <double> x);
	void learning(double input);
	double getWeight(int n);
	double getResult();
};
#endif
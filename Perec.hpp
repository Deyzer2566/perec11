#include <vector>
class Perceptron
{
private:
	std::vector<double>weight;
	std::vector<double> x;
	double b;
public:
	double summ;
	double result;

	double activationFunction(double summ)
	{
		return 1. / (1. + exp(-summ));
	}
	double dactivationFunction(double summ)
	{
		return activationFunction(summ) * (1. - activationFunction(summ));
	}

	Perceptron(int n)
	{
		result = 0;
		summ = 0;
		for (int a = 0; a < n; a++)
			weight.push_back(rand() % 1000 / 1000. - 0.5);
		b = rand() % 1000 / 1000. - 0.5;
	}

	double process(std::vector<double> ax)
	{
		summ = b;
		x = ax;
		for (int a = 0; a < ax.size(); a++)
			summ += weight[a] * x[a];
		result = activationFunction(summ);
		return result;
	}

	void learning(double input)
	{
		//double err = (summ - input)/500;
		double error = input - result;
		double tatta = error * dactivationFunction(summ) * 0.3;
		for (int i = 0; i < weight.size(); i++)
			weight[i] += tatta * x[i];
		b += tatta;
	}
	double getWeight(int n)
	{
		return weight[n];
	}
};

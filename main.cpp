#include <iostream>
#include "Perec.hpp"
#include <thread>

void main()
{
	const int answers = 50;
	std::vector<Perceptron> perec(answers, Perceptron(2));

	while (true)
	{
		int cor = 0;
		int incor = 0;

		for (int i = 0; i <= answers / 2; i++)
			for (int j = 0; j < answers / 2; j++)
			{
				int x = i + j;
				std::vector<double> b(answers);
				b[i + j] = 0.5;
				for (int a = x - 1; a >= 0; a--)
					b[a] = b[a + 1] - 0.5 / answers;
				for (int a = x + 1; a < answers; a++)
					b[a] = b[a - 1] + 0.5 / answers;

				for (int i = 0; i < b.size(); ++i)
				{
					b[i] = activationFunction((i - x) / 500);
				}

				int num = -1;
				double mn = 1e9;
				for (int a = 0; a < perec.size(); a++)
				{
					double res = perec[a].process({ double(i), double(j) });
					if (std::abs(res - 0.5) < mn)
					{
						mn = std::abs(res - 0.5);
						num = a;
					}
					perec[a].learning(b[a]);
				}

				if (num == x) ++cor;
				else incor++;
			}

		std::cout << cor << " " << incor << std::endl;
	}

	for (int a = 0; a < answers; a++)
	{
		perec[a].process(std::vector<double> {50, 28});
		std::cout << perec[a].result << '\t';
	}
}

void mainn()
{
	const int answers = 100;
	//std::vector<Perceptron> perec(answers, Perceptron(2));
	std::vector<std::vector<double>>signals{ {0,0},{0,1},{1,0},{1,1} };
	Perceptron perec(2);
	while (true)
	{
		for (int a = 0; a < signals.size(); a++)
		{
			std::cout << perec.process(signals[a]) << " ";
			perec.learning(signals[a][0] * signals[a][1]);
			//std::cout << perec.process(signals[a]) << "  ";
		}
		std::cout << '\n';
	}
}

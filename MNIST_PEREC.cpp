#include <iostream>
#include <vector>
#include <cstdlib>
#include <SFML\Graphics.hpp>
#include <algorithm>
using namespace std;
struct MNIST
{
private:
	void loadTrainings(std::string nameimg, std::string namelbl)
	{
		FILE* file;
		fopen_s(&file, namelbl.c_str(), "rb");
		fseek(file, 4, SEEK_SET);
		count_of_images = maxX = maxY = 0;
		for (int i = 0; i < 4; i++)
		{
			count_of_images <<= 8;
			unsigned char k;
			fread(&k, 1, 1, file);
			count_of_images += unsigned int(k);
		}
		learn.resize(count_of_images);
		for (int i = 0; i < count_of_images; i++)
		{
			unsigned char k = 0;
			fread(&k, 1, 1, file);
			learn[i].label = k;
		}
		fclose(file);
		fopen_s(&file, nameimg.c_str(), "rb");
		fseek(file, 8, SEEK_SET);
		//count of rows
		for (int i = 0; i < 4; i++)
		{
			maxY << 8;
			int k = 0;
			fread(&k, 1, 1, file);
			maxY += k;
		}
		//columns
		for (int i = 0; i < 4; i++)
		{
			maxX << 8;
			int k = 0;
			fread(&k, 1, 1, file);
			maxX += k;
		}
		for (int i = 0; i < count_of_images; i++)
		{
			std::vector<unsigned char> p(maxX*maxY);
			for (int a = 0; a < maxY*maxX; a++)
				fread(&p[a], 1, 1, file);
			learn[i].pix = p;
		}
		fclose(file);
	}
	void loadTests(std::string nameimg, std::string namelbl)
	{
		FILE* file;
		fopen_s(&file, namelbl.c_str(), "rb");
		fseek(file, 4, SEEK_SET);
		count_of_tests = 0;
		for (int i = 0; i < 4; i++)
		{
			count_of_tests <<= 8;
			unsigned char k;
			fread(&k, 1, 1, file);
			count_of_tests += unsigned int(k);
		}
		test.resize(count_of_tests);
		for (int i = 0; i < count_of_tests; i++)
		{
			unsigned char k = 0;
			fread(&k, 1, 1, file);
			test[i].label = k;
		}
		fclose(file);
		fopen_s(&file, nameimg.c_str(), "rb");
		fseek(file, 16, SEEK_SET);
		for (int i = 0; i < count_of_tests; i++)
		{
			std::vector<unsigned char> p(maxX*maxY);
			for (int a = 0; a < maxY*maxX; a++)
				fread(&p[a], 1, 1, file);
			test[i].pix = p;
		}
		fclose(file);
	}
public:
	struct Image
	{
		std::vector<unsigned char> pix;
		unsigned char label;
	};
	std::vector<Image> learn;
	std::vector<Image> test;
	unsigned int count_of_images;
	unsigned int maxX;
	unsigned int maxY;
	unsigned int count_of_tests;
	MNIST(std::string train_img,std::string train_lbl,std::string test_img,std::string test_lbl)
	{
		loadTrainings(train_img, train_lbl);
		loadTests(test_img, test_lbl);
	}
};
double koef = 0.4f;
class Perceptron
{
private:
	double result;
	double b;
	double summ;
	vector<double> weight;
	vector<unsigned char> sm;
public:
	Perceptron(int n)
	{
		weight.resize(n);
		for (int a = 0; a < n; a++)
		{
			weight[a] = (double(rand() % 5) / 10.f - 0.5f);
		}
		b = double(rand() % 5) / 10.f - 0.5f;
	}
	double activationFunction(double summ1)
	{
		return 1. / (1. + exp(-summ1));
	}
	double dactivationFunction(double summ1)
	{
		return activationFunction(summ1) * (1 - activationFunction(summ1));
	}
	double process(vector <unsigned char>& x)
	{
		summ = b;
		for (int i = 0; i < x.size(); i++)
			summ += double(x[i]) * weight[i];
		sm = x;
		result = activationFunction(summ);
		return result;
	}
	void learning(double input)
	{
		double error = input - result;
		double tetta = error * dactivationFunction(result);
		for (int a = 0; a < weight.size(); a++)
			weight[a] += tetta * double(sm[a]) * koef;
		b += tetta * koef;
	}
	double getWeight(int n)
	{
		return weight[n];
	}
	double getResult()
	{
		return result;
	}
};
unsigned char returning(vector<unsigned char>& pix, vector<Perceptron>& perc, MNIST & mnist)
{
	double max1 = 0;
	char max2 = -1;
	for (int b = 0; b < 10; b++)
	{
		double ans = 0;
		ans = perc[b].process(pix);
		cout << double(perc[b].getResult())<<"  ";
		if (ans > max1)
		{
			max1 = ans;
			max2 = b;
		}
	}
	cout << endl;
	cout <<"Current num: "<< int(max2) << endl;
	return max2;
}
int main()
{
	srand(463715512);
	MNIST mnist("images.gz", "labels.gz","train_images.gz","train_labels.gz");
	vector<Perceptron> perc(10, Perceptron(mnist.maxX*mnist.maxY));
	for (int a = 0; a < mnist.count_of_images; a++)
	{
		for (int c = 0; c < 10; c++)
		{
			perc[c].process(mnist.learn[a].pix);
			perc[c].learning(double((mnist.learn[a].label == c)*0.9));
		}
	}
	/*int errors = 0;
	for (int a = 0; a < mnist.count_of_tests; a++)
	{
		if (returning(mnist.test[a].pix, perc, mnist) != mnist.test[a].label) errors++;
	}
	cout << "Ошибок: " << errors << "\tПроцент ошибок: " << (double(errors) / double(mnist.count_of_tests)) << endl;*/
	sf::RenderWindow window(sf::VideoMode(mnist.maxX * 10, mnist.maxY * 11), "Perec");
	std::vector<unsigned char> pix(mnist.maxX * mnist.maxY, 0);
	bool pressed = 0;
	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
			if (e.type == e.Closed)
				window.close();
		if ((sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)) && window.hasFocus())
		{
			sf::Vector2i pos(sf::Mouse::getPosition(window));
			pos.x %= mnist.maxX * 10;
			pos.y %= mnist.maxY*10;
			size_t k = (unsigned int(pos.x / 10 + pos.y / 10 * mnist.maxY));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) pix[k] = 255;
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) pix[k] = 0;
		}
		if (pressed && !sf::Keyboard::isKeyPressed(sf::Keyboard::T))
		{
			returning(pix, perc, mnist);
			//cout <<returning(pix, perc, mnist) << endl;
			pix.clear();
			pix.resize(mnist.maxX * mnist.maxY, 0);
		}
		pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::T);
		window.clear();
		for (int a = 0; a < mnist.maxX*mnist.maxY; a++)
		{
			sf::RectangleShape r(sf::Vector2f(10, 10));
			r.setPosition((a%mnist.maxX)*10,(a/mnist.maxY)*10);
			r.setFillColor(sf::Color(pix[a], pix[a], pix[a]));
			window.draw(r);
		}
		for (int a = 0; a < 10; a++)
		{
			sf::RectangleShape r(sf::Vector2f(mnist.maxX, mnist.maxY));
			r.setPosition(a*mnist.maxX, mnist.maxY*10);
			r.setFillColor(sf::Color(int(perc[a].getResult() * 127), int(perc[a].getResult() * 127), int(perc[a].getResult() * 127)));
			window.draw(r);
		}
		window.display();
	}
	return 0;
}

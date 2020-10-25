#include <iostream>
#include <vector>
#include <cstdlib>
#include <SFML\Graphics.hpp>
#include <algorithm>
using namespace std;
/*struct MNIST
{
	std::vector <std::vector<std::vector<unsigned char>>> label;
	unsigned int count_of_images;
	unsigned int maxX;
	unsigned int maxY;
	MNIST(std::string nameimg, std::string namelbl)
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
		label.resize(10);
		std::vector<unsigned char> lbl(count_of_images);
		for (int i = 0; i < count_of_images; i++)
		{
			char k = 0;
			fread(&k, 1, 1, file);
			lbl[i] = k;
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
			label[lbl[i]].push_back(p);
		}
		fclose(file);
	}
};*/
struct MNIST
{
	struct Image
	{
		std::vector<unsigned char> pix;
		unsigned char label;
	};
	std::vector<Image> img;
	unsigned int count_of_images;
	unsigned int maxX;
	unsigned int maxY;
	MNIST(std::string nameimg, std::string namelbl)
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
		img.resize(count_of_images);
		for (int i = 0; i < count_of_images; i++)
		{
			unsigned char k = 0;
			fread(&k, 1, 1, file);
			img[i].label = k;
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
			img[i].pix = p;
		}
		fclose(file);
	}
};
double koef = 0.3f;
double activationFunction(double summ1)
{
	return 1. / (1. + exp(-summ1));
}
double dactivationFunction(double summ1)
{
	return activationFunction(summ1) * (1 - activationFunction(summ1));
}
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
			weight[a]=(double(rand() % 5)/10.f - 0.5f);
		}
		b = double(rand() % 5)/10.f-0.5f;
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
};
unsigned char returning(vector<unsigned char>& pix, vector<Perceptron>& perc,MNIST & mnist)
{
	double max1 = 0;
	unsigned char max2 = 0;
	for (int b = 0; b < 10; b++)
	{
		double ans = 0;
		ans = perc[b].process(pix);
		if (ans > max1)
		{
			max1 = ans;
			max2 = b;
		}
	}
	return max2;
}
int main()
{
	srand(21350);
	MNIST mnist("images.gz", "labels.gz");
	vector<Perceptron> perc(10,Perceptron(mnist.maxX*mnist.maxY));
	for (int a = 0; a < mnist.count_of_images; a++)
	{
		for (int c = 0; c < 10; c++)
		{
			perc[c].process(mnist.img[a].pix);
			perc[c].learning(double(mnist.img[a].label == c));
		}
	}
	sf::RenderWindow window(sf::VideoMode(mnist.maxX*10,mnist.maxY*10), "Perec");
	bool pressed = 0;
	std::vector<unsigned char> pix(mnist.maxX * mnist.maxY,0);
	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
			if (e.type == e.Closed)
				window.close();
		pressed = 0;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window.hasFocus())
		{
			pressed = 1;
			sf::RectangleShape r(sf::Vector2f(10, 10));
			sf::Vector2i pos(sf::Mouse::getPosition(window));
			r.setPosition(sf::Vector2f(pos));
			r.setFillColor(sf::Color(127, 127, 127));
			window.draw(r);
			window.display();
			size_t k = (unsigned int(pos.x / 10 + pos.y / 10 * mnist.maxY));
			cout << k << endl;
			pix[k] = 255;
		}
		if (!pressed && !pix.empty())
		{
			cout << unsigned int(returning(pix, perc, mnist)) << endl;
			pix.clear();
			window.clear(sf::Color::Black);
			pressed = 0;
		}
	}
	return 0;
}

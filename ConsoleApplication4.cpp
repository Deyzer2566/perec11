#include <iostream>
#include <SFML\Graphics.hpp>
#include <vector>
#include <ctime>
#include <algorithm>
#include "Bot.hpp"
#define BOT vector<Bot>
#define width 800
#define height 600
using namespace std;
void res(BOT & b)
{
	std::vector<double> minh(b.size());
	std::vector<double> mins(b.size());
	for (int i = 0; i < b.size(); i++)
	{
		minh[i] = b[i].maxHunger;
		mins[i] = b[i].speed;
	}
	std::vector<double>::iterator is = std::min_element(mins.begin(), mins.end()),
		ih = std::min_element(minh.begin(), minh.end());
	cout <<distance(minh.begin(), ih)<<": "<< *ih << endl;
	cout <<distance(mins.begin(), is)<<": "<< *is << endl;
	cout << "#################" << endl;
}
int main()
{
	#ifndef _DEBUG
	srand(time(NULL));
	#endif
	sf::RenderWindow window(sf::VideoMode(width+100, height), "Evo");
	vector<sf::Vector2f> fruit(800);
	BOT bot(200);
	for (int i = 0;i<bot.size();i++)
		bot[i].neuron = Perceptron(3);
	for (int i = 0; i < fruit.size(); i++)
		fruit[i] = sf::Vector2f(double(rand() % width), double(rand() % height));
	bool pressed = 0;
	window.setFramerateLimit(60);
	int curbot = 0;
	while (window.isOpen())
	{
		sf::Event event;
		window.setTitle("Bots: " + to_string(bot.size()));
		while (window.pollEvent(event))
			if (event.type == event.Closed)
				window.close();
		window.clear();
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && window.hasFocus())
		{

			for (int i = 0; i < bot.size(); i++)
			{
				if (len(bot[i].pos, static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))) < 20)
				{
					curbot = i;
					break;
				}
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window.hasFocus())
		{
			if (!pressed || sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
			{
				Bot b;
				b.pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
				bot.push_back(b);
				pressed = 1;
			}
		}
		else pressed = 0;
		if(curbot != -1 && curbot < bot.size()){
			sf::RectangleShape bar(sf::Vector2f(bot[curbot].hunger / bot[curbot].maxHunger
				* 80.f, 10.f));
			bar.setFillColor(sf::Color::Red);
			bar.setPosition(width + 10, 15);
			window.draw(bar);
		}
		//Update bots
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			for (int i = 0; i < bot.size();)
			{
				bot[i].update(fruit, bot, i);
				if (bot[i].hunger <= 0.f)
				{
					//fruit.push_back(bot[i].pos);
					bot.erase((bot.begin() + i));
					if (i < curbot)
						curbot--;
					if (i == curbot)
						curbot = -1;
				}
				else
					i++;
			}
			res(bot);
		}
		for (std::vector<sf::Vector2f>::iterator it = fruit.begin(); it != fruit.end(); 
			++it)//отрисовка фруктов
		{
			sf::CircleShape c(10);
			c.setFillColor(sf::Color::Green);
			c.setPosition(*it);
			window.draw(c);
		}
		if (rand() % (int)1e4 > 9804 || fruit.size()<10)
			fruit.push_back(sf::Vector2f(double(rand() % width), double(rand() % height)));
		if (bot.size() < 10)
			bot.push_back(Bot());
		for (BOT::iterator it = bot.begin(); it != bot.end();it++)
		{
			sf::CircleShape c(10);
			c.setFillColor(sf::Color((it->hunger<=it->maxHunger)?
				(it->hunger/it->maxHunger*255.f):255,0,0));
			c.setOutlineColor(sf::Color::White);
			c.setOutlineThickness(1.f);
			c.setPosition(it->pos);
			window.draw(c);
		}
		window.display();
	}
}
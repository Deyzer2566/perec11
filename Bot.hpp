#ifndef BOT_HPP
#define BOT_HPP
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Perceptron.hpp"
using namespace std;
#define len(p1,p2) sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y))
#define width 800
#define height 600
#define BOT vector<Bot>
struct Bot
{
public:
	enum State { Eat =0,Robot=1}state;
	Perceptron neuron;
	Perceptron neuron2;
	sf::Vector2f pos;
	double hunger;
	float speed;
	double maxHunger;
	double damage;
	Bot();
	~Bot(){}
	Bot Razm(Bot& second);
	Bot& operator=(const Bot& right);
	bool operator==(const Bot& b);
	bool operator!=(const Bot& b);
	void update(vector<sf::Vector2f>& fruits, BOT& bot,int &itn);
};
#endif
#include "Bot.hpp"
#define _KOEF 0.3f
Bot::Bot() : neuron(1), pos(rand() % width, rand() % height), neuron2(1)
{
	maxHunger = (double)(rand() % 101) / 10;
	hunger = _KOEF*maxHunger;
	speed = 10.0 / maxHunger;
}
Bot Bot::Razm(Bot & second)
{
	Bot ret;
	for (int i = 0; i < ret.neuron.weight.size(); i++)
		ret.neuron.weight[i] = (rand() % 2 == 0) ? neuron.weight[i] : \
		second.neuron.weight[i] + (double)(rand() % 11) / 10.f - 0.5f;
	hunger -= _KOEF * maxHunger;
	second.hunger -= _KOEF * second.maxHunger;
	damage += (double)((rand() % 10) + 1);
	maxHunger += (double)((rand() % 2) - 1) * _KOEF;
	ret.pos = pos;
	ret.pos.x += (float)(rand() % 101 - 50);
	ret.pos.y += (float)(rand() % 101 - 50);
	return ret;
}

Bot& Bot::operator=(const Bot& right) {
	this->hunger = right.hunger;
	this->speed = right.speed;
	this->pos = right.pos;
	this->damage = right.damage;
	this->state = right.state;
	this->maxHunger = right.maxHunger;
	neuron = right.neuron;
	return *this;
}
bool Bot::operator==(const Bot& b)//oa 1 2 34 5W1mail
{
	return (pos == b.pos) && (b.neuron.weight == neuron.weight);
}
bool Bot::operator!=(const Bot& b)
{
	return !(*this==b);
}
void Bot::update(vector<sf::Vector2f>& fruits, BOT& bot,int & itn)
{
	hunger -= 0.01f;
	if (hunger > maxHunger)
		hunger = maxHunger;
	if (hunger <= 0.f)
		return;
	enum { TFruit = 0, TBot } typefood;
	double result = neuron.process({(hunger / maxHunger)});
	int elem = 0;
	sf::Vector2f tar;
	if (result > 0.5)//eat
	{
		state = Eat;
		vector<double> meanings(fruits.size() + bot.size() - 1);
		for (int i = 0,j=0; i < meanings.size(); i++)
		{
			if (i == fruits.size() + itn) {
				j = 1;
			}
			if (i < fruits.size())
				meanings[i] = neuron2.process({ len(fruits[i],pos) });
			else meanings[i] = neuron2.process({ len(bot[i + j - fruits.size()].pos,pos) });
		}
		std::vector<double>::iterator it = std::max_element(meanings.begin(), meanings.end());
		if (it - meanings.begin() < fruits.size())
		{
			elem = it - meanings.begin();
			typefood = TFruit;
			tar = *(it-meanings.begin()+fruits.begin());
		}
		else
		{
			elem = it - meanings.begin() - fruits.size();
			typefood = TBot;
			tar = bot[elem].pos;
		}
	}
	else
	{
		vector<double> meanings(bot.size() - 1);
		for (int i = 0, j = 0; i < meanings.size(); i++)
		{
			if (i == itn) {
				j = 1;
			}
			meanings[i] = neuron2.process({ len(bot[i + j].pos,pos) });
		}
		std::vector<double>::iterator it = std::max_element(meanings.begin(), meanings.end());
		elem = it - meanings.begin();
		tar = bot[elem].pos;
		state = Robot;
	}
	double angle;
	angle = atan2(double(tar.y - pos.y), double(tar.x - pos.x));
	sf::Vector2f move(cos(angle), sin(angle));
	move *= speed;
	if (len(tar, pos) > len(move, sf::Vector2f(0, 0)))
		this->pos += move;
	else
		this->pos = tar;
	if (pos == tar)
	{
		if (state == Eat)
		{
			if (typefood == TFruit)
			{
				fruits.erase(fruits.begin()+elem);
				hunger += 0.3;
			}
			else
			{
				if (bot[elem].hunger > damage)
					hunger += damage;
				else
					hunger = bot[elem].hunger;
				bot[elem].hunger -= damage;
				bot.erase((elem + bot.begin()));
				if (itn != 0)
					itn -= (itn >= elem);
			}
		}
		else
		{
			bot.push_back(Razm(bot[elem]));
		}
	}
}
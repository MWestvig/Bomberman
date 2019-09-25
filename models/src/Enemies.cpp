#include "../include/Enemies.hpp"

#include <RNG.hpp>

const int SAFE_ZONE = 2;
const int TOTAL_ENEMIES = 3; 

Enemies::Enemies()
{
}

Enemies::~Enemies()
{
	for(auto &e:this->list)
		delete e;
}

void Enemies::init(int level)
{
	this->list.remove_if([](IEnemy *bomb) { return true; });
}

void Enemies::updateAll(float deltaTime, const Map &map, Player &player)
{
	sf::Vector2f ppos = player.position();
	for(auto &e:this->list)
	{
		e->update(deltaTime, map, player);
		sf::Vector2f epos = e->position();
		float dist = sqrt(((ppos.x-epos.x)*(ppos.x-epos.x)) + ((ppos.y-epos.y)*(ppos.y-epos.y)));
		if (dist < 0.8)
		{
			player.kill();
		}
	}
}

void Enemies::populate()
{
	Finder *f = new Finder();
	list.push_back(f);
}

void Enemies::populate(int numEnemies, int level, const Map &map)
{
	int enemyType;
	IEnemy *e;
	
	//generate coordinates here
	//only generates on uneven blocks
	//check if tile at coordinates exist
	//rerun random number generator

	for(int i = 0; i < numEnemies; i++)
	{
		sf::Vector2f start(0, 0);
		while(map.tileAt(sf::Vector2i(start)) != Tile::Clear)
		{
			int x = RNG::getRandomNumber(SAFE_ZONE, ((map.size().x / 2) - 1)) * 2 + 1;
			int y = RNG::getRandomNumber(SAFE_ZONE, ((map.size().y / 2) - 1)) * 2 + 1;
			start = sf::Vector2f(x + 0.5, y + 0.5);
		}
		enemyType = RNG::getRandomNumber(0, level);
		switch (enemyType)
		{
			case 0:
				e = new IEnemy(sf::Vector2f(start));
				break;
			case 1:
				e = new Ballom(sf::Vector2f(start));
				break;
			case 2:
				e = new Finder(sf::Vector2f(start));
				break;
			default:
				e = new IEnemy(sf::Vector2f(start));
				break;
		}
		list.push_back(e);
	}
}

void Enemies::populate(int numEnemies)
{
	int enemyType;
	IEnemy *e;
	for(int i = 0; i < numEnemies; i++)
	{
		enemyType = (RNG::getRandomNumber(0, TOTAL_ENEMIES - 1));
		switch (enemyType)
		{
			case 0:
				e = new IEnemy();
				break;
			case 1:
				e = new Ballom();
				break;
			case 2:
				e = new Finder();
				break;
			default:
				e = new IEnemy();
				break;
		}
		list.push_back(e);
	}
}

void Enemies::kill(const Map &map)
{
	auto comp = [](Tile tile)
	{
		return tile == Tile::Flame;
	};
	std::list<IEnemy*> enemiesToKill;
	for(auto &e : this->list)
	{
		if (map.collide(e->position(), 0.49, comp))
			enemiesToKill.push_back(e);
	}
	for(auto &e:enemiesToKill)
	{
		delete e;
		list.remove(e);
	}
	enemiesToKill.clear();
}
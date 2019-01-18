#pragma once
#include "Equasion.h"
#include "RandomNumber.h"
#include <mutex>

struct Coord
{
	int x = 0;
	int y = 0;
};

struct threadData
{
	RandomNumber *rnd;
	vector<Coord> *whiteSpaces;
	vector<Coord> *blackSpaces;
	Grid *grid;
	Equasion *equasion;
	mutex *vectorWhiteWrite;
	mutex *vectorBlackWrite;
	int width;
	int height;
	int startX;
	int endX;
	int startY;
	int endY;
};

class Segregation :
	public Equasion
{
protected:
	HANDLE  hThreadArray[NUMBER_OF_RULE_THREADS];
	RandomNumber rnd;
	vector<Coord> whiteSpaces;
	vector<Coord> blackSpaces;
	mutex vectorWhiteAccess;
	mutex vectorBlackAccess;
	mutex cellOverwrite;
public:
	Segregation(Grid *gridIn);
	~Segregation();
	bool applyRule(int x, int y, int z, Grid *outputGrid);
	void initRule();
};


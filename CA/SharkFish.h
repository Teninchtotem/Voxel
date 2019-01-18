#pragma once
#include "Equasion.h"
#include "RandomNumber.h"
class SharkFish :
	public Equasion
{
protected:
	RandomNumber rnd;

public:
	SharkFish(Grid *gridIn);
	~SharkFish();
	bool applyRule(int x, int y, int z, Grid *outputGrid);
	int getNumberOfBreedingSharks(Cell **cells);
	int getNumberOfBreedingFish(Cell **cells);
};


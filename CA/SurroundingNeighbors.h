#pragma once
#include "NeighborBehaviour.h"

class SurroundingNeighbors :
	public NeighborBehaviour
{
public:
	SurroundingNeighbors(void);
	~SurroundingNeighbors(void);

	virtual Cell **getNeighbors(int x, int y, int z, Grid *grid);
};


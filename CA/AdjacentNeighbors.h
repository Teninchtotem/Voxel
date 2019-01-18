#pragma once
#include "NeighborBehaviour.h"

class AdjacentNeighbors :
	public NeighborBehaviour
{
public:
	AdjacentNeighbors(void);
	~AdjacentNeighbors(void);

	virtual Cell **getNeighbors(int x, int y, int z, Grid *grid);
};


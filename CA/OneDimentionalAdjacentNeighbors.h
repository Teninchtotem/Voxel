#pragma once
#include "NeighborBehaviour.h"

class OneDimentionalAdjacentNeighbors :
	public NeighborBehaviour
{
public:
	OneDimentionalAdjacentNeighbors(void);
	~OneDimentionalAdjacentNeighbors(void);

	virtual Cell **getNeighbors(int x, int y, int z, Grid *grid);
};


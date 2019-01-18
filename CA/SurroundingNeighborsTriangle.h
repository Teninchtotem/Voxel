#pragma once
#include "NeighborBehaviour.h"

class SurroundingNeighborsTriangle :
	public NeighborBehaviour
{
public:
	SurroundingNeighborsTriangle(void);
	~SurroundingNeighborsTriangle(void);

	virtual Cell **getNeighbors(int x, int y, int z, Grid *grid);
};


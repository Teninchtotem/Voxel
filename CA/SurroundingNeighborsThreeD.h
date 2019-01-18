#pragma once
#include "NeighborBehaviour.h"

class SurroundingNeighborsThreeD :
	public NeighborBehaviour
{
public:
	SurroundingNeighborsThreeD(void);
	~SurroundingNeighborsThreeD(void);

	virtual Cell **getNeighbors(int x, int y, int z, Grid *grid);
};


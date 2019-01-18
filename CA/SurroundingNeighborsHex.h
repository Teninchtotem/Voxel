#pragma once
#include "NeighborBehaviour.h"

class SurroundingNeighborsHex :
	public NeighborBehaviour
{
public:
	SurroundingNeighborsHex(void);
	~SurroundingNeighborsHex(void);

	virtual Cell **getNeighbors(int x, int y, int z, Grid *grid);
};


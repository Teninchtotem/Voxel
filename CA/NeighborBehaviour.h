#pragma once
#include "Grid.h"

class NeighborBehaviour
{
protected:
	Cell *neighbors;
	int noOfNeighbors;
	int behaviourID;
	bool looping;
public:
	NeighborBehaviour(void);
	~NeighborBehaviour(void);

	void setNoOfNeighbors(int neighborsIn);

	void setLooping(bool in);
	int getNoOfNeighbors();
	int getBehaviourID() { return behaviourID; };
	virtual Cell **getNeighbors(int x, int y, int z, Grid *grid);
};




#include "NeighborBehaviour.h"


NeighborBehaviour::NeighborBehaviour(void)
{
	behaviourID = 0;
	noOfNeighbors = 0;
}


NeighborBehaviour::~NeighborBehaviour(void)
{

}

void NeighborBehaviour::setLooping(bool in)
{ 
	looping = in; 
}
Cell **NeighborBehaviour::getNeighbors(int x, int y, int z, Grid *grid)
{
	return 0;
}

void NeighborBehaviour::setNoOfNeighbors(int neighborsIn)
{
	noOfNeighbors = neighborsIn;
}

int NeighborBehaviour::getNoOfNeighbors()
{
	return noOfNeighbors;
}
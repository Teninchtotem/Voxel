

#include "SurroundingNeighborsTriangle.h"


SurroundingNeighborsTriangle::SurroundingNeighborsTriangle(void)
{
	behaviourID = 5;
	noOfNeighbors = 3;
}


SurroundingNeighborsTriangle::~SurroundingNeighborsTriangle(void)
{
}


Cell **SurroundingNeighborsTriangle::getNeighbors(int x, int y, int z, Grid *grid)
{
	bool leftSafe = false;
	bool rightSafe = false;
	bool upSafe = false;
	bool downSafe = false;

	Cell **cell = new Cell*[3]; //fixed new
	for(int i = 0; i < 3; i++)
		cell[i] = NULL;

	if (x - 1 >= 0)
		cell[0] = grid->getCell(x-1,y,z);
	else if (looping)
		cell[0] = grid->getCell(grid->getWidth() - 1, y, z);	
	if (x + 1 < grid->getWidth())
		cell[1] = grid->getCell(x+1,y,z);
	else if (looping)
		cell[1] = grid->getCell(0, y, z);

	if (y - 1 >= 0)
		downSafe = true;
	if (y + 1 < grid->getHeight())
		upSafe = true;
	
	if ((x + y) % 2 == 1)
	{
		if (upSafe)
			cell[2] = grid->getCell(x, y + 1, z);
		else if (looping)
			cell[2] = grid->getCell(x, 0, z);
	}
	else
	{
		if (downSafe)
			cell[2] = grid->getCell(x, y - 1, z);
		else if (looping)
			cell[2] = grid->getCell(x, grid->getHeight() - 1, z);
	}

	return cell;
}
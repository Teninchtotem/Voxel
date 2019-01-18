

#include "OneDimentionalAdjacentNeighbors.h"


OneDimentionalAdjacentNeighbors::OneDimentionalAdjacentNeighbors(void)
{
	behaviourID = 1;
	noOfNeighbors = 3;
}


OneDimentionalAdjacentNeighbors::~OneDimentionalAdjacentNeighbors(void)
{
}


Cell **OneDimentionalAdjacentNeighbors::getNeighbors(int x, int y, int z, Grid *grid)
{
	Cell **cell = new Cell*[3]; // fixed new
	for(int i = 0; i < 3; i++)
		cell[i] = NULL;
	
	int tempY = y-1;
	if (y - 1 < 0)
		tempY = grid->getHeight() - 1;

	
	if(x-1 >= 0)
		cell[0] = grid->getCell(x - 1, tempY, z);
	else if (looping)
		cell[0] = grid->getCell(grid->getWidth() - 1, tempY, z);

		cell[1] = grid->getCell(x, tempY, z);

	if(x+1 < grid->getWidth())
		cell[2] = grid->getCell(x + 1, tempY, z);
	else if (looping)
		cell[2] = grid->getCell(0, tempY, z);

	return cell;
}

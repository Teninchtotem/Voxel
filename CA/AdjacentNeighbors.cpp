

#include "AdjacentNeighbors.h"


AdjacentNeighbors::AdjacentNeighbors(void)
{
	noOfNeighbors = 4;
}


AdjacentNeighbors::~AdjacentNeighbors(void)
{
}


Cell **AdjacentNeighbors::getNeighbors(int x, int y, int z, Grid *grid)
{
	Cell **cell = new Cell*[4]; // fixed new
	for(int i = 0; i < 4; i++)
		cell[i] = NULL;
	

	if(x-1 >= 0)
		cell[0] = grid->getCell(x-1,y,z);
	else if (looping)
		cell[0] = grid->getCell(grid->getWidth() - 1, y, z);
	if(x+1 < grid->getWidth())
		cell[1] = grid->getCell(x + 1, y, z);
	else if (looping)
		cell[1] = grid->getCell(0, y, z);
		
	if(y-1 >= 0)
		cell[2] = grid->getCell(x, y - 1, z);
	else if (looping)
		cell[2] = grid->getCell(x, grid->getHeight() - 1, z);
	if(y+1 < grid->getHeight())
		cell[3] = grid->getCell(x, y + 1, z);
	else if (looping)
		cell[3] = grid->getCell(x, 0, z);

	return cell;
}

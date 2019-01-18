

#include "SurroundingNeighbors.h"


SurroundingNeighbors::SurroundingNeighbors(void)
{
	behaviourID = 2;
	noOfNeighbors = 8;
}


SurroundingNeighbors::~SurroundingNeighbors(void)
{
}


Cell **SurroundingNeighbors::getNeighbors(int x, int y, int z, Grid *grid)
{
	bool leftSafe = false;
	bool rightSafe = false;
	bool upSafe = false;
	bool downSafe = false;

	Cell **cell = new Cell*[8]; //fixed new
	for(int i = 0; i < 8; i++)
		cell[i] = NULL;

	if(x-1 >= 0)
		leftSafe = true;
	if(x+1 < grid->getWidth())
		rightSafe = true;	

	if(y-1 >= 0)
	{
		cell[6] = grid->getCell(x,y-1,z);
		downSafe = true;
	}
	else if (looping)
	{
		cell[6] = grid->getCell(x, grid->getHeight() - 1, z);
	}
	if(y+1 < grid->getHeight())
	{
		cell[7] = grid->getCell(x,y+1,z);
		upSafe = true;
	}
	else if (looping)
	{
		cell[7] = grid->getCell(x, 0, z);
	}
	
	if(leftSafe)
	{
		cell[0] = grid->getCell(x-1,y,z);

		if(upSafe)
		{
			cell[1] = grid->getCell(x-1,y+1,z);
		}
		else if (looping)
		{
			cell[1] = grid->getCell(x - 1, 0, z);
		}
		if(downSafe)
		{
			cell[2] = grid->getCell(x-1,y-1,z);
		}
		else if (looping)
		{
			cell[2] = grid->getCell(x - 1, grid->getHeight() - 1, z);
		}
	}
	else if (looping)
	{
		cell[0] = grid->getCell(grid->getWidth() - 1, y, z);
		if (!upSafe)
		{
			cell[1] = grid->getCell(grid->getWidth() - 1, 0, z);
		}
		else
		{
			cell[1] = grid->getCell(grid->getWidth() - 1, y+1, z);
		}
		if (!downSafe)
		{
			cell[2] = grid->getCell(grid->getWidth() - 1, grid->getHeight() - 1, z);
		}
		else
		{
			cell[2] = grid->getCell(grid->getWidth() - 1, y-1, z);
		}
	}
	if(rightSafe)
	{
		cell[3] = grid->getCell(x+1,y,z);

		if(upSafe)
		{
			cell[4] = grid->getCell(x+1,y+1,z);
		}
		else if (looping)
		{
			cell[4] = grid->getCell(x + 1, 0, z);
		}
		if(downSafe)
		{
			cell[5] = grid->getCell(x+1,y-1,z);
		}
		else if (looping)
		{
			cell[5] = grid->getCell(x + 1, grid->getHeight() - 1, z);
		}
	}
	else if (looping)
	{
		cell[3] = grid->getCell(0, y, z);
		if (!upSafe)
		{
			cell[4] = grid->getCell(0, 0, z);
		}
		else
		{
			cell[4] = grid->getCell(0, y + 1, z);
		}
		if (!downSafe)
		{
			cell[5] = grid->getCell(0, grid->getHeight() - 1, z);
		}
		else
		{
			cell[5] = grid->getCell(0, y - 1, z);
		}
	}
	return cell;
}
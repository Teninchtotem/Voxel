

#include "SurroundingNeighborsHex.h"


SurroundingNeighborsHex::SurroundingNeighborsHex(void)
{
	behaviourID = 3;
	noOfNeighbors = 6;
}


SurroundingNeighborsHex::~SurroundingNeighborsHex(void)
{
}


Cell **SurroundingNeighborsHex::getNeighbors(int x, int y, int z, Grid *grid)
{
	bool leftSafe = false;
	bool rightSafe = false;
	bool upSafe = false;
	bool downSafe = false;

	Cell **cell = new Cell*[6]; //fixed new

	for(int i = 0; i < 6; i++)
		cell[i] = NULL;


	if (x - 1 >= 0)
		leftSafe = true;
	if (x + 1 < grid->getWidth())
		rightSafe = true;
	if (y - 1 >= 0)
		downSafe = true;
	if (y + 1 < grid->getHeight())
		upSafe = true;

	if (y - 2 >= 0)
	{
		cell[0] = grid->getCell(x,y-2,z);
	}
	else if (looping)
	{
		if (grid->getHeight() % 2 == 0)
		{
			if (downSafe)
				cell[0] = grid->getCell(x, grid->getHeight() - 1, z);
			else
				cell[0] = grid->getCell(x, grid->getHeight() - 2, z);
		}
		else
		{
			if (downSafe)
				cell[0] = grid->getCell(x, grid->getHeight() - 2, z);
			else
				cell[0] = grid->getCell(x, grid->getHeight() - 3, z);
		}
	}

	if (y + 2 < grid->getHeight())
	{
		cell[3] = grid->getCell(x,y+2,z);
	}
	else if (looping)
	{
		if (grid->getHeight() % 2 == 0)
		{
			if (upSafe)
				cell[3] = grid->getCell(x, 0, z);
			else
				cell[3] = grid->getCell(x, 1, z);
		}
		else
		{
			if (upSafe)
				cell[3] = grid->getCell(x, 1, z);
			else
				cell[3] = grid->getCell(x, 2, z);
		}
	}
	
	if (y % 2 == 1)	//Odd		indented hex
	{
		if (upSafe)
		{
			cell[5] = grid->getCell(x, y + 1, z);
			if (rightSafe)
				cell[1] = grid->getCell(x + 1, y + 1, z);
			else if (looping)
				cell[1] = grid->getCell(0, y + 1, z);
		}
		else if (looping)
		{
			cell[5] = grid->getCell(x, 0, z);
			if (rightSafe)
				cell[1] = grid->getCell(x + 1, 0, z);
			else if (looping)
				cell[1] = grid->getCell(0, 0, z);
		}
		if (downSafe)
		{
			cell[4] = grid->getCell(x, y - 1, z);
			if (rightSafe)
				cell[2] = grid->getCell(x + 1, y - 1, z);
			else if (looping)
				cell[2] = grid->getCell(0, y - 1, z);
		}
		else if (looping)
		{
			cell[4] = grid->getCell(x, grid->getHeight() - 1, z);
			if (rightSafe)
				cell[2] = grid->getCell(x + 1, grid->getHeight() - 1, z);
			else if (looping)
				cell[2] = grid->getCell(0, grid->getHeight() - 1, z);
		}
	}
	else					//Odd		edge hex
	{
		if (upSafe)
		{
			cell[1] = grid->getCell(x, y + 1, z);
			if (leftSafe)
				cell[5] = grid->getCell(x - 1, y + 1, z);
			else if (looping)
				cell[5] = grid->getCell(grid->getWidth() - 1, y + 1, z);
		}
		else if (looping)
		{
			cell[1] = grid->getCell(x, 0, z);
			if (leftSafe)
				cell[5] = grid->getCell(x - 1, 0, z);
			else if (looping)
				cell[5] = grid->getCell(grid->getWidth() - 1, 0, z);
		}
		if (downSafe)
		{
			cell[2] = grid->getCell(x, y - 1, z);
			if (leftSafe)
				cell[4] = grid->getCell(x - 1, y - 1, z);
			else if (looping)
				cell[4] = grid->getCell(grid->getWidth() - 1, y - 1, z);
		}
		else if (looping)
		{
			cell[2] = grid->getCell(x, grid->getHeight() - 1, z);
			if (leftSafe)
				cell[4] = grid->getCell(x - 1, grid->getHeight() - 1, z);
			else if (looping)
				cell[4] = grid->getCell(grid->getWidth() - 1, grid->getHeight() - 1, z);
		}
	}

	return cell;
}
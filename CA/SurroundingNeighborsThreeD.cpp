

#include "SurroundingNeighborsThreeD.h"


SurroundingNeighborsThreeD::SurroundingNeighborsThreeD(void)
{
	behaviourID = 4;
	noOfNeighbors = 26;
}


SurroundingNeighborsThreeD::~SurroundingNeighborsThreeD(void)
{
}


Cell **SurroundingNeighborsThreeD::getNeighbors(int x, int y, int z, Grid *grid)
{
	bool leftSafe = false;
	bool rightSafe = false;
	bool upSafe = false;
	bool downSafe = false;
	bool forwardSafe = false;
	bool backSafe = false;

	Cell **cell = new Cell*[26]; //fixed new
	for (int i = 0; i < 26; i++)
	{
		cell[i] = NULL;
	}	

	if(x-1 >= 0)
		leftSafe = true;
	if(x+1 < grid->getWidth())
		rightSafe = true;	

	if (z + 1 < grid->getDepth())
	{
		cell[8] = grid->getCell(x, y, z + 1);
		forwardSafe = true;
	}
	else if (looping)
	{
		cell[8] = grid->getCell(x, y, 0);
	}
	if (z - 1 >= 0)
	{
		cell[17] = grid->getCell(x, y, z - 1);
		backSafe = true;
	}
	else if (looping)
	{
		cell[17] = grid->getCell(x, y, grid->getDepth() - 1);
	}

	if(y-1 >= 0)
	{
		downSafe = true;
		cell[6] = grid->getCell(x, y - 1, z);
		if (forwardSafe)
			cell[15] = grid->getCell(x, y - 1, z + 1);
		else if (looping)
			cell[15] = grid->getCell(x, y - 1, 0);
		if (backSafe)
			cell[24] = grid->getCell(x, y - 1, z - 1);
		else if (looping)
			cell[24] = grid->getCell(x, y - 1, grid->getDepth() - 1);
	}
	else if (looping)
	{
		cell[6] = grid->getCell(x, grid->getHeight() - 1, z);
		if (forwardSafe)
			cell[15] = grid->getCell(x, grid->getHeight() - 1, z + 1);
		else if (looping)
			cell[15] = grid->getCell(x, grid->getHeight() - 1, 0);
		if (backSafe)
			cell[24] = grid->getCell(x, grid->getHeight() - 1, z - 1);
		else if (looping)
			cell[24] = grid->getCell(x, grid->getHeight() - 1, grid->getDepth() - 1);
	}
	if (y + 1 < grid->getHeight())
	{
		upSafe = true;
		cell[7] = grid->getCell(x, y + 1, z);
		if (forwardSafe)
			cell[16] = grid->getCell(x, y + 1, z + 1);
		else if (looping)
			cell[16] = grid->getCell(x, y + 1, 0);
		if (backSafe)
			cell[25] = grid->getCell(x, y + 1, z - 1);
		else if (looping)
			cell[25] = grid->getCell(x, y + 1, grid->getDepth() - 1);
	}
	else if (looping)
	{
		cell[7] = grid->getCell(x, 0, z);
		if (forwardSafe)
			cell[16] = grid->getCell(x, 0, z + 1);
		else if (looping)
			cell[16] = grid->getCell(x, 0, 0);
		if (backSafe)
			cell[25] = grid->getCell(x, 0, z - 1);
		else if (looping)
			cell[25] = grid->getCell(x, 0, grid->getDepth() - 1);
	}
	if(leftSafe)
	{
		cell[0] = grid->getCell(x-1, y, z);
		if (forwardSafe)
			cell[9] = grid->getCell(x - 1, y, z + 1);
		else if (looping)
			cell[9] = grid->getCell(x - 1, y, 0);
		if (backSafe)
			cell[18] = grid->getCell(x - 1, y, z-1);
		else if (looping)
			cell[18] = grid->getCell(x - 1, y, grid->getDepth() - 1);
		if(upSafe)
		{
			cell[1] = grid->getCell(x - 1, y + 1, z);
			if (forwardSafe)
				cell[10] = grid->getCell(x - 1, y + 1, z + 1);
			else if (looping)
				cell[10] = grid->getCell(x - 1, y + 1, 0);
			if (backSafe)
				cell[19] = grid->getCell(x - 1, y + 1, z - 1);
			else if (looping)
				cell[19] = grid->getCell(x - 1, y + 1, grid->getDepth() - 1);
		}
		else if (looping)
		{
			cell[1] = grid->getCell(x - 1, 0, z);
			if (forwardSafe)
				cell[10] = grid->getCell(x - 1, 0, z + 1);
			else if (looping)
				cell[10] = grid->getCell(x - 1, 0, 0);
			if (backSafe)
				cell[19] = grid->getCell(x - 1, 0, z - 1);
			else if (looping)
				cell[19] = grid->getCell(x - 1, 0, grid->getDepth() - 1);
		}
		if(downSafe)
		{
			cell[2] = grid->getCell(x - 1, y - 1, z);
			if (forwardSafe)
				cell[11] = grid->getCell(x - 1, y - 1, z + 1);
			else if (looping)
				cell[11] = grid->getCell(x - 1, y - 1, 0);
			if (backSafe)
				cell[20] = grid->getCell(x - 1, y - 1, z - 1);
			else if (looping)
				cell[20] = grid->getCell(x - 1, y - 1, grid->getDepth() - 1);
		}
		else if (looping)
		{
			cell[2] = grid->getCell(x - 1, grid->getHeight() - 1, z);
			if (forwardSafe)
				cell[11] = grid->getCell(x - 1, grid->getHeight() - 1, z + 1);
			else if (looping)
				cell[11] = grid->getCell(x - 1, grid->getHeight() - 1, 0);
			if (backSafe)
				cell[20] = grid->getCell(x - 1, grid->getHeight() - 1, z - 1);
			else if (looping)
				cell[20] = grid->getCell(x - 1, grid->getHeight() - 1, grid->getDepth() - 1);
		}
	}
	else if (looping)
	{
		cell[0] = grid->getCell(grid->getWidth() - 1, y, z);
		if (forwardSafe)
			cell[9] = grid->getCell(grid->getWidth() - 1, y, z + 1);
		else
			cell[9] = grid->getCell(grid->getWidth() - 1, y, 0);
		if (backSafe)
			cell[18] = grid->getCell(grid->getWidth() - 1, y, z - 1);
		else
			cell[18] = grid->getCell(grid->getWidth() - 1, y, grid->getDepth() - 1);
		if (!upSafe)
		{
			cell[1] = grid->getCell(grid->getWidth() - 1, 0, z);
			if (forwardSafe)
				cell[10] = grid->getCell(grid->getWidth() - 1, 0, z + 1);
			else if (looping)
				cell[10] = grid->getCell(grid->getWidth() - 1, 0, 0);
			if (backSafe)
				cell[19] = grid->getCell(grid->getWidth() - 1, 0, z - 1);
			else if (looping)
				cell[19] = grid->getCell(grid->getWidth() - 1, 0, grid->getDepth() - 1);
		}
		else
		{
			cell[1] = grid->getCell(grid->getWidth() - 1, y + 1, z);
			if (forwardSafe)
				cell[10] = grid->getCell(grid->getWidth() - 1, y + 1, z + 1);
			else if (looping)
				cell[10] = grid->getCell(grid->getWidth() - 1, y + 1, 0);
			if (backSafe)
				cell[19] = grid->getCell(grid->getWidth() - 1, y + 1, z - 1);
			else if (looping)
				cell[19] = grid->getCell(grid->getWidth() - 1, y + 1, grid->getDepth() - 1);
		}
		if (!downSafe)
		{
			cell[2] = grid->getCell(grid->getWidth() - 1, grid->getHeight() - 1, z);
			if (forwardSafe)
				cell[11] = grid->getCell(grid->getWidth() - 1, grid->getHeight() - 1, z + 1);
			else if (looping)
				cell[11] = grid->getCell(grid->getWidth() - 1, grid->getHeight() - 1, 0);
			if (backSafe)
				cell[20] = grid->getCell(grid->getWidth() - 1, grid->getHeight() - 1, z - 1);
			else if (looping)
				cell[20] = grid->getCell(grid->getWidth() - 1, grid->getHeight() - 1, grid->getDepth() - 1);
		}
		else
		{
			cell[2] = grid->getCell(grid->getWidth() - 1, y - 1, z);
			if (forwardSafe)
				cell[11] = grid->getCell(grid->getWidth() - 1, y - 1, z + 1);
			else if (looping)
				cell[11] = grid->getCell(grid->getWidth() - 1, y - 1, 0);
			if (backSafe)
				cell[20] = grid->getCell(grid->getWidth() - 1, y - 1, z - 1);
			else if (looping)
				cell[20] = grid->getCell(grid->getWidth() - 1, y - 1, grid->getDepth() - 1);
		}
	}
	if(rightSafe)
	{
		cell[3] = grid->getCell(x + 1, y, z);
		if (forwardSafe)
			cell[12] = grid->getCell(x + 1, y, z + 1);
		else if (looping)
			cell[12] = grid->getCell(x + 1, y, 0);
		if (backSafe)
			cell[21] = grid->getCell(x + 1, y, z - 1);
		else if (looping)
			cell[21] = grid->getCell(x + 1, y, grid->getDepth() - 1);

		if(upSafe)
		{
			cell[4] = grid->getCell(x + 1, y + 1, z);
			if (forwardSafe)
				cell[13] = grid->getCell(x + 1, y + 1, z + 1);
			else if (looping)
				cell[13] = grid->getCell(x + 1, y + 1, 0);
			if (backSafe)
				cell[22] = grid->getCell(x + 1, y + 1, z - 1);
			else if (looping)
				cell[22] = grid->getCell(x + 1, y + 1, grid->getDepth() - 1);
		}
		else if (looping)
		{
			cell[4] = grid->getCell(x + 1, 0, z);
			if (forwardSafe)
				cell[13] = grid->getCell(x + 1, 0, z + 1);
			else if (looping)
				cell[13] = grid->getCell(x + 1, 0, 0);
			if (backSafe)
				cell[22] = grid->getCell(x + 1, 0, z - 1);
			else if (looping)
				cell[22] = grid->getCell(x + 1, 0, grid->getDepth() - 1);
		}
		if(downSafe)
		{
			cell[5] = grid->getCell(x + 1, y - 1, z);
			if (forwardSafe)
				cell[14] = grid->getCell(x + 1, y - 1, z + 1);
			else if (looping)
				cell[14] = grid->getCell(x + 1, y - 1, 0);
			if (backSafe)
				cell[23] = grid->getCell(x + 1, y - 1, z - 1);
			else if (looping)
				cell[23] = grid->getCell(x + 1, y - 1, grid->getDepth() - 1);
		}
		else if (looping)
		{
			cell[5] = grid->getCell(x + 1, grid->getHeight() - 1, z);
			if (forwardSafe)
				cell[14] = grid->getCell(x + 1, grid->getHeight() - 1, z + 1);
			else if (looping)
				cell[14] = grid->getCell(x + 1, grid->getHeight() - 1, 0);
			if (backSafe)
				cell[23] = grid->getCell(x + 1, grid->getHeight() - 1, z - 1);
			else if (looping)
				cell[23] = grid->getCell(x + 1, grid->getHeight() - 1, grid->getDepth() - 1);
		}
	}
	else if (looping)
	{
		cell[3] = grid->getCell(0, y, z);
		if (forwardSafe)
			cell[12] = grid->getCell(0, y, z + 1);
		else if (looping)
			cell[12] = grid->getCell(0, y, 0);
		if (backSafe)
			cell[21] = grid->getCell(0, y, z - 1);
		else if (looping)
			cell[21] = grid->getCell(0, y, grid->getDepth() - 1);
		if (!upSafe)
		{
			cell[4] = grid->getCell(0, 0, z);
			if (forwardSafe)
				cell[13] = grid->getCell(0, 0, z + 1);
			else if (looping)
				cell[13] = grid->getCell(0, 0, 0);
			if (backSafe)
				cell[22] = grid->getCell(0, 0, z - 1);
			else if (looping)
				cell[22] = grid->getCell(0, 0, grid->getDepth() - 1);
		}
		else
		{
			cell[4] = grid->getCell(0, y + 1, z);
			if (forwardSafe)
				cell[13] = grid->getCell(0, y + 1, z + 1);
			else if (looping)
				cell[13] = grid->getCell(0, y + 1, 0);
			if (backSafe)
				cell[22] = grid->getCell(0, y + 1, z - 1);
			else if (looping)
				cell[22] = grid->getCell(0, y + 1, grid->getDepth() - 1);
		}
		if (!downSafe)
		{
			cell[5] = grid->getCell(0, grid->getHeight() - 1, z);
			if (forwardSafe)
				cell[14] = grid->getCell(0, grid->getHeight() - 1, z + 1);
			else if (looping)
				cell[14] = grid->getCell(0, grid->getHeight() - 1, 0);
			if (backSafe)
				cell[23] = grid->getCell(0, grid->getHeight() - 1, z - 1);
			else if (looping)
				cell[23] = grid->getCell(0, grid->getHeight() - 1, grid->getDepth() - 1);
		}
		else
		{
			cell[5] = grid->getCell(0, y - 1, z);
			if (forwardSafe)
				cell[14] = grid->getCell(0, y - 1, z + 1);
			else if (looping)
				cell[14] = grid->getCell(0, y - 1, 0);
			if (backSafe)
				cell[23] = grid->getCell(0, y - 1, z - 1);
			else if (looping)
				cell[23] = grid->getCell(0, y - 1, grid->getDepth() - 1);
		}
	}

	return cell;
}
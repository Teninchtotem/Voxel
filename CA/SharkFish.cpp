#include "SharkFish.h"


SharkFish::SharkFish(Grid *gridIn)
{
	randID = 1;
	grid = gridIn;
	neighborBehaviour = new SurroundingNeighbors();
	equasionType = -99;
}


SharkFish::~SharkFish()
{
}
bool SharkFish::applyRule(int x, int y, int z, Grid *outputGrid)
{
	//Sharks will be 1
	//Fish will be 2

	Cell **tempNeighbors;
	tempNeighbors = neighborBehaviour->getNeighbors(x, y, z, grid);
	int sharks = neighborCounter(tempNeighbors, 2);
	int fish = neighborCounter(tempNeighbors, 1);

	if (grid->getCell(x, y, z)->getCellID() == 0)
	{
		//Shark reproduction
		if (sharks > 3)
		{
			if (getNumberOfBreedingSharks(tempNeighbors) > 2)
			{
				if (fish < 4)
				{
					outputGrid->setCellID(x, y, z, 2); 
					if (rnd.random(0, 1))
						outputGrid->getCell(x, y, z)->setCellState(1);
					else
						outputGrid->getCell(x, y, z)->setCellState(2);
				}
			}
		}
		//Fish reproduction
		else if (fish > 3)
		{
			if (getNumberOfBreedingFish(tempNeighbors) > 2)
			{
				if (sharks < 4)
				{
					outputGrid->setCellID(x, y, z, 1);
					outputGrid->getCell(x, y, z)->setCellState(2);
				}
			}
		}
	}
	//Shark stuff
	else if (grid->getCell(x, y, z)->getCellID() == 2)
	{
		bool die = false;
		if (outputGrid->getCell(x, y, z)->getCellState() > 19)	//Die of old age
			die = true;
		else
		{
			if (rnd.random(1, 32) == 1) //Chance to die
				die = true;
			else if (sharks > 5)
			{
				if (fish == 0)
					die = true;
			}
			else
				outputGrid->getCell(x, y, z)->setCellState(grid->getCell(x, y, z)->getCellState() + 1); //Live
		}
		if (die)
		{
			outputGrid->setCellID(x, y, z, 0);
			outputGrid->getCell(x, y, z)->setCellState(0);
		}
	}
	//Fish stuff
	else if (grid->getCell(x, y, z)->getCellID() == 1)
	{
		if (outputGrid->getCell(x, y, z)->getCellState() > 9)
		{
			outputGrid->setCellID(x, y, z, 0);
			outputGrid->getCell(x, y, z)->setCellState(0);
		}
		else
		{
			if (sharks > 4 || fish == 8) //Die
			{
				outputGrid->setCellID(x, y, z, 0);
				outputGrid->getCell(x, y, z)->setCellState(0);
			}
			else
			{
				//Live another generation
				outputGrid->getCell(x, y, z)->setCellState(grid->getCell(x, y, z)->getCellState() + 1);
			}
		}
	}
	delete[] tempNeighbors;
	return true;
}
int SharkFish::getNumberOfBreedingSharks(Cell **cells)
{
	int counter = 0;

	//Returning the number of neighbors that that have the ID we are looking for
	for (int i = 0; i < neighborBehaviour->getNoOfNeighbors(); i++)
		if (cells[i] != NULL)
			if (cells[i]->getCellID() == 2)
				if (cells[i]->getCellState() > 1)
					counter++;

	return counter;
}
int SharkFish::getNumberOfBreedingFish(Cell **cells)
{
	int counter = 0;

	//Returning the number of neighbors that that have the ID we are looking for
	for (int i = 0; i < neighborBehaviour->getNoOfNeighbors(); i++)
		if (cells[i] != NULL)
			if (cells[i]->getCellID() == 1)
				if (cells[i]->getCellState() > 2)
					counter++;

	return counter;

}
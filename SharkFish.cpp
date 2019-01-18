#include "SharkFish.h"


SharkFish::SharkFish()
{

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

	z--;
	if (z < 0)
		z = grid->getDepth() - 1;
	bool appliedRule = false;
	if (grid->getCell(x, y, z)->getCellID() == doEquasionIfThisId)
	{
		Cell **tempNeighbors = neighborBehaviour->getNeighbors(x, y, z, grid);




		//Shark reproduction
		if (neighborCounter(tempNeighbors, 1) > 3)
		{
			if (getNumberOfBreedingSharks(tempNeighbors) > 2)
			{
				if (neighborCounter(tempNeighbors, 2) < 4)
				{
					outputGrid->setCellID(x, y, z, 1); 
					outputGrid->getCell(x, y, z)->setCellState(1)
				}
			}
		}
		//Fish reproduction
		else if (neighborCounter(tempNeighbors, 2) > 3)
		{
			if (getNumberOfBreedingFish(tempNeighbors) > 2)
			{
				if (neighborCounter(tempNeighbors, 1) < 4)
				{
					outputGrid->setCellID(x, y, z, 1);
					outputGrid->getCell(x, y, z)->setCellState(1);
				}
			}
		}















		delete[] tempNeighbors;
	}
	return appliedRule;
}
int SharkFish::getNumberOfBreedingSharks(Cell **cells)
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
int SharkFish::getNumberOfBreedingFish(Cell **cells)
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
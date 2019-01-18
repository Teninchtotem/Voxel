

#include "OneDimentionalEquasion.h"


OneDimentionalEquasion::OneDimentionalEquasion(int equasionIDIn, Grid *gridIn, NeighborBehaviour *neighborBehaviourIn)
{
	equasionType = 1;
	equasionID = equasionIDIn;
	int tempEquasionID = equasionID;
	grid = gridIn;
	neighborBehaviour = neighborBehaviourIn;

	//Creating one dimentional rules
	for (int i = 7; i >= 0; i--)
	{
		rules[i] = false;
		if (tempEquasionID > pow(2,i)-1)
		{
			rules[i] = true;
			tempEquasionID -= pow(2, i);
		}
	}
}
bool OneDimentionalEquasion::applyRule(int x, int y, int z, Grid *outputGrid)
{
	bool appliedRule = false;
	Cell **tempNeighbors = neighborBehaviour->getNeighbors(x, y, z, grid);
	if (rules[neighborCounter(tempNeighbors, 0)])
		outputGrid->setCellID(x, y, z, 1);
	else
		outputGrid->setCellID(x, y, z, 0);

	delete[] tempNeighbors;

	return true;
}
int OneDimentionalEquasion::neighborCounter(Cell **cells, int ID)
{
	int counter = 0;
	for (int i = 0; i < neighborBehaviour->getNoOfNeighbors(); i++)
	{
		if (cells[i] != NULL)
		{
			if (cells[i]->getCellID() != 0)
			{
				switch (i)
				{
				case 0: counter += 4; break;
				case 1: counter += 2; break;
				case 2: counter += 1; break;
				}
			}
		}
	}
	return counter;
}

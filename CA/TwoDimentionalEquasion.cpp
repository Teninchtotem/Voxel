

#include "TwoDimentionalEquasion.h"


TwoDimentionalEquasion::TwoDimentionalEquasion()
{
	equasionType = 2;
}
bool TwoDimentionalEquasion::applyRule(int x, int y, int z, Grid *outputGrid)
{
	z--;
	if (z < 0)
		z = grid->getDepth() - 1;
	bool appliedRule = false;
	if (grid->getCell(x, y, z)->getCellID() == doEquasionIfThisId)
	{
		Cell **tempNeighbors = neighborBehaviour->getNeighbors(x, y, z, grid);
		if (equasionBehaviour->calculate(neighborCounter(tempNeighbors, IdToTest), numberToCheck))
		{
			if (z + 1 < grid->getDepth())
				outputGrid->setCellID(x, y, z + 1, idToBecomeIfSucessfull);
			else
				outputGrid->setCellID(x, y, 0, idToBecomeIfSucessfull);
			bool appliedRule = true;
		}
		delete[] tempNeighbors;
	}
	return appliedRule;
}

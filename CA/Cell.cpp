

#include "Cell.h"

Cell::Cell()
{
	cellID = 0;
	cellState = 0;
}
Cell::~Cell()
{

}


void Cell::setCellID(int cellIDIn)
{
	cellID = cellIDIn;
}
void Cell::setCellState(int cellStateIn)
{
	cellState = cellStateIn;
}


int Cell::getCellID()
{
	return cellID;
}
int Cell::getCellState()
{
	return cellState;
}


void Cell::getLocation(int &xOut, int &yOut, int &zOut)
{
	xOut = location[0];
	yOut = location[1];
	zOut = location[2];
}
void Cell::setLocation(int x, int y, int z)
{
	location[0] = x;
	location[1] = y;
	location[2] = z;
}

Cell* Cell::getCell()
{
	return this;
}
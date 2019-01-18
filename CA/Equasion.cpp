
#include <sstream>
#include "Equasion.h"


Equasion::Equasion(void)
{
	randID = 0;
	equasionType = 0;
	neighborBehaviourID = 0;
	equasionBehaviourID = 0;

	neighborBehaviour = NULL;
	equasionBehaviour = NULL;
}


Equasion::~Equasion(void)
{
}


void Equasion::setLoopingGrid(bool in)
{
	neighborBehaviour->setLooping(in);
}

void Equasion::setNewGrid(Grid *gridIn)
{
	grid = gridIn;
}

string Equasion::GetEquasionAsString(void)
{
	string returnVal;
	string temp;
	stringstream convert;

	convert << doEquasionIfThisId;
	convert >> temp;
	if (temp.size() == 1)
		returnVal += " ";
	returnVal += "  ";
	returnVal += temp;
	returnVal += "       ";
	if (temp.size() == 1)
		returnVal += " ";

	convert.clear();
	convert << IdToTest;
	convert >> temp;
	returnVal += temp;
	returnVal += "       ";
	if (temp.size() == 1)
		returnVal += " ";

	switch (equasionBehaviourID)
	{
	case 0:
		returnVal += "=";
		break;
	case 1:
		returnVal += ">";
		break;
	case 2:
		returnVal += "<";
		break;
	}
	returnVal += "      ";
	if (temp.size() == 1)
		returnVal += " ";

	convert.clear();
	convert << numberToCheck;
	convert >> temp;
	returnVal += temp;
	returnVal += "       ";
	if (temp.size() == 1)
		returnVal += " ";

	convert.clear();
	convert << idToBecomeIfSucessfull;
	convert >> temp;
	returnVal += temp;
	returnVal += "       ";
	if (temp.size() == 1)
		returnVal += " ";

	convert.clear();
	convert << equasionID;
	convert >> temp;
	returnVal += temp;

	return returnVal;
}

void Equasion::initialiseEquasion(int equasionIDIn, Grid *gridIn, NeighborBehaviour *neighborBehaviourIn,	EquasionBehaviour *equasionBehaviourIn,
								  int doEquasionIfThisIdIn,	int IdToTestIn, int numberToCheckIn, int idToBecome)
{
	equasionID = equasionIDIn;
	grid = gridIn;
	neighborBehaviour = neighborBehaviourIn;
	equasionBehaviour = equasionBehaviourIn;
	doEquasionIfThisId = doEquasionIfThisIdIn;
	idToBecomeIfSucessfull = idToBecome;
	IdToTest = IdToTestIn;
	numberToCheck = numberToCheckIn;
}

void Equasion::setNeighborBehaviour(NeighborBehaviour *in, neighbors nIn)
{
	neighborBehaviourID = int(nIn);
	if (neighborBehaviour != NULL)
		delete neighborBehaviour;
	neighborBehaviour = in;
}

int Equasion::getID()
{
	return equasionID;
}
bool Equasion::applyRule(int x, int y, int z, Grid *outputGrid)
{
	bool appliedRule = false;
	//Dont bother doing this rule unless the current cell is the right ID
	if (grid->getCell(x, y, z)->getCellID() == doEquasionIfThisId)
	{
		//Get all of your valid neightbors
		Cell **tempNeighbors = neighborBehaviour->getNeighbors(x, y, z, grid);

		//Compare the number of valid neighbors to the number that we want to compare to
		if (equasionBehaviour->calculate(neighborCounter(tempNeighbors, IdToTest), numberToCheck))
		{
			//Set the cell to the right ID if we succeeded! Look at this naming system, it practially explains itself!
			outputGrid->setCellID(x, y, z, idToBecomeIfSucessfull);
			bool appliedRule = true;
		}
		delete[] tempNeighbors;
	}
	return appliedRule;
}
int Equasion::neighborCounter(Cell **cells, int ID)
{
	int counter = 0;

	//Returning the number of neighbors that that have the ID we are looking for
	for(int i = 0; i < neighborBehaviour->getNoOfNeighbors(); i++)
		if(cells[i] != NULL)
			if(cells[i]->getCellID() == ID)
				counter++;

	return counter;
}

#pragma once
#include <iostream>
#include <string.h>
#include "GreaterThanBehaviour.h"
#include "LessThanBehaviour.h"
#include "EqualBehaviour.h"
#include "AdjacentNeighbors.h"
#include "SurroundingNeighbors.h"
#include "SurroundingNeighborsTriangle.h"
#include "SurroundingNeighborsHex.h"
#include "SurroundingNeighborsThreeD.h"
#include "OneDimentionalAdjacentNeighbors.h"
#include "Grid.h"
using namespace std;

class Equasion
{
protected:
	int equasionType;
	int cellIdAssosiation;
	EquasionBehaviour *equasionBehaviour;
	int neighborBehaviourID;
	int equasionBehaviourID;
	int doEquasionIfThisId;
	int idToBecomeIfSucessfull;
	Grid *grid;
	int equasionID;
	int numberToCheck;
	int randID;
	int IdToTest;

public:
	NeighborBehaviour *neighborBehaviour;

	Equasion(void);
	~Equasion(void);

	void setEquasionBehaviourID(int in) { equasionBehaviourID = in; };
	void setNeighborBehaviourID(int in) { neighborBehaviourID = in; };
	int getRandID() { return randID; };

	virtual void initRule(){};
	virtual string GetEquasionAsString(void);
	virtual void setNewGrid(Grid *gridIn);
	virtual int getID();
	virtual void setLoopingGrid(bool in);
	virtual void initialiseEquasion(int equasionIDIn, Grid *gridIn, NeighborBehaviour *neighborBehaviourIn, EquasionBehaviour *equasionBehaviourIn,
		int doEquasionIfThisIdIn, int IdToTestIn, int numberToCheckIn, int idToBecome);
	virtual bool applyRule(int x, int y, int z, Grid *outputGrid);
	virtual int neighborCounter(Cell **cells, int ID);
	virtual void setNeighborBehaviour(NeighborBehaviour *in, neighbors nIn);

	virtual void save(std::ostream& saveFile, const Equasion &equasion) const 
	{
		saveFile << equasion.equasionID << " ";
		saveFile << equasion.neighborBehaviourID << " ";
		saveFile << equasion.equasionBehaviourID << " ";
		saveFile << equasion.doEquasionIfThisId << " ";
		saveFile << equasion.numberToCheck << " ";
		saveFile << equasion.IdToTest << " ";
		saveFile << equasion.idToBecomeIfSucessfull << "\n";
	}
	friend std::ostream& operator<<(std::ostream& saveFile, const Equasion &equasion)
	{
		saveFile << equasion.equasionType << " ";
		equasion.save(saveFile, equasion);
		return saveFile;
	}
};


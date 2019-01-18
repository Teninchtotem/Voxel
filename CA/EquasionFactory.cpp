

#include "EquasionFactory.h"


EquasionFactory::EquasionFactory(void)
{

}


EquasionFactory::~EquasionFactory(void)
{

}


Equasion *EquasionFactory::createEquasion(int equasionIDIn, Grid *gridIn, neighbors neighborIn, symbols SymbolIn, int initialCellIdCondition,
	int IdToLookFor, int numberToCompare, int idToBecome, bool loopingGrid)
{
	Equasion *equasion = new Equasion;
	EquasionBehaviour *equasionbehavior;
	NeighborBehaviour *neighborBehaviour;

	equasion->setNeighborBehaviourID(neighborIn);
	equasion->setEquasionBehaviourID(SymbolIn);

	switch (neighborIn)
	{
	case Adjacent_Neighbors: neighborBehaviour = new AdjacentNeighbors; break;
	case Surrounding_Neighbors: neighborBehaviour = new SurroundingNeighbors; break;
	case One_Dimentional_Adjacent_Neighbors: neighborBehaviour = new OneDimentionalAdjacentNeighbors; break;
	case Surrounding_Neighbors_Three_D: neighborBehaviour = new SurroundingNeighborsThreeD; break;
	case Surrounding_Neighbors_Triangles: neighborBehaviour = new SurroundingNeighborsTriangle; break;
	case Surrounding_Neighbors_Hex: neighborBehaviour = new SurroundingNeighborsHex; break;
	}
	switch (SymbolIn)
	{
	case Equal_To: equasionbehavior = new EqualBehaviour; break;
	case Greater_Than: equasionbehavior = new GreaterThanBehaviour; break;
	case Less_Than: equasionbehavior = new LessThanBehaviour; break;
	}

	equasion->initialiseEquasion(equasionIDIn, gridIn, neighborBehaviour, equasionbehavior,
		initialCellIdCondition, IdToLookFor, numberToCompare, idToBecome);

	equasion->setLoopingGrid(loopingGrid);


	return equasion;
}
Equasion *EquasionFactory::createTwoStepEquasion(int equasionIDIn, Grid *gridIn, neighbors neighborIn, symbols SymbolIn, int initialCellIdCondition,
	int IdToLookFor, int numberToCompare, int idToBecome, bool loopingGrid)
{
	Equasion *equasion = new TwoDimentionalEquasion;
	EquasionBehaviour *equasionbehavior;
	NeighborBehaviour *neighborBehaviour;

	equasion->setNeighborBehaviourID(neighborIn);
	equasion->setEquasionBehaviourID(SymbolIn);

	switch (neighborIn)
	{
	case Adjacent_Neighbors: neighborBehaviour = new AdjacentNeighbors; break;
	case Surrounding_Neighbors: neighborBehaviour = new SurroundingNeighbors; break;
	case One_Dimentional_Adjacent_Neighbors: neighborBehaviour = new OneDimentionalAdjacentNeighbors; break;
	case Surrounding_Neighbors_Three_D: neighborBehaviour = new SurroundingNeighborsThreeD; break;
	}
	switch (SymbolIn)
	{
	case Equal_To: equasionbehavior = new EqualBehaviour; break;
	case Greater_Than: equasionbehavior = new GreaterThanBehaviour; break;
	case Less_Than: equasionbehavior = new LessThanBehaviour; break;
	}

	equasion->initialiseEquasion(equasionIDIn, gridIn, neighborBehaviour, equasionbehavior,
		initialCellIdCondition, IdToLookFor, numberToCompare, idToBecome);

	equasion->setLoopingGrid(loopingGrid);


	return equasion;
}
Equasion *EquasionFactory::createOneDimentionalEquasion(int equasionIDIn, Grid *gridIn, neighbors neighborIn, bool loopingGrid)
{
	NeighborBehaviour *neighborBehaviour;

	switch (neighborIn)
	{
	case One_Dimentional_Adjacent_Neighbors: neighborBehaviour = new OneDimentionalAdjacentNeighbors; break;
	default: One_Dimentional_Adjacent_Neighbors : neighborBehaviour = new OneDimentionalAdjacentNeighbors; break;
	}
	Equasion *equasion = new OneDimentionalEquasion(equasionIDIn, gridIn, neighborBehaviour);

	equasion->setNeighborBehaviourID(neighborIn);

	equasion->setLoopingGrid(loopingGrid);

	return equasion;
}
Equasion *EquasionFactory::createFishShark(Grid *gridIn)
{
	return new SharkFish(gridIn);
}
Equasion *EquasionFactory::createSegregation(Grid *gridIn)
{
	return new Segregation(gridIn);
}

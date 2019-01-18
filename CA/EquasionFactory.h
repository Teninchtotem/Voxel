#pragma once
#include "Equasion.h"
#include "OneDimentionalEquasion.h"
#include "TwoDimentionalEquasion.h"
#include "SharkFish.h"
#include "Segregation.h"

enum neighbors { Adjacent_Neighbors, Surrounding_Neighbors, One_Dimentional_Adjacent_Neighbors, 
	Surrounding_Neighbors_Three_D, Surrounding_Neighbors_Triangles, Surrounding_Neighbors_Hex};
enum symbols {Equal_To, Greater_Than, Less_Than};

class EquasionFactory
{
protected:

public:
	EquasionFactory(void);
	~EquasionFactory(void);
	Equasion *createEquasion(int equasionIDIn, Grid *gridIn, neighbors neighborIn, symbols symbolIn,
		int initialCellIdCondition, int IdToLookFor, int numberToCompare, int idToBecome, bool loopingGrid);
	Equasion *createTwoStepEquasion(int equasionIDIn, Grid *gridIn, neighbors neighborIn, symbols symbolIn,
		int initialCellIdCondition, int IdToLookFor, int numberToCompare, int idToBecome, bool loopingGrid);
	Equasion *createOneDimentionalEquasion(int equasionIDIn, Grid *gridIn, neighbors neighborIn, bool loopingGrid);
	Equasion *createFishShark(Grid *gridIn);
	Equasion *createSegregation(Grid *gridIn);
};


#pragma once
#include "Cell.h"
#include <Windows.h>
#include <iostream>
#include <vector>
using namespace std;

//Only 2 threads for now (PIEPIEPIE look into finding optimum number of threads? Maybe use TBB
#define NUMBER_OF_RULE_THREADS 2

class Equasion;
class EquasionFactory;
enum neighbors;
enum symbols;

class Grid
{
protected:
	HANDLE  hThreadArray[NUMBER_OF_RULE_THREADS];
	Cell ***gridSpace;
	EquasionFactory *equasionFactory;
	vector<Equasion*> rules;
	int height;
	int width;
	int depth;
	bool deleteRules;

public:
	Grid();
	~Grid();
	Grid(int x, int y, int z);
	void wipeGrid();

	void setCellID(int x, int y, int z, int IDin);

	void initRules();
	void cancelThreads();
	void setLooping(bool in);
	void toggleRuleDeletion(bool in);
	Cell *getCell(int x, int y, int z);
	int getWidth();
	int getHeight();
	int getDepth();
	vector<Equasion*> getRules();
	void setRules(vector<Equasion*> rulesIn);
	void addEquasion(int equasionIDIn, neighbors neighborIn, symbols SymbolIn, int initialCellIdCondition,
		int IdToLookFor, int numberToCompare, int idToBecome, bool looping);
	void addTwoStepEquasion(int equasionIDIn, neighbors neighborIn, symbols SymbolIn, int initialCellIdCondition,
		int IdToLookFor, int numberToCompare, int idToBecome, bool looping);
	void addOneDimentionalEquasion(int equasionIDIn, neighbors neighborIn, bool looping);
	bool removeEquasion(int equasionID);
	void removeAllEquasions();
	void applyRules(Grid *outputGrid);
	void applyRulesOneDimentional(Grid *outputGrid, int row);
	void applyRulesTwoDimentional(Grid *outputGrid, int row);
	void addSharkFish(Grid *gridIn);
	void addSegregation(Grid *gridIn);
	int getFirstFreeID();


	friend std::ostream& operator<<(std::ostream& saveFile, const Grid &grid)
	{
		saveFile << grid.width << " ";
		saveFile << grid.height << " ";
		saveFile << grid.depth << "\n";

		saveFile << grid.rules.size() << "\n";

		return saveFile;
	}
};
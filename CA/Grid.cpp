
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "Grid.h"
#include "EquasionFactory.h"
#include "../Log.h"

Grid::Grid()
{
	height = 50;
	width = 50;
	depth = 50;
	deleteRules = true;
}
Grid::~Grid()
{
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j)
			delete[] gridSpace[i][j];

		delete[] gridSpace[i];
	}
	delete[] gridSpace;

	delete equasionFactory;

	if (deleteRules)
		for (vector<Equasion *>::iterator i = rules.begin(); i != rules.end(); ++i)
			delete *i;
}
int Grid::getFirstFreeID()
{
	int id = 0;
	bool searching = true;

	while (searching)
	{
		searching = false;
		for (vector<Equasion *>::iterator i = rules.begin(); i != rules.end(); ++i)
		{
			if (id == (*i)->getID())
			{
				id++;
				searching = true;
			}
		}
	}
	return id;
}
Grid::Grid(int x, int y, int z)
{
	width = x;
	height = y;
	depth = z;
	equasionFactory = new EquasionFactory;

	gridSpace = new Cell**[x];
	for(int i = 0; i < x; i++)
	{
		gridSpace[i] = new Cell*[y];
		for(int j = 0; j < y; j++)
		{
			gridSpace[i][j] = new Cell[z]();
			for(int k = 0; k < z; k++)
			{
				gridSpace[i][j][k].setLocation(i,j,k);
			}
		}
	} 
}

void Grid::toggleRuleDeletion(bool in)
{
	deleteRules = in;
}

void Grid::wipeGrid()
{
	for(int i = 0; i < width; i++)
		for(int j = 0; j < height; j++)
		for (int k = 0; k < depth; k++)
		{
			gridSpace[i][j][k].setCellID(0);
			gridSpace[i][j][k].setCellState(0);
		}
}
void Grid::setCellID(int x, int y, int z, int IDin)
{
	gridSpace[x][y][z].setCellID(IDin);
}


Cell *Grid::getCell(int x, int y, int z)
{
	return gridSpace[x][y][z].getCell();
}
int Grid::getWidth()
{
	return width;
}
int Grid::getHeight()
{
	return height;
}
int Grid::getDepth()
{
	return depth;
}
void Grid::setLooping(bool in)
{
	for (vector<Equasion *>::iterator i = rules.begin(); i != rules.end(); i++)
	{
		(*i)->setLoopingGrid(in);
	}
}
void Grid::addSharkFish(Grid *gridIn)
{
	rules.push_back(equasionFactory->createFishShark(gridIn));
}
void Grid::initRules()
{
	for (vector<Equasion *>::iterator i = rules.begin(); i != rules.end(); i++)
		(*i)->initRule();
}
void Grid::addSegregation(Grid *gridIn)
{
	rules.push_back(equasionFactory->createSegregation(gridIn));
}
void Grid::addEquasion(int equasionIDIn, neighbors neighborIn, symbols SymbolIn, int initialCellIdCondition,
	int IdToLookFor, int numberToCompare, int idToBecome, bool looping)
{
	rules.push_back(equasionFactory->createEquasion(equasionIDIn, this, neighborIn, SymbolIn, initialCellIdCondition, IdToLookFor, numberToCompare, idToBecome, looping));
}
void Grid::addTwoStepEquasion(int equasionIDIn, neighbors neighborIn, symbols SymbolIn, int initialCellIdCondition,
	int IdToLookFor, int numberToCompare, int idToBecome, bool looping)
{
	rules.push_back(equasionFactory->createTwoStepEquasion(equasionIDIn, this, neighborIn, SymbolIn, initialCellIdCondition, IdToLookFor, numberToCompare, idToBecome, looping));
}
void Grid::addOneDimentionalEquasion(int equasionIDIn, neighbors neighborIn, bool looping)
{
	rules.push_back(equasionFactory->createOneDimentionalEquasion(equasionIDIn, this, neighborIn, looping));
}
void Grid::removeAllEquasions()
{
	for (vector<Equasion *>::iterator i = rules.begin(); i != rules.end(); ++i)
		delete *i;
	rules.clear();
}
bool Grid::removeEquasion(int equasionID)
{
	//Deleting an equasion by it's ID
	for (vector<Equasion *>::iterator i = rules.begin(); i != rules.end(); i++)
	{
		int temp = (*i)->getID();

		if ((*i)->getID() == equasionID)
		{
			delete *i;
			rules.erase(i);
			return true;
		}
	}
	return false;
}
struct gridThreadData
{
	vector<Equasion*> rules;
	Grid *gridOut;
	int width;
	int height;
	int depth;
	int startX;
	int endX;
	int startY;
	int endY;
	int startZ;
	int endZ;
};
//Thread function to split the workload like a coconut and i dont even like coconut
DWORD WINAPI applyRulesToLines(void *dataIn)
{
	//And as death pointed to the void he said "whoops, wait i meen no...over heere", and so he pointed to the thread data instead
	gridThreadData *data = (gridThreadData *)dataIn;

	//Doing workload between spesified start and end points
	bool done = false;
	for (int i = data->startX; i < data->endX && i < data->width; i++)
	for (int j = data->startY; j < data->endY && j < data->height; j++)
	for (int k = data->startZ; k < data->endZ && k < data->depth; k++)
	{
		done = false;
		for (int p = 0; p < data->rules.size(); p++)
		{
 			done = data->rules[p]->applyRule(i, j, k, data->gridOut);
			if (done == true)
				p = data->rules.size();
		}
	}
	return 0;
}
void Grid::applyRules(Grid *outputGrid)
{
	gridThreadData data[NUMBER_OF_RULE_THREADS];
	int linesPerThread = width / NUMBER_OF_RULE_THREADS;
	if (NUMBER_OF_RULE_THREADS * linesPerThread < width)
		linesPerThread++;

	//Setting info for each thread and creating it
	for (int i = 0; i < NUMBER_OF_RULE_THREADS; i++)
	{
		data[i].rules = rules;
		data[i].gridOut = outputGrid;
		data[i].width = width;
		data[i].height = height;
		data[i].depth = depth;
		data[i].startX = linesPerThread*i;
		data[i].endX = linesPerThread*(i+1);
		data[i].startY = 0;
		data[i].endY = height;
		data[i].startZ = 0;
		data[i].endZ = depth;
		hThreadArray[i] = CreateThread(NULL, 0, applyRulesToLines, (void*)&data[i], 0, NULL);
	}
	//	*twiddle thumbs*
	WaitForMultipleObjects(NUMBER_OF_RULE_THREADS, hThreadArray, TRUE, INFINITE);
}

void Grid::cancelThreads()
{
	for (int i = 0; i < NUMBER_OF_RULE_THREADS; i++)
	{
		TerminateThread(hThreadArray[i], NULL);
	}
}

void Grid::applyRulesOneDimentional(Grid *outputGrid, int row)
{
	bool done = false;
	for (int i = 0; i < width; i++)
	{
		done = false;
		for (int p = 0; p < rules.size(); p++)
		{
			done = rules[p]->applyRule(i, row, 0, outputGrid);
			if (done == true)
				p = rules.size();
		}
	}
}
struct threadDataTwoStep
{
	vector<Equasion*> rules;
	Grid *gridOut;
	int width;
	int height;
	int depth;
	int startX;
	int endX;
	int startY;
	int endY;
	int row;
};
DWORD WINAPI applyTwoStepRulesToLines(void *dataIn)
{
	threadDataTwoStep *data = (threadDataTwoStep *)dataIn;

	bool done = false;
	for (int i = data->startX; i < data->endX && i < data->width; i++)
	for (int j = data->startY; j < data->endY && j < data->height; j++)
	{
		done = false;
		for (int p = 0; p < data->rules.size(); p++)
		{
			done = data->rules[p]->applyRule(i, j, data->row, data->gridOut);
			if (done == true)
				p = data->rules.size();
		}
	}
	return 0;
}
void Grid::applyRulesTwoDimentional(Grid *outputGrid, int row)
{
	//Creating thread related info
	const int split = 2;
	HANDLE  hThreadArray[split];
	threadDataTwoStep data[split];

	int linesPerThread = width / split;
	if (split*linesPerThread < width)
		linesPerThread++;

	//Creating data to send to thread
	for (int i = 0; i < split; i++)
	{
		data[i].rules = rules;
		data[i].gridOut = outputGrid;
		data[i].width = width;
		data[i].height = height;
		data[i].depth = depth;
		data[i].startX = linesPerThread*i;
		data[i].endX = linesPerThread*(i + 1);
		data[i].startY = 0;
		data[i].endY = height;
		data[i].row = row;
		hThreadArray[i] = CreateThread(NULL, 0, applyTwoStepRulesToLines, (void*)&data[i], 0, NULL);
	}
	WaitForMultipleObjects(split, hThreadArray, TRUE, INFINITE);
}

vector<Equasion*> Grid::getRules()
{
	return rules;
}
void Grid::setRules(vector<Equasion*> rulesIn)
{
	for (vector<Equasion *>::iterator i = rules.begin(); i != rules.end(); ++i)
		delete *i;

	rules = rulesIn;

	for (vector<Equasion *>::iterator i = rules.begin(); i != rules.end(); ++i)
		(*i)->setNewGrid(this);
}
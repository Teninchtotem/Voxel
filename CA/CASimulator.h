#pragma once
#include "Display.h"
#include "EquasionFactory.h"
#include "../FileManager.h"
#include "RandomNumber.h"
#include <mutex>
namespace Win
{
	class ControllerFormGL;
}

class CASimulator
{
protected:
	Grid *CA;
	Grid *CANextStep;
	Grid *CADrawSafe;
	bool advanced;
	RandomNumber rnd;
	Display display;
	int CAType;
	int rowToCalculate;
	mutex *drawMutex;
	bool syncStepDraw;
	int stepCounter;
	int twoDimentionalTessellation;
	int squareNeighborType;
	int totalSteps;
	int stepsToDo;
	bool inStep;
	bool stepsToDoBool;
	bool looping;
	bool staticCatch;
	bool staticCatchChecking;
	void copyGridOver();
	bool loopingGrid;
	Win::ControllerFormGL *controllerForm;

public:
	FileManager fm;

	CASimulator();
	~CASimulator();

	void open(string fileName = "");
	void save(string fileName = "");
	void setAdvanced(bool in);
	bool getAdvanced();
	int getSteps();
	void toggleLooping();
	void wipeGrid();
	void setCAType(int typeIn);
	int getCAType();
	bool getInStep(); 
	bool getLooping();
	void setLooping(bool in);
	void init();
	void setDrawMutex(mutex *in);
	string convertToString(int in);
	void updateViewController();
	void initViewController(Win::ControllerFormGL* in);
	void oneDimentionalstep();
	void twoDimentionalstep();
	void checkForStatic();
	void checkForStaticTwoStep(int rowIn);
	void toggleSyncDisplay();
	void setStepsToDo(int stepsIn);
	int getStepsToDo();
	void setGridSize(int widthIn, int heightIn, int depthIn);
	void step();
	void displayGrid();
	void mainLoop();
	int getTwoDimentionalTessellation();
	int getSquareNeighborType();
	void setSquareNeighborType(int in);
	void setTwoDimentionalTessellation(int in);
	void setCellID(int x, int y, int z, int ID);
	void simpleRandomise(int percentChance);
	void simpleRuleToggle(int ruleNoIn);
	void simpleRandomise(int *IDs, int numberOfIDs, int percentChance);
	void arrayRandomise(int numberOfIDs, int *IDs, int *Chances);
	Grid *getGrid();
	Grid *getRuleGrid();
	Grid *getSecondaryGrid();


	mutex *getMutex(){ return drawMutex; };
	friend std::ostream& operator<<(std::ostream& saveFile, const CASimulator &simulator)
	{
		saveFile << simulator.CAType << " ";
		if (simulator.CAType == 1)
			saveFile << simulator.twoDimentionalTessellation << " ";
		saveFile << simulator.looping << "\n";
		return saveFile;
	}
};
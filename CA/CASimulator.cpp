
#include "../ControllerFormGL.h"
#include "CASimulator.h"
#include "../Log.h"

CASimulator::CASimulator()
{
	advanced = false;
	controllerForm = NULL;
	syncStepDraw = true;
	inStep = false;
	totalSteps = 0;
	stepsToDo = 0;
	squareNeighborType = 0;
	stepsToDoBool = false;
	staticCatch = false;
	staticCatchChecking = true;
	rowToCalculate = 1;
	twoDimentionalTessellation = 0;
	stepCounter = 0;
	looping = false;
	CAType = 1;
	CA = new Grid(1, 1, 1);
	CADrawSafe = CA;
	CANextStep = new Grid(1, 1, 1);
}
void CASimulator::init()
{
	if (!fm.loadSimulatorStateFromName("Welcome", *this))
	{
		//Setting defaults
		int gridWidth = 100;
		int gridHeight = 100;
		CA = new Grid(gridWidth, gridHeight, 1);
		CADrawSafe = CA;
		CANextStep = new Grid(gridWidth, gridHeight, 1);

		//Creating Game of life as a default (rules could be simplified but would not allow for checkbox toggling)
		CA->addEquasion(-1, Surrounding_Neighbors, Equal_To, 1, 1, 0, 0, looping);
		CA->addEquasion(-2, Surrounding_Neighbors, Equal_To, 1, 1, 1, 0, looping);
		CA->addEquasion(-5, Surrounding_Neighbors, Equal_To, 1, 1, 4, 0, looping);
		CA->addEquasion(-6, Surrounding_Neighbors, Equal_To, 1, 1, 5, 0, looping);
		CA->addEquasion(-7, Surrounding_Neighbors, Equal_To, 1, 1, 6, 0, looping);
		CA->addEquasion(-8, Surrounding_Neighbors, Equal_To, 1, 1, 7, 0, looping);
		CA->addEquasion(-9, Surrounding_Neighbors, Equal_To, 1, 1, 8, 0, looping);
		CA->addEquasion(-14, Surrounding_Neighbors, Equal_To, 0, 1, 3, 1, looping);
		simpleRandomise(40);
	}
	if (controllerForm != NULL)
		controllerForm->calculateUIfromSimulator();
}
CASimulator::~CASimulator()
{
	inStep = false;
	delete CA;
	delete CANextStep;
}
void CASimulator::setDrawMutex(mutex *in)
{
	drawMutex = in;
}
void CASimulator::checkForStatic()
{
	staticCatch = true;

	//Loop through grid to check if the two grids are the same after a step
	for (int i = 0; i < CA->getWidth(); i++)
	for (int j = 0; j < CA->getHeight(); j++)
	for (int k = 0; k < CA->getDepth(); k++)
	if (CA->getCell(i, j, k)->getCellID() != CANextStep->getCell(i, j, k)->getCellID())
	{
		staticCatch = false;
		i = CA->getWidth();
		j = CA->getHeight();
		k = CA->getDepth();
	}
}
void CASimulator::checkForStaticTwoStep(int rowIn)
{
	staticCatch = true;
	//Loop through the grid and check if the current row is the same as the previous row
	for (int i = 0; i < CA->getWidth(); i++)
	{
		for (int j = 0; j < CA->getHeight(); j++)
		{
			if (rowIn < 1)
				rowIn = CA->getDepth();
			if (CA->getCell(i, j, rowIn)->getCellID() != CA->getCell(i, j, rowIn - 1)->getCellID())
			{
				staticCatch = false;
				i = CA->getWidth();
				j = CA->getHeight();
			}
		}
	}
}
bool CASimulator::getLooping()
{
	return looping;
}

void CASimulator::setCAType(int typeIn)
{
	if (typeIn >= 0 && typeIn <= 4)
	{
		totalSteps = 0;
		//Setting the grid to a depth of 1 if needed (becoming a 2D grid)
		if (CAType != typeIn)
		{
			if (typeIn < 2 && CA->getDepth() > 1)
				setGridSize(CA->getWidth(), CA->getHeight(), 1);
		}
		CAType = typeIn;
	}	
}
void CASimulator::setStepsToDo(int stepsIn)
{
	stepsToDo = stepsIn;
	stepsToDoBool = true;
}
int CASimulator::getStepsToDo()
{
	return stepsToDo;
}
void CASimulator::initViewController(Win::ControllerFormGL *in)
{
	controllerForm = in;
}
void CASimulator::updateViewController()
{
	//Sending a request to the controller to tell it its info is out of date and to get the new stuff
	if (controllerForm!= NULL)
		controllerForm->setInfo();
}
void CASimulator::toggleSyncDisplay()
{
	syncStepDraw = !syncStepDraw;
}
int CASimulator::getCAType()
{
	return CAType;
}

Grid *CASimulator::getRuleGrid()
{
	return CA;
}
Grid *CASimulator::getSecondaryGrid()
{
	return CANextStep;
}

void CASimulator::toggleLooping()
{
	looping = !looping;
	CA->setLooping(looping);
}
void CASimulator::setLooping(bool in)
{
	looping = in;
	CA->setLooping(in);
}
void CASimulator::setCellID(int x, int y, int z, int ID)
{
	CA->setCellID(x, y, z, ID);
	CANextStep->setCellID(x, y, z, ID);
}
string CASimulator::convertToString(int in)
{
	stringstream convert;
	string returnString;
	convert << in;
	convert >> returnString;
	return returnString;
}
void CASimulator::setGridSize(int widthIn, int heightIn, int depthIn)
{
	(*drawMutex).lock();

	stepCounter = 0;
	totalSteps = 0;

	//Updating Controller with new info
	updateViewController();

	//Saving the current rules and making sure that they dont get deleted
	CA->toggleRuleDeletion(false);
	vector<Equasion*> tempRules = CA->getRules();

	//Win::log("Creating New Grids");
	CADrawSafe = CANextStep;
	delete CA;
	CA = new Grid(widthIn, heightIn, depthIn);

	CADrawSafe = CA;
	delete CANextStep;
	CANextStep = new Grid(widthIn, heightIn, depthIn);

	CADrawSafe = CANextStep;

	//Loading rules back into the grid
	CA->setRules(tempRules);

	string gridInfo = "Created new grid " + convertToString(widthIn) + " x " +
		convertToString(heightIn) + " x " + convertToString(depthIn);
	Win::log(gridInfo.c_str());

	(*drawMutex).unlock();
}
int CASimulator::getSquareNeighborType()
{
	return squareNeighborType;
}
void CASimulator::setSquareNeighborType(int in)
{
	if (squareNeighborType != in)
	{
		squareNeighborType = in;
		for (int i = 0; i < CA->getRules().size(); i++)
		{
			neighbors id;
			NeighborBehaviour *temp;
			switch (in)
			{
			case 0:		
				id = Surrounding_Neighbors;
				temp = new SurroundingNeighbors;	
				break;
			case 1:
				id = Adjacent_Neighbors;
				temp = new AdjacentNeighbors;	
				break;
			default:
				id = Surrounding_Neighbors;
				temp = new SurroundingNeighbors;	
				break;
			}
			CA->getRules()[i]->setNeighborBehaviour(temp, id);
		}
	}
}
void CASimulator::simpleRuleToggle(int ruleNoIn)
{
	//Will delete the equasion if it exists which will then skip the creation
	if (!CA->removeEquasion(ruleNoIn))
	{
		//Allows the simple 0-8 rules to toggle based on unique ID's -1 to -19
		if (CAType == 1)
		{
			neighbors neighborType = Surrounding_Neighbors;
			switch (twoDimentionalTessellation)
			{
			case 0: 
				switch (squareNeighborType)
				{
				case 0:		neighborType = Surrounding_Neighbors;	break;
				case 1:		neighborType = Adjacent_Neighbors;	break;
				default:	neighborType = Surrounding_Neighbors;	break;
				}
				break;
			case 1: neighborType = Surrounding_Neighbors_Triangles; break;
			case 2: neighborType = Surrounding_Neighbors_Hex; break;
			default: neighborType = Surrounding_Neighbors; break;
			}
			switch (ruleNoIn)
			{
			case -1:	CA->addEquasion(-1, neighborType, Equal_To, 1, 1, 0, 0, looping);	break;
			case -2:	CA->addEquasion(-2, neighborType, Equal_To, 1, 1, 1, 0, looping);	break;
			case -3:	CA->addEquasion(-3, neighborType, Equal_To, 1, 1, 2, 0, looping);	break;
			case -4:	CA->addEquasion(-4, neighborType, Equal_To, 1, 1, 3, 0, looping);	break;
			case -5:	CA->addEquasion(-5, neighborType, Equal_To, 1, 1, 4, 0, looping);	break;
			case -6:	CA->addEquasion(-6, neighborType, Equal_To, 1, 1, 5, 0, looping);	break;
			case -7:	CA->addEquasion(-7, neighborType, Equal_To, 1, 1, 6, 0, looping);	break;
			case -8:	CA->addEquasion(-8, neighborType, Equal_To, 1, 1, 7, 0, looping);	break;
			case -9:	CA->addEquasion(-9, neighborType, Equal_To, 1, 1, 8, 0, looping);	break;
			case -11:	CA->addEquasion(-11, neighborType, Equal_To, 0, 1, 0, 1, looping);	break;
			case -12:	CA->addEquasion(-12, neighborType, Equal_To, 0, 1, 1, 1, looping);	break;
			case -13:	CA->addEquasion(-13, neighborType, Equal_To, 0, 1, 2, 1, looping);	break;
			case -14:	CA->addEquasion(-14, neighborType, Equal_To, 0, 1, 3, 1, looping);	break;
			case -15:	CA->addEquasion(-15, neighborType, Equal_To, 0, 1, 4, 1, looping);	break;
			case -16:	CA->addEquasion(-16, neighborType, Equal_To, 0, 1, 5, 1, looping);	break;
			case -17:	CA->addEquasion(-17, neighborType, Equal_To, 0, 1, 6, 1, looping);	break;
			case -18:	CA->addEquasion(-18, neighborType, Equal_To, 0, 1, 7, 1, looping);	break;
			case -19:	CA->addEquasion(-19, neighborType, Equal_To, 0, 1, 8, 1, looping);	break;
			}
		}
		//Allows the simple 0-8 rules of 2D step CA to toggle based on unique ID's -1 to -19 
		else if (CAType == 2)
		{
			switch (ruleNoIn)
			{
			case -1:	CA->addTwoStepEquasion(-1, Surrounding_Neighbors, Equal_To, 1, 1, 0, 0, looping);	break;
			case -2:	CA->addTwoStepEquasion(-2, Surrounding_Neighbors, Equal_To, 1, 1, 1, 0, looping);	break;
			case -3:	CA->addTwoStepEquasion(-3, Surrounding_Neighbors, Equal_To, 1, 1, 2, 0, looping);	break;
			case -4:	CA->addTwoStepEquasion(-4, Surrounding_Neighbors, Equal_To, 1, 1, 3, 0, looping);	break;
			case -5:	CA->addTwoStepEquasion(-5, Surrounding_Neighbors, Equal_To, 1, 1, 4, 0, looping);	break;
			case -6:	CA->addTwoStepEquasion(-6, Surrounding_Neighbors, Equal_To, 1, 1, 5, 0, looping);	break;
			case -7:	CA->addTwoStepEquasion(-7, Surrounding_Neighbors, Equal_To, 1, 1, 6, 0, looping);	break;
			case -8:	CA->addTwoStepEquasion(-8, Surrounding_Neighbors, Equal_To, 1, 1, 7, 0, looping);	break;
			case -9:	CA->addTwoStepEquasion(-9, Surrounding_Neighbors, Equal_To, 1, 1, 8, 0, looping);	break;
			case -11:	CA->addTwoStepEquasion(-11, Surrounding_Neighbors, Equal_To, 0, 1, 0, 1, looping);	break;
			case -12:	CA->addTwoStepEquasion(-12, Surrounding_Neighbors, Equal_To, 0, 1, 1, 1, looping);	break;
			case -13:	CA->addTwoStepEquasion(-13, Surrounding_Neighbors, Equal_To, 0, 1, 2, 1, looping);	break;
			case -14:	CA->addTwoStepEquasion(-14, Surrounding_Neighbors, Equal_To, 0, 1, 3, 1, looping);	break;
			case -15:	CA->addTwoStepEquasion(-15, Surrounding_Neighbors, Equal_To, 0, 1, 4, 1, looping);	break;
			case -16:	CA->addTwoStepEquasion(-16, Surrounding_Neighbors, Equal_To, 0, 1, 5, 1, looping);	break;
			case -17:	CA->addTwoStepEquasion(-17, Surrounding_Neighbors, Equal_To, 0, 1, 6, 1, looping);	break;
			case -18:	CA->addTwoStepEquasion(-18, Surrounding_Neighbors, Equal_To, 0, 1, 7, 1, looping);	break;
			case -19:	CA->addTwoStepEquasion(-19, Surrounding_Neighbors, Equal_To, 0, 1, 8, 1, looping);	break;
			}
		}
	}
}
void CASimulator::copyGridOver()
{
	for (int i = 0; i < CA->getWidth(); i++)
	for (int j = 0; j < CA->getHeight(); j++)
	for (int k = 0; k < CA->getDepth(); k++)
	{
		CANextStep->setCellID(i, j, k, CA->getCell(i, j, k)->getCellID());
		CANextStep->getCell(i, j, k)->setCellState(CA->getCell(i, j, k)->getCellState());
	}
}

bool CASimulator::getInStep()
{
	return inStep;
}

void CASimulator::step()
{
	//Apply the rules
	CA->applyRules(CANextStep);
	CADrawSafe = CANextStep;

	//Check for a static condition
	if (staticCatchChecking)
		checkForStatic();

	//Copy grid back over for next step
	for (int i = 0; i < CA->getWidth(); i++)
	for (int j = 0; j < CA->getHeight(); j++)
	for (int k = 0; k < CA->getDepth(); k++)
	{
		CA->setCellID(i, j, k, CANextStep->getCell(i, j, k)->getCellID());
		CA->getCell(i, j, k)->setCellState(CANextStep->getCell(i, j, k)->getCellState());
	}
	CADrawSafe = CA;
}
void CASimulator::oneDimentionalstep()
{
	staticCatch = false;

	//Apply the rules
	CA->applyRulesOneDimentional(CANextStep, rowToCalculate);

	CADrawSafe = CANextStep;
	for (int i = 0; i < CA->getWidth(); i++)
	{
		//Copying grid back over
		CA->setCellID(i, rowToCalculate, 0, CANextStep->getCell(i, rowToCalculate, 0)->getCellID());

		//Give a buffer of empty lines ahead of the current row to the for loop
		int tempRowToCalculate = rowToCalculate;
		int buffer = 0;
		if (CA->getHeight() > 20)
			buffer = 2;
		else if (CA->getHeight() > 10)
			buffer = 1;
		for (int j = 0; j < buffer; j++)
		{
			if (tempRowToCalculate + 1 >= CA->getHeight())
				tempRowToCalculate = 0;
			else
				tempRowToCalculate++;
			CA->setCellID(i, tempRowToCalculate, 0, 0);
		}
	}
	CADrawSafe = CA;

	rowToCalculate++;
	if (rowToCalculate == CA->getHeight())
		rowToCalculate = 0;
}
void CASimulator::twoDimentionalstep()
{
	if (CA->getDepth() > 1)
	{
		//Depth dimention is all over the place in the contained functions because of the different
		//depths (z) that need to be sampled, copied and tested - Just trust me it's in the right place!
		int tempZ = rowToCalculate - 1;
		if (tempZ < 0)
			tempZ = CA->getDepth() - 1;

		//Preparing the next row
		for (int i = 0; i < CA->getWidth(); i++)
		{
			for (int j = 0; j < CA->getHeight(); j++)
			{
				CANextStep->setCellID(i, j, rowToCalculate, CA->getCell(i, j, tempZ)->getCellID());
			}
		}
		//Applying rules
		CA->applyRulesTwoDimentional(CANextStep, rowToCalculate);

		CADrawSafe = CANextStep;
		for (int i = 0; i < CA->getWidth(); i++)
		{
			for (int j = 0; j < CA->getHeight(); j++)
			{
				//Copying new row back over
				CA->setCellID(i, j, rowToCalculate, CANextStep->getCell(i, j, rowToCalculate)->getCellID());

				//Give a buffer of empty lines ahead of the current row equal to the for loop
				int tempRowToCalculate = rowToCalculate;
				int buffer = 0;
				if (CA->getDepth() > 20)
					buffer = 2;
				else if (CA->getDepth() > 10)
					buffer = 1;
				for (int k = 0; k < buffer; k++)
				{
					if (tempRowToCalculate + 1 >= CA->getDepth())
						tempRowToCalculate = 0;
					else
						tempRowToCalculate++;
					CA->setCellID(i, j, tempRowToCalculate, 0);
				}
			}
		}

		CADrawSafe = CA;

		//Checking for static condition
		checkForStaticTwoStep(rowToCalculate);

		rowToCalculate++;
		if (rowToCalculate == CA->getDepth())
			rowToCalculate = 0;
	}
	else if (controllerForm != NULL)
		staticCatch = true;
}

int CASimulator::getSteps()
{
	return totalSteps;
}

Grid *CASimulator::getGrid()
{
	return CADrawSafe;
}
void CASimulator::displayGrid()
{
	display.displayToScreen(CA);
}
void CASimulator::mainLoop()
{
	bool stoneSetSync = syncStepDraw;		//Stops it trying to unlock if this is toggled mid-step
	bool pauseSimulation = false;

	if (stoneSetSync)
		(*drawMutex).lock();

	inStep = true;
	bool mainContinue = true;
	//Working out if there are a set number of steps to perform and pausing if needed 
	if (stepsToDoBool)
	{
		if (stepsToDo > 0)
		{
			stepsToDo--;
		}
		else
		{
			stepsToDoBool = false;
			mainContinue = false;
			pauseSimulation = true;
		}
	}
	//Does something different depending on the current type of CA
	if (mainContinue)
	{
		CA->initRules();
		totalSteps++;
		switch (CAType)
		{
		case 0: oneDimentionalstep(); break;
		case 1: step(); break;
		case 2: twoDimentionalstep(); break;
		case 3: step(); break;
		}
	}
	inStep = false;
	if (stoneSetSync)
		(*drawMutex).unlock();

	if (pauseSimulation || staticCatch)
	if (controllerForm != NULL)
		controllerForm->pauseSimualtion();



	updateViewController();
}
void CASimulator::simpleRandomise(int percentChance)
{
	int height = CA->getHeight();
	int depth = CA->getDepth();
	//Limiting where to ramdomise on the step based CA's
	if (CAType == 0)
	{
		rowToCalculate = 1;
		wipeGrid();
		height = 1;
		depth = 1;
	}
	if (CAType == 2)
	{
		rowToCalculate = 1;
		wipeGrid();
		depth = 1;
	}
	//Randomising the array as either alive or dead based on the percentChance parameter
	for (int i = 0; i < CA->getWidth(); i++)
	for (int j = 0; j < height; j++)
	for (int k = 0; k < depth; k++)
	if (rnd.random(1, 100) <= percentChance)
	{
		CA->setCellID(i, j, k, 1);
		CANextStep->setCellID(i, j, k, 1);
	}
	else
	{
		CA->setCellID(i, j, k, 0);
		CANextStep->setCellID(i, j, k, 0);
	}
}

void CASimulator::wipeGrid()
{
	totalSteps = 0;
	rowToCalculate = 1;
	CA->wipeGrid();
	CANextStep->wipeGrid();
}
void CASimulator::simpleRandomise(int *IDs, int numberOfIDs, int percentChance)
{
	//randomly places one of the possible candidates onto the grid
	int counter1 = 0;
	int counter2 = 0;
	int counter3 = 0;
	int cellChance = 100 / numberOfIDs;
	for (int i = 0; i < CA->getWidth(); i++)
	{
		for (int j = 0; j < CA->getHeight(); j++)
		for (int k = 0; k < CA->getDepth(); k++)
		if (rnd.random(1, 100) <= percentChance)
		{
			int IDcounter = 0;
			int startingCount = rnd.random(1, 99);
			while (startingCount > cellChance)
			{
				startingCount -= cellChance;
				IDcounter++;
			}
			CA->setCellID(i, j, k, IDs[IDcounter]);
			CANextStep->setCellID(i, j, k, IDs[IDcounter]);
		}
	}
}
void CASimulator::arrayRandomise(int numberOfIDs, int *IDs, int *Chances)
{
	for (int i = 0; i < CA->getWidth(); i++)
	{
		for (int j = 0; j < CA->getHeight(); j++)
		{
			for (int k = 0; k < CA->getDepth(); k++)
			{
				int chance = rnd.random(1, 100);
				bool success = false;
				for (int p = 0; p < numberOfIDs; p++)
				{
					if (chance <= Chances[p])
					{
						success = true;
						CA->setCellID(i, j, k, IDs[p]);
						CANextStep->setCellID(i, j, k, IDs[p]);
						p = numberOfIDs;
					}
					else
					{
						chance -= Chances[p];
					}
				}
				if (!success)
				{
					CA->setCellID(i, j, k, 0);
					CANextStep->setCellID(i, j, k, 0);
				}
				CA->getCell(i, j, k)->setCellState(0);
				CANextStep->getCell(i, j, k)->setCellState(0);
			}

		}
		
	}
}
int CASimulator::getTwoDimentionalTessellation()
{
	return twoDimentionalTessellation;
}
void CASimulator::setTwoDimentionalTessellation(int in)
{
	twoDimentionalTessellation = in;
}
void CASimulator::setAdvanced(bool in)
{
	advanced = in;
}
void CASimulator::open(string fileName)
{
	controllerForm->prepareForFileDialog();
	
	if (fileName == "")
		fm.loadSimulatorStateFromName(fm.getNameOfLoadFile(), *this);
	else
		fm.loadSimulatorStateFromName(fileName, *this);

	controllerForm->recoverFromFileDialog();
}
void CASimulator::save(string fileName)
{
	bool doSave = true;
	if (CA->getRules().size() > 0)
	{
		if (CA->getRules()[0]->getRandID() > 0)
		{
			doSave = false;
		}
	}
	if (doSave)
	{
		controllerForm->prepareForFileDialog();

		if (fileName == "")
			fm.saveSimulatorStateFromName(fm.getNameOfSaveFile(), *this);
		else
			fm.saveSimulatorStateFromName(fileName, *this);

		controllerForm->recoverFromFileDialog();
	}
}
bool CASimulator::getAdvanced()
{
	return advanced;
}
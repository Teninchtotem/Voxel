#include "Segregation.h"


Segregation::Segregation(Grid *gridIn)
{
	randID = 2;
	grid = gridIn;
	neighborBehaviour = new SurroundingNeighbors();
	equasionType = -99;
}

Segregation::~Segregation()
{
}

bool Segregation::applyRule(int x, int y, int z, Grid *outputGrid)
{
	bool white = false;
	bool black = false;

	if (grid->getCell(x, y, z)->getCellID() == 1)
		white = true;
	else if (grid->getCell(x, y, z)->getCellID() == 2)
		black = true;

	if (black || white)
	{
		Cell **tempNeighbors;
		tempNeighbors = neighborBehaviour->getNeighbors(x, y, z, grid);
		int whites = neighborCounter(tempNeighbors, 1);
		int blacks = neighborCounter(tempNeighbors, 2);

		bool wantToMove = false;
		bool stay = true;

		//Maybe >?
		if (black)
		{
			if (whites > 2 * blacks)
				wantToMove = true;
		}
		else
		{
			if (blacks > 2 * whites)
				wantToMove = true;
		}

		if (wantToMove)
		{
			float bestEucl = grid->getWidth() + grid->getHeight();
			bool newLocFound = false;
			vector<Coord>::iterator moveLoc;
			vector<Coord> *toCheck;
			if (black)
			{
				vectorBlackAccess.lock();
				toCheck = &blackSpaces;
			}
			else
			{
				vectorWhiteAccess.lock();
				toCheck = &whiteSpaces;
			}

			for (vector<Coord>::iterator i = toCheck->begin(); i != toCheck->end(); ++i)
			{
				if (sqrt(pow(i->x - x, 2) + pow(i->y - y, 2)) < bestEucl)
				{
					bool neighboursValidated = false;
					if (sqrt(pow(i->x - x, 2) + pow(i->y - y, 2)) < 1.5)		//Making sure it isnt counting itself as a valid neighbour
					{
						Cell **tempValidationNeighbours;
						tempValidationNeighbours = neighborBehaviour->getNeighbors(i->x, i->y, 0, grid);

						int whiteValidate = neighborCounter(tempValidationNeighbours, 1);
						int blackValidate = neighborCounter(tempValidationNeighbours, 2);

						//maybe <= ?
						if (black)
						{
							if (whiteValidate <= 2 * (blackValidate - 1))
							{
								neighboursValidated = true;
							}
						}
						else      //white
						{
							if (blackValidate <= 2 * (whiteValidate - 1))
							{
								neighboursValidated = true;
							}
						}
						delete[] tempValidationNeighbours;
					}
					else
						neighboursValidated = true;
					if (neighboursValidated)
					{
						bestEucl = sqrt(pow(i->x - x, 2) + pow(i->y - y, 2));
						moveLoc = i;
						newLocFound = true;
					}
				}
			}
			if (newLocFound)
			{
				stay = false;

				outputGrid->setCellID(x, y, 0, 0);
				if (black)
				{
					outputGrid->getCell(moveLoc->x, moveLoc->y, 0)->setCellID(2);
					blackSpaces.erase(moveLoc);
				}
				else
				{
					outputGrid->getCell(moveLoc->x, moveLoc->y, 0)->setCellID(1);
					whiteSpaces.erase(moveLoc);
				}
			}

			if (black)
				vectorBlackAccess.unlock();
			else
				vectorWhiteAccess.unlock();

		}
		if (stay)
		{
			if (black)
				outputGrid->getCell(x,y,0)->setCellID(2);
			else
				outputGrid->getCell(x,y,0)->setCellID(1);
		}
		delete[] tempNeighbors;
	}

	return true;
}
//Thread function to split the workload like a coconut and i dont even like coconut
DWORD WINAPI getGoodSpots(void *dataIn)
{
	//And as death pointed to the void he said "whoops, wait i meen no...over heere", and so he pointed to the thread data instead
	threadData *data = (threadData *)dataIn;

	//Doing workload between spesified start and end points
	bool done = false;
	for (int i = data->startX; i < data->endX && i < data->width; i++)
	for (int j = data->startY; j < data->endY && j < data->height; j++)
	{
		if (data->grid->getCell(i, j, 0)->getCellID() == 0)
		{
			Cell **tempNeighbors;
			tempNeighbors = data->equasion->neighborBehaviour->getNeighbors(i, j, 0, data->grid);

			int white = data->equasion->neighborCounter(tempNeighbors, 1);
			int black = data->equasion->neighborCounter(tempNeighbors, 2);

			Coord temp;
			temp.x = i;
			temp.y = j;

			//maybe <= ?
			if (data->rnd->random(0, 1))
			{
				if (white <= 2 * black)
				{
					data->vectorBlackWrite->lock();
					data->blackSpaces->push_back(temp);
					data->vectorBlackWrite->unlock();
				}
				else if (black <= 2 * white)
				{
					data->vectorWhiteWrite->lock();
					data->whiteSpaces->push_back(temp);
					data->vectorWhiteWrite->unlock();
				}
			}
			else
			{
				if (black <= 2 * white)
				{
					data->vectorWhiteWrite->lock();
					data->whiteSpaces->push_back(temp);
					data->vectorWhiteWrite->unlock();
				}
				else if (white <= 2 * black)
				{
					data->vectorBlackWrite->lock();
					data->blackSpaces->push_back(temp);
					data->vectorBlackWrite->unlock();
				}
			}

			delete[] tempNeighbors;
		}
	}
	return 0;
}
void Segregation::initRule()
{
	whiteSpaces.clear();
	blackSpaces.clear();

	threadData data[2];
	int linesPerThread = grid->getWidth() / 2;
	if (2 * linesPerThread < grid->getWidth())
		linesPerThread++;

	//Setting info for each thread and creating it
	for (int i = 0; i < 2; i++)
	{
		data[i].whiteSpaces = &whiteSpaces;
		data[i].blackSpaces = &blackSpaces;
		data[i].vectorWhiteWrite = &vectorWhiteAccess;
		data[i].vectorBlackWrite = &vectorBlackAccess;
		data[i].equasion = this;
		data[i].grid = grid;
		data[i].rnd = &rnd;
		data[i].width = grid->getWidth();
		data[i].height = grid->getHeight();
		data[i].startX = linesPerThread*i;
		data[i].endX = linesPerThread*(i + 1);
		data[i].startY = 0;
		data[i].endY = grid->getHeight();
		hThreadArray[i] = CreateThread(NULL, 0, getGoodSpots, (void*)&data[i], 0, NULL);
	}
	//	*twiddle thumbs*
	WaitForMultipleObjects(2, hThreadArray, TRUE, INFINITE);
}
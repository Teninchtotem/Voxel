#pragma once
#include "Equasion.h"
using namespace std;

class OneDimentionalEquasion :
	public Equasion
{
protected:
	bool rules[8];

public:
	OneDimentionalEquasion(int equasionIDIn, Grid *gridIn, NeighborBehaviour *neighborBehaviourIn);
	~OneDimentionalEquasion(void);
	bool applyRule(int x, int y, int z, Grid *outputGrid);
	int neighborCounter(Cell **cells, int ID);



	void saveUnique(std::ostream& saveFile, const OneDimentionalEquasion &equasion) const
	{
		saveFile << equasion.neighborBehaviourID << " ";
		int rule = 0;
		for (int i = 0; i < 8; i++)
		{
			if (equasion.rules[i])
				rule += pow(2, i);
		}
		saveFile << rule << "\n";
	}
	virtual void save(std::ostream& saveFile, const Equasion &equasion) const
	{
		saveUnique(saveFile, *this);
	}
};


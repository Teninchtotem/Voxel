#pragma once
#include "Equasion.h"
using namespace std;

class TwoDimentionalEquasion :
	public Equasion
{
public:
	TwoDimentionalEquasion();
	~TwoDimentionalEquasion(void);
	bool applyRule(int x, int y, int z, Grid *outputGrid);
};


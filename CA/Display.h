#pragma once
#include "Grid.h"
#include "Console.h"
#include <iostream>
using namespace std;

class Display
{
protected:
	Console console;
public:
	Display();
	~Display();

	void wipeDisplay();
	void displayToScreen(Grid *grid);

};
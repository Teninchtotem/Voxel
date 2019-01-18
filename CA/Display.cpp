

#include "Display.h"


Display::Display()
{

}
Display::~Display()
{

}
void Display::wipeDisplay()
{
	console.clear();
}
void Display::displayToScreen(Grid *grid)
{
	for(int i = 0; i < grid->getHeight(); i++)
	{
		for(int j = 0; j < grid->getWidth(); j++)
		{
			if(grid->getCell(i,j,0)->getCellID() != 0)
			{
				console.gotoXY(i,j);
				switch(grid->getCell(i,j,0)->getCellID())
				{
				case 1: console.setColour(console.DEFAULT,console.LIGHT_YELLOW); break;
				case 2: console.setColour(console.DEFAULT,console.BLUE); break;
				case 3: console.setColour(console.DEFAULT,console.RED); break;
				}
				cout << " ";
			}
		}
	}
	console.setColour(console.DEFAULT,console.BLACK);

	/*cout << " ";
	for(int i = 0; i < grid->getWidth(); i++)
		cout << "_";
	cout << endl;

	for(int i = 0; i < grid->getHeight(); i++)
	{
		cout << "|";
		for(int j = 0; j < grid->getWidth(); j++)
		{
			if(grid->getCell(i,j,0)->getCellID() == 0)
				cout << " ";
			else
				cout << "X";
		}
		cout << "|" << endl;
	}
	cout << " ";
	for(int i = 0; i < grid->getWidth(); i++)
		cout << "_";
	cout << endl;*/
}

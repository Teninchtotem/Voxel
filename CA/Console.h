#pragma once
#include <iostream>
#include <string>
#include <windows.h>

class Console
{
private:
	static HANDLE hConsole; 

public:
	static enum COLOUR {BLACK, BLUE, GREEN, AQUA, RED, PURPLE, YELLOW, DEFAULT, GRAY, LIGHT_BLUE, 
		LIGHT_GREEN, LIGHT_AQUA, LIGHT_RED, LIGHT_PURPLE, LIGHT_YELLOW, WHITE };

	static void clear();
	// Description: Clears the screen

	static void gotoXY(int x, int y);
	// Precondition: two non-negative integer parameters for the x and y are provided
	// Description: Moves the cursor to x, y in console window, i.e. x=left\right y=top\bottom
	// Example: gotoXY(10, 10) moves the cursor to those coordinates
	//			gotoXY(0, 10) moves the cursor to those coordinates
	//			gotoXY(-1, 0), gotoXY(0, -1) and gotoXY(-1, -1) throw an exception

	static void setColour(COLOUR foreground, COLOUR background);
	// Precondition: an enumerated COLOUR foreground and background type for colour is required from
	//				BLACK, BLUE, GREEN, AQUA, RED, PURPLE, YELLOW, DEFAULT, GRAY, LIGHT_BLUE, 
	//				LIGHT_GREEN, LIGHT_AQUA, LIGHT_RED, LIGHT_PURPLE, LIGHT_YELLOW, WHITE
	// Postcondition: text foreground and background colour is changed to one of these 256 possibilities
	// Description: changes the foreground and background text colour within the console window
	// Example: setColour(BLACK, WHITE) changes the foreground text colour to black and the background to white.

};

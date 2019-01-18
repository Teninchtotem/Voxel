#pragma once
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <fstream>

class CASimulator;

using namespace std;

class FileManager
{
protected:
public:
	FileManager();
	~FileManager();
	string getNameOfSaveFile();
	bool saveSimulatorStateFromName(string path, CASimulator &simulator);
	string getNameOfLoadFile();
	bool loadSimulatorStateFromName(string path, CASimulator &simulator);
	string addPath(string fileName);
};


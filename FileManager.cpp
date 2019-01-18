#include "FileManager.h"
#include "CA\CASimulator.h"
#include "Log.h"
#include <cerrno>


FileManager::FileManager()
{
}


FileManager::~FileManager()
{
}
string FileManager::getNameOfSaveFile()
{
	OPENFILENAME ofn;

	char szFileName[MAX_PATH] = "";

	TCHAR NPath[MAX_PATH];
	TCHAR CompletePath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);

	int index = 0;
	TCHAR addon[10] = { '\\', 'E', 'x', 'a', 'm', 'p', 'l', 'e', 's', '\0' };
	while (NPath[index] != 0)
	{
		CompletePath[index] = NPath[index];
		index++;
	}
	for (int i = 0; i < 10; i++)
	{
		CompletePath[index + i] = addon[i];
	}

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrInitialDir = CompletePath;
	ofn.lpstrFilter = (LPCWSTR)L"Matt's CA (*.mca)\0*.mca\0";
	ofn.lpstrFile = (LPWSTR)szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = (LPCWSTR)L"mca";


	GetSaveFileName(&ofn);

	string returnString;

	for (int i = 0; i < MAX_PATH; i++)
		returnString += ofn.lpstrFile[i];
	return returnString;
}
bool FileManager::saveSimulatorStateFromName(string pathIn, CASimulator &simulator)
{
	pathIn = addPath(pathIn);
	TCHAR path[MAX_PATH];
	for (int i = 0; i < pathIn.size(); i++)
		path[i] = pathIn[i];
	ofstream saveFile;
	saveFile.open(path);


	if (saveFile.is_open())
	{
		saveFile << simulator;
		saveFile << *simulator.getRuleGrid();

		for (int i = 0; i < simulator.getRuleGrid()->getRules().size(); i++)
		{
			saveFile << *simulator.getRuleGrid()->getRules()[i];
		}

		for (int i = 0; i < simulator.getRuleGrid()->getWidth(); i++)
		{
			for (int j = 0; j < simulator.getRuleGrid()->getHeight(); j++)
			{
				for (int k = 0; k < simulator.getRuleGrid()->getDepth(); k++)
				{
					saveFile << simulator.getRuleGrid()->getCell(i, j, k)->getCellID() << " " << simulator.getRuleGrid()->getCell(i, j, k)->getCellState() << " ";
				}
			}
		}
		saveFile << "\n";

		saveFile.close();
		return true;
	}
	else
		return false;
}
string FileManager::getNameOfLoadFile()
{
	OPENFILENAME ofn;

	char szFileName[MAX_PATH] = "";

	TCHAR NPath[MAX_PATH];
	TCHAR CompletePath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);

	int index = 0;
	TCHAR addon[10] = { '\\', 'E', 'x', 'a', 'm', 'p', 'l', 'e', 's', '\0' };
	while (NPath[index] != 0)
	{
		CompletePath[index] = NPath[index];
		index++;
	}
	for (int i = 0; i < 10; i++)
	{
		CompletePath[index + i] = addon[i];
	}

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrInitialDir = CompletePath;
	ofn.lpstrFilter = (LPCWSTR)L"Matt's CA (*.mca)\0*.mca\0";
	ofn.lpstrFile = (LPWSTR)szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = (LPCWSTR)L"mca";

	GetOpenFileName(&ofn);

	string returnString;

	for (int i = 0; i < MAX_PATH; i++)
		returnString += ofn.lpstrFile[i];
	return returnString;
}
bool FileManager::loadSimulatorStateFromName(string pathIn, CASimulator &simulator)
{
	pathIn = addPath(pathIn);
	TCHAR path[MAX_PATH];
	for (int i = 0; i < pathIn.size(); i++)
		path[i] = pathIn[i];
	ifstream saveFile;
	saveFile.open(path);
	if (saveFile.fail())
	{
		std::string sErr = "Failed to open File ";
		sErr += pathIn;
		Win::log(sErr.c_str());
		std::string sErr2 = "ERROR:\t";
		sErr2 += strerror(errno);
		Win::log(sErr2.c_str());
	}

	if (saveFile.is_open())
	{
		simulator.getMutex()->lock();
		simulator.wipeGrid();
		simulator.getRuleGrid()->removeAllEquasions();

		bool advanced = true;
		int CAType, Tessellation = 0, Looping;
		saveFile >> CAType;
		if (CAType == 1)
			saveFile >> Tessellation;
		saveFile >> Looping;

		simulator.getMutex()->unlock();
		simulator.setCAType(CAType);
		simulator.setTwoDimentionalTessellation(Tessellation);
		simulator.setLooping(Looping);
		simulator.getMutex()->lock();

		int width, height, depth, ruleNo;
		saveFile >> width >> height >> depth >> ruleNo;
		simulator.getMutex()->unlock();
		simulator.setGridSize(width, height, depth);
		simulator.getMutex()->lock();
		for (int i = 0; i < ruleNo; i++)
		{
			bool oneDimentional = false;
			int ruleType, ID, neighborsSet, symbolSet, initial, compare, lookID, become, neighborType;
			saveFile >> ruleType;
			switch (ruleType)
			{
			case 0:		//Standard rules for 2D and 3D
				saveFile >> ID >> neighborsSet >> symbolSet >> initial >> compare >> lookID >> become;
				if (ID < 0)
					advanced = false;
				switch (neighborsSet)
				{
				case 0:		neighborType = 1;	break;
				case 1:		neighborType = 0;	break;
				default:	neighborType = 0;	break;
				}
				simulator.getRuleGrid()->addEquasion(ID, neighbors(neighborsSet), symbols(symbolSet), initial, lookID, compare, become, simulator.getLooping());
				break;
			case 1:		//1D step rules
				oneDimentional = true;
				advanced = false;
				saveFile >> neighborsSet >> become;
				simulator.getRuleGrid()->addOneDimentionalEquasion(become, neighbors(neighborsSet), simulator.getLooping());
				break;
			case 2:		//2D step rules
				saveFile >> ID >> neighborsSet >> symbolSet >> initial >> compare >> lookID >> become;
				if (ID < 0)
					advanced = false;
				simulator.getRuleGrid()->addTwoStepEquasion(ID, neighbors(neighborsSet), symbols(symbolSet), initial, lookID, compare, become, simulator.getLooping());
				break;
			}
			if (!oneDimentional)
			simulator.setSquareNeighborType(neighborType);
		}
		int cellID, cellState;
		for (int i = 0; i < simulator.getRuleGrid()->getWidth(); i++)
		{
			for (int j = 0; j < simulator.getRuleGrid()->getHeight(); j++)
			{
				for (int k = 0; k < simulator.getRuleGrid()->getDepth(); k++)
				{
					saveFile >> cellID >> cellState;
					simulator.getRuleGrid()->getCell(i, j, k)->setCellID(cellID);
					simulator.getRuleGrid()->getCell(i, j, k)->setCellState(cellState);
					simulator.getSecondaryGrid()->getCell(i, j, k)->setCellID(cellID);
					simulator.getSecondaryGrid()->getCell(i, j, k)->setCellState(cellState);
				}
			}
		}
		simulator.setAdvanced(advanced);
		simulator.getMutex()->try_lock();
		simulator.getMutex()->unlock();
		saveFile.close();
		return true;
	}
	else
		return false;
}
string FileManager::addPath(string fileName)
{
	//PIEPIEPIEPIEPIE comment maybe? (ironic, huh)
	TCHAR NPath[MAX_PATH];
	TCHAR CompletePath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);

	bool same = true;

	for (int i = 0; i < 6; i++)
	{
		if (NPath[i] != fileName[i])
			same = false;
	}

	if (!same)
	{
		int index = 0;
		TCHAR addon[10] = { '\\', 'E', 'x', 'a', 'm', 'p', 'l', 'e', 's', '\\' };
		while (NPath[index] != 0)
		{
			CompletePath[index] = NPath[index];
			index++;
		}
		bool examplesDone = false;
		if (index > 9)
		if (CompletePath[index - 1] == 's')
		if (CompletePath[index - 2] == 'e')
		if (CompletePath[index - 3] == 'l')
		if (CompletePath[index - 4] == 'p')
		if (CompletePath[index - 5] == 'm')
		if (CompletePath[index - 6] == 'a')
		if (CompletePath[index - 7] == 'x')
		if (CompletePath[index - 8] == 'E')
		if (CompletePath[index - 9] == '\\')
		examplesDone = true;

		if (!examplesDone)
		{
			for (int i = 0; i < 10; i++)
			{
				CompletePath[index + i] = addon[i];
			}
			index += 10;
		}
		else
			CompletePath[index++] = '\\';
		int actualSize = 0;
		for (int i = 0; i < MAX_PATH && i < fileName.size(); i++)
		{
			actualSize = i + 1;
			if (fileName[i] == '\0')
			{
				i = MAX_PATH;
			}
		}
		for (int i = 0; i < actualSize; i++)
		{
			CompletePath[index + i] = fileName[i];
		}
		index += actualSize;


		bool sameEnd = true;
		if (fileName.size() < 4)
			sameEnd = false;
		else if (fileName[fileName.size() - 4] != '.')
			sameEnd = false;

		if (!sameEnd)
		{
			CompletePath[index++] = '.';
			CompletePath[index++] = 'm';
			CompletePath[index++] = 'c';
			CompletePath[index++] = 'a';
			CompletePath[index] = '\0';
		}
		if (CompletePath[index] != '\0')
			CompletePath[index] = '\0';

		fileName = "";
		for (int i = 0; i < MAX_PATH; i++)
		{
			fileName += CompletePath[i];
		}
	}
	return fileName;

}
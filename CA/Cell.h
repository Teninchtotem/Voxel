#pragma once
class Cell
{
protected:
	int cellState;
	int cellID;
	int location[3];

public:
	Cell();
	~Cell();
	
	void setCellID(int cellIDIn);
	void setCellState(int cellStateIn);
	void setLocation(int x, int y, int z);

	int getCellID();
	int getCellState();
	void getLocation(int &xOut, int &yOut, int &zOut);
	Cell* getCell();
};
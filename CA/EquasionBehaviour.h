#pragma once

class EquasionBehaviour
{
protected:
	int equasionType;
public:
	EquasionBehaviour(void);
	~EquasionBehaviour(void);
	int getEquasionType() { return equasionType; };
	virtual bool calculate(int neighbourCount, int countToTestAgainst);
};


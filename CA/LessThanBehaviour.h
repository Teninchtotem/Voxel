#pragma once
#include "EqualBehaviour.h"
class LessThanBehaviour :
	public EquasionBehaviour
{
public:
	LessThanBehaviour(void);
	~LessThanBehaviour(void);
	
	virtual bool calculate(int neighbourCount, int countToTestAgainst);
};


#pragma once
#include "EqualBehaviour.h"
class GreaterThanBehaviour :
	public EquasionBehaviour
{
public:
	GreaterThanBehaviour(void);
	~GreaterThanBehaviour(void);
	
	virtual bool calculate(int neighbourCount, int countToTestAgainst);
};


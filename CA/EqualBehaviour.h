#pragma once
#include "equasionbehaviour.h"
class EqualBehaviour :
	public EquasionBehaviour
{
public:
	EqualBehaviour(void);
	~EqualBehaviour(void);
	
	virtual bool calculate(int neighbourCount, int countToTestAgainst);
};


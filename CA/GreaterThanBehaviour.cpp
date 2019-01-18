

#include "GreaterThanBehaviour.h"


GreaterThanBehaviour::GreaterThanBehaviour(void)
{
	equasionType = 2;
}


GreaterThanBehaviour::~GreaterThanBehaviour(void)
{
}


bool GreaterThanBehaviour::calculate(int neighbourCount, int countToTestAgainst)
{
	if(neighbourCount > countToTestAgainst)
		return true;
	else
		return false;
}

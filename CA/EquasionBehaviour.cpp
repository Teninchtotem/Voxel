

#include "EquasionBehaviour.h"


EquasionBehaviour::EquasionBehaviour(void)
{
	equasionType = 0;
}


EquasionBehaviour::~EquasionBehaviour(void)
{
}


bool EquasionBehaviour::calculate(int neighbourCount, int countToTestAgainst)
{
	if(neighbourCount == countToTestAgainst)
		return true;
	else
		return false;
}



#include "LessThanBehaviour.h"


LessThanBehaviour::LessThanBehaviour(void)
{
	equasionType = 1;
}


LessThanBehaviour::~LessThanBehaviour(void)
{
}


bool LessThanBehaviour::calculate(int neighbourCount, int countToTestAgainst)
{
	if(neighbourCount < countToTestAgainst)
		return true;
	else
		return false;
}

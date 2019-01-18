

#include "EqualBehaviour.h"


EqualBehaviour::EqualBehaviour(void)
{
}


EqualBehaviour::~EqualBehaviour(void)
{
}



bool EqualBehaviour::calculate(int neighbourCount, int countToTestAgainst)
{
	if(neighbourCount == countToTestAgainst)
		return true;
	else
		return false;
}
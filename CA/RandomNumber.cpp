//
//  RandomNumber.cpp
//
//  Created by Ian Wilson on 26/02/2012.
//  Copyright (c) 2012 University of Glamorgan. All rights reserved.
//

#include "RandomNumber.h"

RandomNumber::RandomNumber()
{
	initialiseSeed();
}

void RandomNumber::initialiseSeed()
// precondition:
// postcondition:
// random number initialised
// description:
// random number has been initialised to the current time.
// example:
// RandomNumber::initialiseSeed(); for direct call to the method or
// RandomNumber rnd;
// rnd.initialseSeed(); for a call from an object
{
    srand((unsigned int)time(0));
}

int RandomNumber::random(int lower, int upper)
// preconditions: 
// lower >= 0 upper > 0 upper > lower.
// postconditions: 
// returns a positive integer within the lower and upper bound range 
// description: 
// this function will return a positive random number within a specific lower and 
// upper boundary. 
// examples: 
// randomNumber(10, 20) returns 14
// randomNumber(20, 10) generates an exception
// randomNumber(0, 0) generates an exception
// randomNumber(10, -20) generates an exception
// randomNumber(-10, -20) generates an exception
{
    if(lower>=upper) 
    {
        cout << "Precondition failed. Upper bound must exceed lower bound." << endl;
        exit(EXIT_FAILURE);
    }
    else if(lower<0)
    {
        cout << "Precondition failed. Lower bound must be greater than zero." << endl;
        exit(EXIT_FAILURE);
    }
    else if(upper<1)
    {
        cout << "Precondition failed. Upper bound must be greater than zero." << endl;
        exit(EXIT_FAILURE);
    }
    else 
    {
        int range = upper - lower + 1;
        return (rand() % range + lower);
    }
}
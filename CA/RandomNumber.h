//
//  RandomNumber.h
//  Collision
//
//  Created by Ian Wilson on 26/02/2012.
//  Copyright (c) 2012 University of Glamorgan. All rights reserved.
//

#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class RandomNumber
{
public:
	RandomNumber();
    // precondition:
    // postcondition:
    // RandomNumber object created
    // description:
    // Calling the default constructor will create a RandomNumber object and seed
	// the random number.
    // example:
    // RandomNumber rnd;

    static void initialiseSeed();
    // precondition:
    // postcondition:
    // random number initialised
    // description:
    // random number has been initialised to the current time.
    // example:
    // RandomNumber::initialiseSeed(); for direct call to the method or
    // RandomNumber rnd;
    // rnd.initialseSeed(); for a call from an object
    
    static int random(int lower, int upper);
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
};

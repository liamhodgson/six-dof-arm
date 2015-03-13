#include <iostream>
#include <vector>
#include "servo.h"
#include "manipulator.h"

Manipulator::Manipulator(std::vector<int> ids)
{
	this->nServos = ids.size(); // initialize number of servos in manipulator
	for(int i = 0; i<nServos; i++)
		this->actuators[i] = new Servo(ids[i]); // create a new servo object for each ids

}

Manipulator::move(std::vector<int> goals)
{
	if(goals.size() != nServos)
		throw "Goal command required for each servo";

	for(int i = 0; i<nServos; i++)
		// send move command to servo i		

}

Manipulator::~Manipulator(void)
{
	for(int i = 0; i<actuators.size(); i++)
		delete actuators[i]; // free heap memory

}

#ifndef _MANIPULATOR_H_
#define _MANIPULATOR_H_

#include <iostream>
#include <vector>
#include "servo.h"

class Manipulator : private Servo
{
	public:
		Manipulator(std::vector<int> ids, int fd); // constructor
		void moveAll(std::vector<int> goals); // send move commands to all servos
		~Manipulator(void); // destructor
	private:
		int nServos; // the number of servos that make up the manipulator
		std::vector<Servo*> actuators; // vector of pointers to Servo objects
		int fd; // file descriptor for serial read/write
};

#endif

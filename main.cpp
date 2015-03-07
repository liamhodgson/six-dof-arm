#include <iostream>
#include <vector>
#include "servo.h"
#include "serial.h"

using namespace std;

int main(int argc, char* argv[])
{
	ServoSerial ttyO1;

	Servo servo1(0xFE,ttyO1.access());

	//servo1.move(512);

	//cout << "Servo position is " << servo1.getPosition() << endl;

	return 0;
}
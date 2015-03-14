// Allows user to read RAM address values, write to memory and move servo from command line.
// Useful for testing servo limits and setting servo ID when multiple servos are in use.

#include <iostream>
#include <vector>
#include "servo.h"
#include "serial.h"

using namespace std;

int main(void)
{
	cout << "HerkuleX DRS-0101/0102 Servo Debug Menu" << endl;
	
	// automatically send stat command all servo ids and read back actual id of connected servo.
	// modify id member variable to this id.
	// return error if no reply (no servo connected)
	// display is of servo
	
	ServoSerial ttyO1;
	Servo servo1(0xFD,ttyO1.access());

	cout << "Select an option:" << endl;
	cout << "[0] Return to menu" << endl << "[1] Read from RAM address" << endl;
	cout << "[2] Write to RAM address" << endl << "[3] Move servo" << endl;
	cout << "[4] Set servo ID" << endl << ">> ";
	
	int option;
	cin >> option;












	return 0;
}

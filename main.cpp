#include <iostream>
#include <vector>
#include <unistd.h>
#include "servo.h"
#include "serial.h"

using namespace std;

int main(int argc, char* argv[])
{
	ServoSerial ttyO1;

	Servo servo1(0xFD,ttyO1.access());

	int goal;
	cout << "Enter goal position:" << endl << ">> ";
	cin >> goal;
	while(goal>0){
		try{
			servo1.move(goal);
		}catch(const char* msg){
		cerr << msg << endl;
		}
		
		usleep(900*1000); // 900ms
		cout << "Servo angle is " << servo1.getPosition() << endl;

		cout << ">> ";
		cin >> goal;
	}

	return 0;
}

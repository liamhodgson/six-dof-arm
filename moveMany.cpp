#include <iostream>
#include <vector>
#include <unistd.h>
#include "servo.h"
#include "serial.h"

using namespace std;

int main(void)
{
	ServoSerial ttyO1;

	vector<int> ids {0x01,0x02};
	Manipulator twoMotor(ids);

	vector<int> goalvec (2);
	cout << "Enter goal position:" << endl << ">> ";
	cin >> goal
	while(goal>0){
		goalvec(1) = goal;
		goalvec(2) = goal; 
		try{
			twoMotors.move(goalvec);
		}catch(const char* msg){
		cerr << msg << endl;
		}
		
		cout << ">> ";
		cin >> goal;
	}

	return 0;
}

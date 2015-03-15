#include <iostream>
#include <vector>
#include <unistd.h>
#include "servo.h"
#include "serial.h"
#include "manipulator.h"

using namespace std;

int main(void)
{
	ServoSerial ttyO1;

	int tmp[2] = {0x01,0x02};
	vector<int> ids(&tmp[0],&tmp[0]+2);
	Manipulator twoMotor(ids,ttyO1.access());

	vector<int> goalvec (2);
	cout << "Enter goal position:" << endl << ">> ";
	int goal;
	cin >> goal;
	while(goal>0){
		goalvec[1] = goal;
		goalvec[2] = goal; 
		try{
			twoMotor.moveAll(goalvec);
		}catch(const char* msg){
		cerr << msg << endl;
		}
		
		cout << ">> ";
		cin >> goal;
	}

	return 0;
}

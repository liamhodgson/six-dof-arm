#include <iostream>
#include <vector>
#include <unistd.h>
#include "servo.h"
#include "serial.h"

using namespace std;

int main(int argc, char* argv[])
{
	ServoSerial ttyO1;

	Servo servo1(0x0A,ttyO1.access());

	vector<int> data;
	data.push_back(0xFD);
	servo1.writeAddr(0x00,data);


	int addr=0x00, nBytes=0x01;
	vector<int> reply;
	reply = servo1.readAddr(addr,nBytes);
	cout << "received: ";
	for(int i = 0; i<reply.size(); i++)
		cout << reply[i] << " ";
	cout << endl;

/*
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
*/
	return 0;
}

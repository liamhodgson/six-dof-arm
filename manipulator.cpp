#include <iostream>
#include <vector>
#include "servo.h"
#include "manipulator.h"

using namespace std;

Manipulator::Manipulator(vector<int> ids, int newfd)
{
	this->nServos = ids.size(); // initialize number of servos in manipulator
	for(int i = 0; i<nServos; i++)
		this->actuators.push_back(new Servo(ids[i], fd)); // create a new servo object for each id

	cmd.servoID = 0xFE; // manipulator object sends commands to all servos

	fd = newfd;
}

// sends specified command to each servo, using I_JOG tag
void Manipulator::moveAll(vector<int> goals)
{
	if(goals.size() != nServos)
		throw "Goal command required for each servo";
	
	// get move tag from each servo
	vector<int> tag;
	vector<int> cmdData;
	for(int i = 0; i<nServos; i++)
	{
		tag = actuators[i]->makeI_JOGtag(goals[i]);
		cmdData.insert(cmdData.end(), tag.begin(), tag.end());	
	}

	// call modified I_JOG with multiple tags
	cmd.packetSize = 7 + cmdData.size();
	cmd.command = 0x05; // I_JOG command
	cmd.pktData = cmdData;

	cout << "in moveAll, fd = " << fd << endl;

	this->pktCreate();
	this->send();	
}

Manipulator::~Manipulator(void)
{
	for(int i = 0; i<actuators.size(); i++)
		delete this->actuators[i]; // free heap memory

}

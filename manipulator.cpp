#include <iostream>
#include <vector>
#include "servo.h"
#include "manipulator.h"

Manipulator::Manipulator(std::vector<int> ids)
{
	this->nServos = ids.size(); // initialize number of servos in manipulator
	for(int i = 0; i<nServos; i++)
		this->actuators[i] = new Servo(ids[i]); // create a new servo object for each id

	cmdmservoID = 0xFE; // manipulator object sends commands to all servos
}

// sends specified command to each servo, using I_JOG tag
Manipulator::moveAll(std::vector<int> goals)
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

	this->pktCreate();
	this->send();	
}

void Manipulator::send(void)
{
	int len = cmdPacket.size();

	char buf[len];
	for(int i = 0; i<len; i++){
		buf[i] = (char)cmdPacket[i];
		cout << hex << (int)buf[i] << " ";
	}
	cout << dec << endl;

	write(fd, &buf, len); // send the command over the serial line
}

Manipulator::~Manipulator(void)
{
	for(int i = 0; i<actuators.size(); i++)
		delete this->actuators[i]; // free heap memory

}

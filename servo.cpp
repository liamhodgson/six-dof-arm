#include <iostream>
#include <vector>
#include <fcntl.h>
#include "servo.h"

using namespace std;

Servo::Servo(int id, int serial_fd)
{
	cmd.servoID = id;
	fd = serial_fd;

	// set torque policy to on, so that servo will actually move
	this->torqueON();
}


void Servo::move(float angle)
{
	// convert move command into m/lsb
	int LSB = (int)angle&0x00FF;
	int MSB = ((int)angle&0xFF00)>>8;

	// send command to servo
	this->I_JOG(MSB, LSB);
}

float Servo::getPosition(void)
{
	vector<int> posData;
	posData = this->RAMread(0x3C, 0x02); // read 2 bytes from RAM address 60 (0x3C, absolute position)	
	// convert two bytes of position data into float
	return posData[0];
}


void Servo::torqueON(void)
{
	vector<int> tmp;
	tmp.push_back(0x34); // address of torque control in RAM
	tmp.push_back(0x01);
	tmp.push_back(0x60);
	
	this->RAMwrite(tmp);
}


void Servo::RAMwrite(vector<int> data)
{
	cmd.pktData.clear();
	cmd.pktData = data; // first value is address, remainder is data
	cmd.packetSize = 7 + data.size(); // size of header + data to write
	cmd.command = 0x03; // RAM_WRITE command number
	
	this->pktCreate();
	this->send();
}


vector<int> Servo::RAMread(int addr, int nBytes)
{
	cmd.pktData.clear();
	cmd.pktData.push_back(addr);
	cmd.pktData.push_back(nBytes);
	cmd.packetSize = 9; // size of header (7) + data to write (2)
	cmd.command = 0x04; // RAM_READ command number
	
	this->pktCreate();
	this->send();
	return this->receive(nBytes);
}


void Servo::I_JOG(int MSB, int LSB)
{
	// create vector of I_JOG tag data
	cmd.pktData.clear();
	cmd.pktData.push_back(LSB);
	cmd.pktData.push_back(MSB);
	cmd.pktData.push_back(0x04); // SET byte; fixed for the moment
	cmd.pktData.push_back(cmd.servoID);
	cmd.pktData.push_back(0x3C); // PLAYTIME byte; fixed for the moment

	cmd.packetSize = 7 + cmd.pktData.size();
	cmd.command = 0x05; // I_JOG command number

	this->pktCreate();
	this->send();
}


void Servo::pktCreate(void)
{
	cmdPacket.clear(); // ensure new command packet is empty

	cmdPacket.push_back(0xFF);
	cmdPacket.push_back(0xFF);
	cmdPacket.push_back(cmd.packetSize);
	cmdPacket.push_back(cmd.servoID);
	cmdPacket.push_back(cmd.command);
	cmdPacket.push_back(this->chk1());
	cmdPacket.push_back(this->chk2());
	for(int i = 0; i<cmd.pktData.size(); i++)
		cmdPacket.push_back(cmd.pktData[i]);
}

// calculates and returns checksum 1
int Servo::chk1(void)
{
	int tmp = (cmd.packetSize)^(cmd.servoID)^(cmd.command);
	for(int i = 0; i < cmd.pktData.size(); i++)
		tmp = tmp^cmd.pktData[i];
	return tmp&0xFE;
}

// calculates and returns checksum 2
int Servo::chk2(void)
{
	return (~chk1())&0xFE;
}


void Servo::send(void)
{
	int* buf = &cmdPacket[0];
	int len = cmdPacket.size();
	cout << "Command packet sent: " << endl;
	cout << "Length: " << len << endl;
	for(int i = 0; i<len; i++)
		cout << hex << buf[i] << dec << endl;
	write(fd, buf, len); // send the command over the serial line
	cout << "Write returns: " << err << endl;
}


vector<int> Servo::receive(int nBytes)
{
	cout << "Received " << nBytes << " of data." << endl;

	vector<int> fake_return(1);
	fake_return[0] = -1;
	return fake_return;
}

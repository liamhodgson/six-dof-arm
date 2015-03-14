#include <iostream>
#include <stdio.h>
#include <vector>
#include <fcntl.h>
#include <cstring>
#include <cstdio>
#include <sstream>
#include "servo.h"

using namespace std;

Servo::Servo(int id, int serial_fd)
{
	cmd.servoID = id;
	fd = serial_fd;

	// set torque policy to on, so that servo will actually move
	this->torqueON();
}


void Servo::move(float goal)
{
	if(goal<21||goal>1002)
		throw "Position out of range";

	// convert move command into m/lsb
	int LSB = (int)goal&0x00FF;
	int MSB = ((int)goal&0xFF00)>>8;

	// send command to servo
	this->I_JOG(MSB, LSB);
}

float Servo::getPosition(void)
{
	vector<int> posData;
	posData = this->RAMread(0x3A, 0x02); // read 2 bytes from RAM address 60 (0x3C, absolute position)	

	float offset = 24598.0;
	float to_angle = 0.325; // factor to convert from reading to servo angle
	return ((float)(posData[0] + (posData[1]<<8))-offset)*to_angle;
}


vector<int> Servo::readAddr(int addr, int nBytes)
{
	return this->RAMread(addr, nBytes);
}

void Servo::writeAddr(int addr, vector<int> data)
{
	this->RAMwrite(addr, data);
}

void Servo::torqueON(void)
{
	vector<int> tmp;
	tmp.push_back(0x60);
	
	this->RAMwrite(0x34, tmp);
}


void Servo::RAMwrite(int addr, vector<int> data)
{
	cmd.pktData.clear();
	cmd.pktData.push_back(addr); // first value is address
	cmd.pktData.push_back(data.size()); // number of bytes to be written
	cmd.pktData.insert(cmd.pktData.end(),data.begin(),data.end()); // append data to be sent to end of packet
	cmd.packetSize = 7 + 2 + data.size(); // size of header + address/data length + data to write
	cmd.command = 0x03; // RAM_WRITE command number

	this->pktCreate();
	this->send();

	// if the servo id is changed, need to update that variable
	if(addr==0x00)
		cmd.servoID = data[0];
}


vector<int> Servo::RAMread(int addr, int nBytes)
{
	cmd.pktData.clear();
	cmd.pktData.push_back(addr);
	cmd.pktData.push_back(nBytes);
	cmd.packetSize = 9; // size of header (7) + number bytes to read (1) + address (1)
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

std::vector<int> Servo::makeI_JOGtag(int goal)
{
	// generates an I_JOG tag to move servo
	// convert move command into m/lsb
	int LSB = (int)goal&0x00FF;
	int MSB = ((int)goal&0xFF00)>>8;

	vector<int> tag;
	tag.push_back(LSB); // position lsb
	tag.push_back(MSB); // position msb
	tag.push_back(0x04); // SET value; sets position control and LED; fixed for now
	tag.push_back(cmd.servoID); // id of this servo
	tag.push_back(0x3C); // playtime (in multiples of 11.2ms); fixed for now

	return tag;
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
	int len = cmdPacket.size();

	char buf[len];
	for(int i = 0; i<len; i++){
		buf[i] = (char)cmdPacket[i];
	}

	write(fd, &buf, len); // send the command over the serial line
}

vector<int> Servo::receive(int nBytes)
{
	int len = 12 + nBytes; // command echo + return data + status error
	vector<int> ack(len);
	char c;
	for(int i = 0; i<len; i++){
		read(fd,&c,sizeof(c));
		ack[i] = (int)c;
	}

	vector<int> data;
	for(int i = 0; i<nBytes; i++)
		data.push_back(ack[9+i]);

	return data;
}

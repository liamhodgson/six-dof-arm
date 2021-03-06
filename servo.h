#ifndef _SERVO_H_
#define _SERVO_H_

#include <vector>

class Servo
{
	public:
		/* Note: setting servo id from constructor does not actually assign physical servo's id.
		 * Each servo needs to have its id manually and individually assigned before
		 * trying to control it using this servo class.
		 */
		// default constructor
		Servo(void);
		// constructor
		Servo(int id, int serial_fd);
		// move servo to specified angle. return command array
		void move(float angle);
		// returns the current calibrated angle of the servo
		float getPosition(void);
		// sends specified data to specified address
		std::vector<int> readAddr(int addr, int nBytes);
		// write data to specified address
		void writeAddr(int addr, std::vector<int> data);
	
		// returns I_JOG tag for goal position
		std::vector<int> makeI_JOGtag(int goal);
	protected:
		/**
		 ** command packet specific member functions
		 **/
		 
		void send(void);
		std::vector<int> receive(int nBytes);
		void pktCreate(void);

		// file descriptor for accessing serial communication
		int fd;
		
		int chk1(void);
		int chk2(void);

		// struct to store data to be sent to servo
		struct packet_template{
			// number of bytes in data packet
			int packetSize;
			// ID of servo to which command is sent
			int servoID;
			// command number
			int command;
			// data to be sent to servo
			std::vector<int> pktData;
		}cmd;

		// complete command packet ready to be sent to servo
		std::vector<int> cmdPacket;


	private:
		// stores the servo's min and max position
		char positionLimit[2];

		// returns nbytes of data from EEP address
		char* EEPread(char address, char nbytes);
		// writes data array to EEP address
		void EEPwrite(char address, char* data);
		// reads nbytes from RAM address
		std::vector<int> RAMread(int addr, int nBytes);
		// writes data array to RAM address
		void RAMwrite(int addr, std::vector<int> data);
		// sends command to move servo (in future add SET and movetime)
		void I_JOG(int MSB, int LSB);
		// sends command to move servo
		void S_JOG(char MSB, char LSB, char SET, char movetime);
		// requests info from servo
		char* STAT(void);

		// turn on torque (needs to be called for move() to work!)
		void torqueON(void);
		// set the position that the servo is currently at to zeroAngle 
		void calibrate(float zeroAngle);  

		/* General order of calls: servo object calls public member function. This calls
		 * a low-level servo command (sets data to be sent in cmd struct), then calls 
		 * cmdCreate (sets remain command packet values and returns complete command packet).
		 */ 
};

#endif

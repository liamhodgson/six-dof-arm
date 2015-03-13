#ifndef _SERIAL_H_
#define _SERIAL_H_

class ServoSerial
{
	public:
		ServoSerial(void); // constructor
		int access(void); // return port file descriptor
		
	private:
		int fd;
};

#endif

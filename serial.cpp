#include <iostream>
#include "serial.h"

using namespace std;

ServoSerial::ServoSerial(void)
{
	const char uartpath[11] = "/dev/ttyO1";
	struct termios uart4,old;

	// open UART4 for read/write
    fd = open(uartpath, O_RDWR | O_NOCTTY); // (from fcntl.h) opens file for read/write and returns file descriptor.
    if(fd<0)
            printf("uart port failed to open");

    // save current attributes
    tcgetattr(fd,&old); // reads the original attributes of the uart port to a termios struct
    bzero(&uart4,sizeof(uart4)); // writes zeros to the uart4 structi

    uart4.c_cflag = B115200 | CS8 | CLOCAL | CREAD; // baud rate | character size mask | ignore modem control lines | enable receiver
    uart4.c_iflag = IGNPAR | ICRNL; // input modes
    uart4.c_oflag = 0; // output modes
    uart4.c_lflag = 0; // local modes

    // special characters
    uart4.c_cc[VTIME] = 0;
    uart4.c_cc[VMIN] = 1;

    // clean the line and set attributes
    tcflush(fd,TCIFLUSH); // discards data received but not read
    tcsetattr(fd,TCSANOW,&uart4); // uses uart4 struct to set parameters of terminal fd (TCSANOW = immediately)
}

int ServoSerial::port(void)
{
    return fd;
}

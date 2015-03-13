#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[]){
	int fd;
	const char uartpath[11] = "/dev/ttyO1";
	unsigned char data;
	//char buf[7] = {0xFF,0xFF,0x07,0xFD,0x07,0xFC,0x02}; // STAT command
	//char buf[7] = {0xff, 0xff, 0x07, 0xFD, 0x09, 0xf2, 0x0c}; // reboot
	//char buf[10] = {0xFF,0xFF,0x0A,0xFD,0x03,0xC0,0x3E,0x35,0x01,0x01}; // turn LED green
	//char buf[9] = {0xff, 0xff, 0x09, 0xFD, 0x08, 0xfc, 0x02, 0, 0}; // rollback
	char buf[9] = {0xff, 0xff, 0x09, 0xfd, 0x04, 0x00, 0x00, 0x3c, 0x01}; // read position data
	buf[5] = (buf[2]^buf[3]^buf[4]^buf[7]^buf[8])&0xFE;
	buf[6] = (~buf[5])&0xFE;
	

	//char buf[12] = {0xFF, 0xFF, 0x0C, 0xFD, 0x05, 0x00, 0x00, 0x15, 0x00, 0x04, 0xfd, 0x3c}; // move MIN command
	//char buf[12] = {0xFF, 0xFF, 0x0C, 0xFD, 0x05, 0x00, 0x00, 0xEA, 0x03, 0x04, 0xfd, 0x3c}; // move MAX command
	//char buf[10] = {0xFF, 0xFF, 0x0A, 0xFD, 0x03, 0x00, 0x00, 0x34, 0x01, 0x60}; // torque ON command
	//buf[5] = (buf[2]^buf[3]^buf[4]^buf[7]^buf[8]^buf[9])&0xFE;
	//buf[6] = (~buf[5])&0xFE;
	//buf[5] = (buf[2]^buf[3]^buf[4]^buf[7]^buf[8]^buf[9]^buf[10]^buf[11])&0xFE;
	//buf[6] = (~buf[5])&0xFE;
	//char buf[9] = {0xff, 0xff, 0x09, 0xfd, 0x04, 0xc4, 0x3a, 0x3c, 0x01};
	//buf[5] = (buf[2]^buf[3]^buf[4]^buf[7]^buf[8])&0xFE;
	//buf[6] = (~buf[5])&0xFE;

    	// UART config with termios
    	// termios is used to create general terminal interface to control asynchronous communication ports
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

	int i,a;
	for(i=0; i<sizeof(buf)/sizeof(buf[0]); i++)
		printf("buf[%d] = %x\n", i, buf[i]);
	a = write(fd, &buf, sizeof(buf));
	printf("third var is: %d\n", sizeof(buf));	

	size_t n_bytes = 1;
	char c;
	for(i=0; i<12; i++){
		read(fd,&c,sizeof(c));
		printf("0x%x ",c);
	}
	printf("\n");

	return 0;
}

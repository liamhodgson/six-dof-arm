#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>


void sendcmd(char* buf, int fd, int len)
{
	int i,a;
       	//for(i=0; i<len; i++)
        //	printf("buf[%d] = %x\n", i, buf[i]);
        a = write(fd, buf, len);
}

int main(int argc, char *argv[]){
        if(argc<2){
		printf("not enough args\n");
		return 0;
}

	int fd;
        const char uartpath[11] = "/dev/ttyO1";
        unsigned char data;
        //char buf[7] = {0xFF,0xFF,0x07,0xFD,0x07,0xFC,0x02}; // STAT command
        //char buf[7] = {0xff, 0xff, 0x07, 0xFD, 0x09, 0xf2, 0x0c}; // reboot
        //char buf[10] = {0xFF,0xFF,0x0A,0xFD,0x03,0xC0,0x3E,0x35,0x01,0x01}; // turn LED green
        //char buf[9] = {0xff, 0xff, 0x09, 0xFD, 0x08, 0xfc, 0x02, 0, 0}; // rollback
        //char buf[9] = {0xff, 0xff, 0x09, 0xfd, 0x04, 0x00, 0x00, 0x3c, 0x01}; // read position data
        //buf[5] = (buf[2]^buf[3]^buf[4]^buf[7]^buf[8])&0xFE;
        //buf[6] = (~buf[5])&0xFE;

	// set an ID to 0xFC
	char setID[10] = {0xFF,0xFF,0x0A,0xFD,0x01,0x00,0x00,0x06,0x01,0xFC};
        setID[5] = (setID[2]^setID[3]^setID[4]^setID[7]^setID[8]^setID[9])&0xFE;
        setID[6] = (~setID[5])&0xFE;

	// test move
	char posTest[12] = {0xFF,0xFF,0x0C,0xFD,0x05,0x32,0xCC,0x00,0x02,0x04,0xFD,0x3C};

	// move MIN position
        char posMin[12] = {0xFF, 0xFF, 0x0C, 0xFD, 0x05, 0x00, 0x00, 0x15, 0x00, 0x04, 0xfd, 0x3c}; // move MIN command
        posMin[5] = (posMin[2]^posMin[3]^posMin[4]^posMin[7]^posMin[8]^posMin[9]^posMin[10]^posMin[11])&0xFE;
        posMin[6] = (~posMin[5])&0xFE;
        
	// move max position
        char posMax[12] = {0xFF, 0xFF, 0x0C, 0xFD, 0x05, 0x00, 0x00, 0xEA, 0x03, 0x04, 0xfd, 0x3c}; // move MAX command
        posMax[5] = (posMax[2]^posMax[3]^posMax[4]^posMax[7]^posMax[8]^posMax[9]^posMax[10]^posMax[11])&0xFE;
        posMax[6] = (~posMax[5])&0xFE;

	// turn torque on servo id 0xFD
	char tON1[10] = {0xFF, 0xFF, 0x0A, 0xFD, 0x03, 0x00, 0x00, 0x34, 0x01, 0x60}; // torque ON command
        tON1[5] = (tON1[2]^tON1[3]^tON1[4]^tON1[7]^tON1[8]^tON1[9])&0xFE;
        tON1[6] = (~tON1[5])&0xFE;
        //char buf[9] = {0xff, 0xff, 0x09, 0xfd, 0x04, 0xc4, 0x3a, 0x3c, 0x01};
        //buf[5] = (buf[2]^buf[3]^buf[4]^buf[7]^buf[8])&0xFE;
        //buf[6] = (~buf[5])&0xFE;

	// move goal position
        int goal = atoi(argv[1]);
	if(goal<21 || goal>1002){
		printf("position goal out of range\n");
		return 0;
	}
        /*
	int goal2 = atoi(argv[2]);
	if(goal2<21 || goal2>1002){
		printf("position goal out of range\n");
		return 0;
	}*/

	char posGoal[12] = {0xFF, 0xFF, 0x0C, 0xFD, 0x05, 0x00, 0x00, goal&0x00FF, (goal&0xFF00)>>8, 0x04, 0xfd, 0x3c};
        posGoal[5] = (posGoal[2]^posGoal[3]^posGoal[4]^posGoal[7]^posGoal[8]^posGoal[9]^posGoal[10]^posGoal[11])&0xFE;
        posGoal[6] = (~posGoal[5])&0xFE;
	/*
	char posGoal2[17] = {0xFF, 0xFF, 0x0C, 0xFE, 0x05, 0x00, 0x00, goal&0x00FF, (goal&0xFF00)>>8, 0x04, 0xfd, 0x3c,goal2&0x00FF,(goal2&0xFF00)>>8,0x04,0xfc,0x3c};
        posGoal2[5] = (posGoal2[2]^posGoal2[3]^posGoal2[4]^posGoal2[7]^posGoal2[8]^posGoal2[9]^posGoal2[10]^posGoal2[11]^posGoal2[12]^posGoal2[13]^posGoal2[14]^posGoal2[15]^posGoal2[16])&0xFE;
        posGoal2[6] = (~posGoal2[5])&0xFE;
        */
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

	int delay = 1000; // ms
	//sendcmd(tON1,fd, sizeof(tON1)/sizeof(tON1[0]));
	//sendcmd(posTest,fd,sizeof(posTest)/sizeof(posTest[0]));
	//sendcmd(posGoal2,fd,sizeof(posGoal2)/sizeof(posGoal2[0]));
	//usleep(delay*1000);
	//sendcmd(posMin,fd,sizeof(posMin)/sizeof(posMin[0]));
	sendcmd(posGoal,fd,sizeof(posGoal)/sizeof(posGoal[0]));
	
	//usleep(delay*1000);
	//sendcmd(posMax,fd, sizeof(posMax)/sizeof(posMax[0]));
        
	/*
	size_t n_bytes = 1;
        char c;
        for(i=0; i<12; i++){
                read(fd,&c,sizeof(c));
                printf("0x%x ",c);
        }
        printf("\n");
	*/

        return 0;
}


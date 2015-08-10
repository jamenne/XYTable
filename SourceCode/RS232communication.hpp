#ifndef RS232_COMMUNICATION_H
#define RS232_COMMUNICATION_H

#define INVALID_HANDLE_VALUE -1
#define BAUDRATE B9600

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

struct termios oldtio,newtio;

//Opcodes of all TMCL commands that can be used in direct mode
#define TMCL_ROR 1
#define TMCL_ROL 2
#define TMCL_MST 3
#define TMCL_MVP 4
#define TMCL_SAP 5
#define TMCL_GAP 6
#define TMCL_STAP 7
#define TMCL_RSAP 8
#define TMCL_SGP 9
#define TMCL_GGP 10
#define TMCL_STGP 11
#define TMCL_RSGP 12
#define TMCL_RFS 13
#define TMCL_SIO 14
#define TMCL_GIO 15
#define TMCL_SCO 30
#define TMCL_GCO 31
#define TMCL_CCO 32

//Opcodes of TMCL control functions (to be used to run or abort a TMCL program in the module)
#define TMCL_APPL_STOP 128
#define TMCL_APPL_RUN 129
#define TMCL_APPL_RESET 131

//Options for MVP commands
#define MVP_ABS 0
#define MVP_REL 1
#define MVP_COORD 2

//Options for RFS command
#define RFS_START 0
#define RFS_STOP 1
#define RFS_STATUS 2

#define FALSE 0
#define TRUE 1

//Result codes for GetResult
#define TMCL_RESULT_OK 0
#define TMCL_RESULT_NOT_READY 1
#define TMCL_RESULT_CHECKSUM_ERROR 2

int number_of_comports(){
	char search[50],output[2];
	sprintf(search,"ls /dev/ttyUSB* | wc -w"); // get word count of list of newtioUSB* devices
	FILE* pipe = popen(search, "r");
	if (!pipe) perror("popen()");
	fscanf(pipe,"%s", output);
	pclose(pipe);
	string output1 = output; // convert char array to string
	istringstream buf(output1);
	int n;
	buf >> n; // convert string to int
	return n;
}

//Usage: ComHandle=OpenRS232(ComNumber, CBR_9600) Baudrate statically set to 9600
int OpenRS232(int ComNumber, long BaudRate){
	int ComHandle;
	char cmd[50], ComName[20];
	// get the name of COM device(ComNumber) on USB-port with ls
	  sprintf(cmd,"ls /dev/ttyUSB* | awk 'NR==%d'",ComNumber);
	  FILE* pipe = popen(cmd, "r");
	  if(!pipe) perror("popen() failed");
	  fscanf(pipe,"%s",ComName);
	  //cout << ComName << " connected." << endl;
	  pclose(pipe);
	  //cout << "ComName: " << ComName << endl;
	  ComHandle = open(ComName, O_RDWR | O_NOCTTY);// | O_NONBLOCK);
	  //cout << "ComHandle: " << ComHandle << endl;
	  if(ComHandle<0) return INVALID_HANDLE_VALUE;
	  else
	  {
		tcgetattr(ComHandle ,&oldtio); /* save current serial port settings */
		//bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */
		memset (&newtio, 0, sizeof(newtio));

		cfsetospeed (&newtio, (speed_t)B9600);
		cfsetispeed (&newtio, (speed_t)B9600);

		/* Setting other Port Stuff */
		newtio.c_cflag     &=  ~PARENB;        // Make 8n1
		newtio.c_cflag     &=  ~CSTOPB;
		newtio.c_cflag     &=  ~CSIZE;
		newtio.c_cflag     |=  CS8;

		newtio.c_cflag     &=  ~CRTSCTS;       // no flow control
		newtio.c_cc[VMIN]      =   1;                  // read block (characters)
		newtio.c_cc[VTIME]     =   8;                  // n*100msec read timeout
		newtio.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
		fcntl(ComHandle, F_SETFL, FNDELAY);
        /*
          BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
          CRTSCTS : output hardware flow control (only used if the cable has
                    all necessary lines. See sect. 7 of Serial-HOWTO)
          CS8     : 8n1 (8bit,no parity,1 stopbit)
          CLOCAL  : local connection, no modem contol
          CREAD   : enable receiving characters

         newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

          IGNPAR  : ignore bytes with parity errors
          ICRNL   : map CR to NL (otherwise a CR input on the other computer
                    will not terminate input)
          otherwise make device raw (no other input processing)

         newtio.c_iflag = IGNPAR | ICRNL;

         Raw output.

         newtio.c_oflag = 0;
					//set input mode (non-canonical, no echo...)
         newtio.c_lflag = 0;

          ICANON  : enable canonical input
          disable all echo functionality, and don't send signals to calling program

         //newtio.c_lflag = ICANON;

          initialize all control characters
          default values can be found in /usr/include/termios.h, and are given
          in the comments, but we don't need them here

         newtio.c_cc[VTIME]    = 5;     // inter-character timer 100ms
         newtio.c_cc[VMIN]     = 1;     // blocking read until n character arrives

          now clean the modem line and activate the settings for the port
        */
			cfmakeraw(&newtio);
			tcflush(ComHandle, TCIFLUSH);
			tcsetattr(ComHandle ,TCSANOW, &newtio);

	return ComHandle;
  }
}


//Close the serial port
//Usage: CloseRS232(ComHandle);
void CloseRS232(int Handle)
{
	close(Handle);
}


//Send a binary TMCL command
//e.g.  SendCmd(ComHandle, 1, TMCL_MVP, MVP_ABS, 1, 50000);   will be MVP ABS, 1, 50000 for a module with address 1
//Parameters: Handle: Handle of the serial port (returned by OpenRS232).
//            Address: address of the module (factory default is 1).
//            Command: the TMCL command (see the constants at the begiining of this file)
//            Type:    the "Type" parameter of the TMCL command (set to 0 if unused)
//            Motor:   the motor number (set to 0 if unused)
//            Value:   the "Value" parameter (depending on the command, set to 0 if unused)
void SendCmd(int Handle, unsigned char Address, unsigned char Command, unsigned char Type, unsigned char Motor, int Value)
{
	unsigned char TxBuffer[9];
	int i;

	TxBuffer[0]=Address;
	TxBuffer[1]=Command;
	TxBuffer[2]=Type;
	TxBuffer[3]=Motor;
	TxBuffer[4]=Value >> 24;
	TxBuffer[5]=Value >> 16;
	TxBuffer[6]=Value >> 8;
	TxBuffer[7]=Value & 0xff;
	TxBuffer[8]=0;
	for(i=0; i<8; i++)
		TxBuffer[8]+=TxBuffer[i];

	//Send the datagram
	write(Handle, TxBuffer, 9);
}

//Read the result that is returned by the module
//Parameters: Handle: handle of the serial port, as returned by OpenRS232
//            Address: pointer to variable to hold the reply address returned by the module
//            Status: pointer to variable to hold the status returned by the module (100 means okay)
//            Value: pointer to variable to hold the value returned by the module
//Return value: TMCL_RESULT_OK: result has been read without errors
//              TMCL_RESULT_NOT_READY: not enough bytes read so far (try again)
//              TMCL_RESULT_CHECKSUM_ERROR: checksum of reply packet wrong
unsigned char GetResult(int Handle, unsigned char *Address, unsigned char *Status, int *Value)
{
	unsigned char RxBuffer[9], Checksum;
	int i,byte=1,nbytes=0,nerror=0;

	while(nbytes < 9){
		byte = read(Handle, &RxBuffer[nbytes], 1);
		if (byte > 0) nbytes++;
		else nerror++;
		if(nerror > 9) break;
	}
//	cout << "read() loop returned " << nbytes << endl;
	Checksum=0;
	for(i=0; i<8; i++)
		Checksum+=RxBuffer[i];

	if(Checksum!=RxBuffer[8]) return -1;

	*Address=RxBuffer[0];
	*Status=RxBuffer[2];
	*Value=(RxBuffer[4] << 24) | (RxBuffer[5] << 16) | (RxBuffer[6] << 8) | RxBuffer[7];

	return nbytes;
}

#endif
#ifndef RS232_COMMUNICATION_H
#define RS232_COMMUNICATION_H

int number_of_comports();

//Usage: ComHandle=OpenRS232(ComNumber, CBR_9600) Baudrate statically set to 9600
int OpenRS232(int ComNumber, long BaudRate);

int OpenRS232_dev(const char* DevName, long BaudRate);


//Close the serial port
//Usage: CloseRS232(ComHandle);
void CloseRS232(int Handle);


#endif
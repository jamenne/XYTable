#include "RS232communication.h"
#include "MotorControl.h"
#include <fstream>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sstream>
#include <ctime>
#include <vector>
#include <poll.h>
#include <errno.h>
#include <fcntl.h>

using namespace std;

void MotorControl::CheckComports(){

	int Motor_i2c;
	Motor_i2c = 0; // always 0 for RS232 single axis motors

	int n=number_of_comports();
	switch(n){
		case 0: cerr << "no comports present."; break;
		case 1: cout << "1 comport present." << endl; break;
		default: cout << n << " comports present." << endl; break;
	}

}

void MotorControl::ConnectMotor(bool verbosity, vector<int> &Motor){

	int n=number_of_comports();
	unsigned char Address, Status;
	int Value;

	vector<int> Motor_handle(n);
	for (int a=0;a<n;a++){
		Motor_handle[a] = OpenRS232(a+1, 9600); // Baudrate=9600 is defined in RS232communication.hpp, this parameter has no effect
		//cout << a << Motor_handle[a] << endl;
		if (Motor_handle[a] >= 0){
			bool connected = true;
			SendCmd(Motor_handle[a], 1, TMCL_GAP, 0, 0, 0); // just try to speak with motor, used GAP to not change the position
			sleep(1);
			Address = 0; Status = 0; Value = 0;


			//if (GetResult(Motor_handle[a], &Address, &Status, &Value)==TMCL_RESULT_OK) break;
			GetResult(Motor_handle[a], &Address, &Status, &Value);
				if (verbosity) cout << "errno = " << errno << ":\t" << strerror(errno) << endl;
					if (errno != 0 ){
						connected = false;
						if (verbosity) cout << "com device " << a+1 << " did not respond" << endl;
					}
				else{
					Motor.push_back(Motor_handle[a]);
					//cout << Motor_handle[a] << endl;
					cout << "Motor " << a+1 << " OK." << endl;
				}
		}
	}

	switch(Motor.size()){
		case 0: {cerr << "No Motor detected - Please check RS232 connections and power plugs. Assuming Motor at COM1." << endl; Motor.push_back(Motor_handle[1]); break;} //return -1;}
		case 1: cout << "1 Motor detected." << endl; break;
		default: cout << Motor.size() << " Motors detected." << endl; break;
	}

	//Enable end switches 
	SendCmd(Motor[1], 1, TMCL_SAP, 12, 0, 0);
	GetResult(Motor[1], &Address, &Status, &Value);
	SendCmd(Motor[1], 1, TMCL_SAP, 13, 0, 0);
	GetResult(Motor[1], &Address, &Status, &Value);
	SendCmd(Motor[0], 1, TMCL_SAP, 12, 0, 0);
	GetResult(Motor[0], &Address, &Status, &Value);
	SendCmd(Motor[0], 1, TMCL_SAP, 13, 0, 0);
	GetResult(Motor[0], &Address, &Status, &Value);

}

void MotorControl::ReferenceRunX(vector<int> &Motor, unsigned char Address, unsigned char Status, int Value){
	//Parameters maybe have to be adjusted

	//Settings for the x-axis
	//Enable end switches 
	SendCmd(Motor[0], 1, TMCL_SAP, 12, 0, 0);
	GetResult(Motor[0], &Address, &Status, &Value);
	SendCmd(Motor[0], 1, TMCL_SAP, 13, 0, 0);
	GetResult(Motor[0], &Address, &Status, &Value);
	//Referencing search mode
	//1 – Only the left reference switch is searched.
	//2 – The right switch is searched, then the left switch is searched.
	//3 – Three-switch-mode: the right switch is searched first, then the reference switch will be searched.
	//SendCmd(Motor[0], 1, TMCL_SAP, 193, 0, 1);
	//GetResult(Motor[0], &Address, &Status, &Value);
	//Max. current; TMC109 max. 255 
	SendCmd(Motor[0], 1, TMCL_SAP, 6, 0, 200);
	GetResult(Motor[0], &Address, &Status, &Value);
	//Max. acceleration
	SendCmd(Motor[0], 1, TMCL_SAP, 5, 0, 5);
	GetResult(Motor[0], &Address, &Status, &Value);
	//Max. speed
	SendCmd(Motor[0], 1, TMCL_SAP, 4, 0, 300);
	GetResult(Motor[0], &Address, &Status, &Value);
	//Min. speed
	//SendCmd(Motor[0], 1, TMCL_SAP, 130, 0, 1);
	//GetResult(Motor[0], &Address, &Status, &Value);
	//Disable soft stop, the motor will stop immediately (disregarding motor limits), when the reference or limit switch is hit.
	SendCmd(Motor[0], 1, TMCL_SAP, 149, 0, 0);
	GetResult(Motor[0], &Address, &Status, &Value);
	//Referencing search speed (0-full speed, 1-half of max.,...)
	SendCmd(Motor[0], 1, TMCL_SAP, 194, 0, 0);
	GetResult(Motor[0], &Address, &Status, &Value);
	//Reference switch speed (0-8), the speed for the switching point calibration can be selected
	//SendCmd(Motor[0], 1, TMCL_SAP, 195, 0, 8);
	//GetResult(Motor[0], &Address, &Status, &Value);
	sleep(1);
	

	//Reference Run
	SendCmd(Motor[0], 1, TMCL_RFS, RFS_START, 0, 0);
	GetResult(Motor[0], &Address, &Status, &Value);

	sleep(90);

	//Value = 6; // should not be zero

	/*while(Value != 0){

		SendCmd(Motor[1], 1, TMCL_RFS, RFS_STATUS, 0, 0);
		GetResult(Motor[1], &Address, &Status, &Value);
		cout << "Value: " << Value  << endl;
	}*/
	

	//Set zero position
	SendCmd(Motor[0], 1, TMCL_MST, 0, 0, 0);
	GetResult(Motor[0], &Address, &Status, &Value);
	SendCmd(Motor[0], 1, TMCL_SAP, 0, 0, 0);
	GetResult(Motor[0], &Address, &Status, &Value);
	SendCmd(Motor[0], 1, TMCL_SAP, 1, 0, 0);
	GetResult(Motor[0], &Address, &Status, &Value);


}

void MotorControl::ReferenceRunY(vector<int> &Motor, unsigned char Address, unsigned char Status, int Value){

	//Parameters maybe have to be adjusted

	//Settings for the y-axis
	//Enable end switches 
	SendCmd(Motor[1], 1, TMCL_SAP, 12, 0, 0);
	GetResult(Motor[1], &Address, &Status, &Value);
	SendCmd(Motor[1], 1, TMCL_SAP, 13, 0, 0);
	GetResult(Motor[1], &Address, &Status, &Value);
	//Referencing search mode
	//1 – Only the left reference switch is searched.
	//2 – The right switch is searched, then the left switch is searched.
	//3 – Three-switch-mode: the right switch is searched first, then the reference switch will be searched.
	//SendCmd(Motor[1], 1, TMCL_SAP, 193, 0, 1);
	//GetResult(Motor[1], &Address, &Status, &Value);
	//Max. current; Max. 1500
	SendCmd(Motor[1], 1, TMCL_SAP, 6, 0, 900);
	GetResult(Motor[1], &Address, &Status, &Value);
	//Max. acceleration
	SendCmd(Motor[1], 1, TMCL_SAP, 5, 0, 3);
	GetResult(Motor[1], &Address, &Status, &Value);
	//Max. speed
	SendCmd(Motor[1], 1, TMCL_SAP, 4, 0, 200);
	GetResult(Motor[1], &Address, &Status, &Value);
	//Min. speed
	//SendCmd(Motor[1], 1, TMCL_SAP, 130, 0, 1);
	//GetResult(Motor[1], &Address, &Status, &Value);
	//Referencing search speed (0-full speed, 1-half of max.,...)
	SendCmd(Motor[1], 1, TMCL_SAP, 194, 0, 0);
	GetResult(Motor[1], &Address, &Status, &Value);
	//Disable soft stop
	SendCmd(Motor[1], 1, TMCL_SAP, 149, 0, 0);
	GetResult(Motor[1], &Address, &Status, &Value);
	//Reference switch speed (0-8), the speed for the switching point calibration can be selected
	//SendCmd(Motor[1], 1, TMCL_SAP, 195, 0, 8);
	//GetResult(Motor[1], &Address, &Status, &Value);
	sleep(1);


	//Reference Run
	SendCmd(Motor[1], 1, TMCL_RFS, RFS_START, 0, 0);
	GetResult(Motor[1], &Address, &Status, &Value);

	sleep(20);
	/*Value = 6; // should not be zero

	while(Value != 0){

		SendCmd(Motor[1], 1, TMCL_RFS, RFS_STATUS, 0, 0);
		GetResult(Motor[1], &Address, &Status, &Value);
		cout << "Value: " << Value  << endl;
	}*/
	
	//Set zero position
	SendCmd(Motor[1], 1, TMCL_MST, 0, 0, 0);
	GetResult(Motor[1], &Address, &Status, &Value);
	SendCmd(Motor[1], 1, TMCL_SAP, 0, 0, 0);
	GetResult(Motor[1], &Address, &Status, &Value);
	SendCmd(Motor[1], 1, TMCL_SAP, 1, 0, 0);
	GetResult(Motor[1], &Address, &Status, &Value);

}

void MotorControl::MoveRelative(vector<int> &Motor, string xy, int pos, unsigned char Address, unsigned char Status, int Value, int speed){

	if (xy=="x" || xy=="X"){

		SendCmd(Motor[0], 1, TMCL_SAP, 4, 0, speed);		
		SendCmd(Motor[0], 1, TMCL_MVP, MVP_REL, 0, pos);
		GetResult(Motor[0], &Address, &Status, &Value);
		sleep(4);
	}

	if (xy=="y" || xy =="Y")
	{
		SendCmd(Motor[1], 1, TMCL_SAP, 4, 0, speed);		
		SendCmd(Motor[1], 1, TMCL_MVP, MVP_REL, 0, pos);
		GetResult(Motor[1], &Address, &Status, &Value);
		sleep(4);
	}

	if(xy != "y" && xy != "Y" && xy != "x" && xy != "X"){
		cout << "Wrong parameter for xy used" << endl;
	}
}

void MotorControl::MoveAbsolute(vector<int> &Motor, string xy, int pos, unsigned char Address, unsigned char Status, int Value, int speed){


	if (xy=="x" || xy=="X")
	{
		SendCmd(Motor[0], 1, TMCL_SAP, 4, 0, speed);			
		SendCmd(Motor[0], 1, TMCL_MVP, MVP_ABS, 0, pos);
		GetResult(Motor[0], &Address, &Status, &Value);
		sleep(4);
	}

	if (xy=="y" || xy =="Y")
	{
		SendCmd(Motor[1], 1, TMCL_SAP, 4, 0, speed);			
		SendCmd(Motor[1], 1, TMCL_MVP, MVP_ABS, 0, pos);
		GetResult(Motor[1], &Address, &Status, &Value);
		sleep(4);
	}

	if(xy != "y" && xy != "Y" && xy != "x" && xy != "X"){
		cout << "Wrong parameter for xy used" << endl;	
	} 
}

int MotorControl::CalcStepsX(double pos){ // returns number of steps for a given distance in mm for the Motor on the x axis
	// For x axis: 12800 steps = 140mm -> 91,4285714steps = 1mm
	// round down to int -> +0.5
	
	return (int)(pos * 91.4285714 + 0.5);
}

int MotorControl::CalcStepsY(double pos){ // returns number of steps for a given distance in mm for the Motor on the y axis
	// For y axis: 12800 steps = 85mm -> 150,588235 = 1mm
	// round down to int -> +0.5

	return (int)(pos * 150.588235 + 0.5);
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


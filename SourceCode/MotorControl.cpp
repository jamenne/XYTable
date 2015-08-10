#include "RS232communication.hpp"
#include "MotorControl.h"
#include <fstream>
#include <string>
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

void MotorControl::ConnectMotor(bool verbosity, vector<int> Motor){

	int n=number_of_comports();
	unsigned char Address, Status;
	int Value;

	vector<int> Motor_handle(n);
	for (int a=0;a<n;a++){
		Motor_handle[a] = OpenRS232(a+1, 9600); // Baudrate=9600 is defined in RS232communication.hpp, this parameter has no effect
		//cout << a << Motor_handle[a] << endl;
		if (Motor_handle[a] >= 0){
			bool connected = true;
			SendCmd(Motor_handle[a], 1, TMCL_MVP, 0, 0, 0);
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
					cout << Motor_handle[a] << endl;
					cout << "Motor " << a+1 << " OK." << endl;
				}
		}
	}

	switch(Motor.size()){
		case 0: {cerr << "No Motor detected - Please check RS232 connections and power plugs. Assuming Motor at COM1." << endl; Motor.push_back(Motor_handle[1]); break;} //return -1;}
		case 1: cout << "1 Motor detected." << endl; break;
		default: cout << Motor.size() << " Motors detected." << endl; break;
	}


}

void MotorControl::ReferenceRunX(int Motor, unsigned char Address, unsigned char Status, int Value){

	//Parameters maybe have to be adjusted

	//Settings for the x-axis
	//Enable end switches 
	SendCmd(Motor, 1, TMCL_SAP, 12, 0, 0);
	GetResult(Motor, &Address, &Status, &Value);
	SendCmd(Motor, 1, TMCL_SAP, 13, 0, 0);
	GetResult(Motor, &Address, &Status, &Value);
	//Referencing search mode
	//1 – Only the left reference switch is searched.
	//2 – The right switch is searched, then the left switch is searched.
	//3 – Three-switch-mode: the right switch is searched first, then the reference switch will be searched.
	SendCmd(Motor, 1, TMCL_SAP, 193, 0, 2);
	GetResult(Motor, &Address, &Status, &Value);
	//Max. current; TMC109 max. 255 
	SendCmd(Motor, 1, TMCL_SAP, 6, 0, 200);
	GetResult(Motor, &Address, &Status, &Value);
	//Max. acceleration
	SendCmd(Motor, 1, TMCL_SAP, 5, 0, 5);
	GetResult(Motor, &Address, &Status, &Value);
	//Max. speed
	SendCmd(Motor, 1, TMCL_SAP, 4, 0, 200);
	GetResult(Motor, &Address, &Status, &Value);
	//Min. speed
	SendCmd(Motor, 1, TMCL_SAP, 130, 0, 1);
	GetResult(Motor, &Address, &Status, &Value);
	//Disable soft stop, the motor will stop immediately (disregarding motor limits), when the reference or limit switch is hit.
	SendCmd(Motor, 1, TMCL_SAP, 149, 0, 0);
	GetResult(Motor, &Address, &Status, &Value);
	//Referencing search speed (0-full speed, 1-half of max.,...)
	SendCmd(Motor, 1, TMCL_SAP, 194, 0, 0);
	GetResult(Motor, &Address, &Status, &Value);
	//Reference switch speed (0-8), the speed for the switching point calibration can be selected
	SendCmd(Motor, 1, TMCL_SAP, 195, 0, 8);
	GetResult(Motor, &Address, &Status, &Value);
	sleep(1);


	//Reference Run
	SendCmd(Motor, 1, TMCL_RFS, RFS_START, 0, 0);
	GetResult(Motor, &Address, &Status, &Value);


}

void MotorControl::ReferenceRunY(int Motor, unsigned char Address, unsigned char Status, int Value){

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
	SendCmd(Motor[1], 1, TMCL_SAP, 193, 0, 2);
	GetResult(Motor[1], &Address, &Status, &Value);
	//Max. current; Max. 1500
	SendCmd(Motor[1], 1, TMCL_SAP, 6, 0, 900);
	GetResult(Motor[1], &Address, &Status, &Value);
	//Max. acceleration
	SendCmd(Motor[1], 1, TMCL_SAP, 5, 0, 3);
	GetResult(Motor[1], &Address, &Status, &Value);
	//Max. speed
	SendCmd(Motor[1], 1, TMCL_SAP, 4, 0, 100);
	GetResult(Motor[1], &Address, &Status, &Value);
	//Min. speed
	SendCmd(Motor[1], 1, TMCL_SAP, 130, 0, 1);
	GetResult(Motor[1], &Address, &Status, &Value);
	//Referencing search speed (0-full speed, 1-half of max.,...)
	SendCmd(Motor[1], 1, TMCL_SAP, 194, 0, 0);
	GetResult(Motor[1], &Address, &Status, &Value);
	//Disable soft stop
	SendCmd(Motor[1], 1, TMCL_SAP, 149, 0, 0);
	GetResult(Motor[1], &Address, &Status, &Value);
	//Reference switch speed (0-8), the speed for the switching point calibration can be selected
	SendCmd(Motor[1], 1, TMCL_SAP, 195, 0, 8);
	GetResult(Motor[1], &Address, &Status, &Value);
	sleep(1);


	//Reference Run
	SendCmd(Motor[1], 1, TMCL_RFS, RFS_START, 0, 0);
	GetResult(Motor[1], &Address, &Status, &Value);

}

void MotorControl::MoveRelative(int Motor, int pos, unsigned char Address, unsigned char Status, int Value, int speed){

	SendCmd(Motor, 1, TMCL_SAP, 4, 0, speed);		
	SendCmd(Motor, 1, TMCL_MVP, MVP_REL, 0, pos);
	GetResult(Motor, &Address, &Status, &Value);
	sleep(4);
}

void MotorControl::MoveAbsolute(int Motor, int pos, unsigned char Address, unsigned char Status, int Value, int speed){

	SendCmd(Motor, 1, TMCL_SAP, 4, 0, speed);			
	SendCmd(Motor, 1, TMCL_MVP, MVP_ABS, 0, pos);
	GetResult(Motor, &Address, &Status, &Value);
	sleep(4);
}






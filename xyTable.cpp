//	xyTable.cpp to use the xyTable
//
//	Created by Janine Müller on 07.03.2017
//
//
//
//
//

// xyTable
#include "xyTable.h"
// Motor Control
#include "../MotorControl/Motor.h"
// Spectrometer
#include "../SpectrometerClass/Spectrometer.h"
// Spectrometer Measurements
#include "../SpectrometerClass/SpecMeasurement/SpecMeasurement.h"
// LED
#include "../LEDClass/LED.h"

#include <iostream>
#include <unistd.h>

using namespace std;

xyTable::xyTable()
{
	this->_y_StartPosition = 0; // in mm, used to calculate no of steps for the motor
	this->_y_Dis = 0; // distance between two measurements
	this->_y_NumbOfMeas = 0;
	//x axis
	this->_x_StartPosition = 0; // in mm, used to calculate no of steps for the motor
	this->_x_Dis = 0; // distance between two measurements
	this->_x_NumbOfMeas = 0;

	this->_yaxis = false;


}

double xyTable::Get_y_StartPosition(){
	return this->_y_StartPosition;
}

double xyTable::Get_x_StartPosition(){
	return this->_x_StartPosition;
}

double xyTable::Get_y_Dis(){
	return this->_y_Dis;
}

double xyTable::Get_x_Dis(){
	return this->_x_Dis;
}

int xyTable::Get_y_NumbOfMeas(){
	return this->_y_NumbOfMeas;
}

int xyTable::Get_x_NumbOfMeas(){
	return this->_x_NumbOfMeas;
}

bool xyTable::Get_Usage_Yaxis(){
	return this->_yaxis;
}

bool xyTable::Set_Usage_Yaxis(bool usageY){
	this->_yaxis = usageY;
	return true;
}

bool xyTable::Set_y_StartPosition(double ystart){
	if(ystart > 0 && ystart <= 190){
		this->_y_StartPosition = ystart;	
		return true;
	}

	else{
		cout << "y Start position needs to be > 0 and < 100mm" << endl;
		return false;
	}
}

bool xyTable::Set_x_StartPosition(double xstart){
	if(xstart > 0 && xstart < 2500){
		this->_x_StartPosition = xstart;	
		return true;
	}

	else{
		cout << "x Start position needs to be > 0 and < 2500mm" << endl;
		return false;
	}
}

bool xyTable::Set_x_Dis(double xDis){
	if(xDis > 0 && xDis < 2500){
		this->_x_Dis = xDis;	
		return true;
	}

	else{
		cout << "x distance needs to be > 0 and < 2500mm" << endl;
		return false;
	}
}

bool xyTable::Set_y_Dis(double yDis){
	if(yDis > 0 && yDis < 100){
		this->_y_Dis = yDis;	
		return true;
	}

	else{
		cout << "y distance needs to be > 0 and < 100mm" << endl;
		return false;
	}
}

bool xyTable::Set_y_NumbOfMeas(int yNumMeas){
	if(yNumMeas > 0){
		this->_y_NumbOfMeas = yNumMeas;	
		return true;
	}

	else{
		cout << "Number of Measurements in y needs to be > 0" << endl;
		return false;
	}
}

bool xyTable::Set_x_NumbOfMeas(int xNumMeas){
	if(xNumMeas > 0){
		this->_x_NumbOfMeas = xNumMeas;
		return true;
	}

	else{
		cout << "Number of Measurements in x needs to be > 0" << endl;
		return false;
	}
}

void xyTable::xyTableMeasurementOnlyXAxis(Motor *Mot){

	Spectrometer *ham = new Spectrometer();
	cout << "Spectrometer created!" << endl;

	LED *led = new LED();
	cout << "LED created!" << endl;

	SpecMeasurement *SpecMeas = new SpecMeasurement();

	double current1, current2, current3;

	char input;
	bool ThreeLightMeas = false;

	cout << "Do you want to take a 3 light measurement? (Y/N)" << endl;
	cin >> input;

	if((input=='y') | (input=='Y')){

		ThreeLightMeas = true;

		cout << "Which currents you would like to use? (current1, current2, current3 in mA)" << endl;
		cout << "Enter 1st current..." << endl;
		cin >> current1;
		cout << "Enter 2nd current..." << endl;
		cin >> current2;
		cout << "Enter 3rd current..." << endl;
		cin >> current3;

	}

	else if((input=='n') | (input=='N')){

		ThreeLightMeas = false;

		cout << "Which current you would like to use? (current in mA)" << endl;
		cin >> current1;

	}

	else{
		cout << "You should have entered N or Y!" << endl;
		exit(EXIT_FAILURE);
	}
	
	int IntTime;
	cout << "Which integration time you would like to use for the Spectrometer? (10000-10000000µs)" << endl;
	cin >> IntTime;

	int NumberOfAverages;
	cout << "How many averages would you like to take?" << endl;
	cin >> NumberOfAverages;

	//********** Start of Measurement **********//

	char temp;
	stringstream path;

	cout << "Press any key to Start Measurement!" << endl;
	cin >> temp;

	cout << "Now you have 40 seconds to leave the room!" << endl;
	sleep(30);
	cout << "Ten secounds..." << endl;
	sleep(10);

	cout << "Starting measurements..." << endl;
	double posx=0;

	for(int i = 1; i<=this->Get_x_NumbOfMeas(); i++){

		cout << "Current x position: " << posx << "mm" << endl;

		path << "/home/xytable/data/Spectrometer/Spectrum_x=" << posx << "mm.txt";

		//********** Spectrometer Measurement **********//

		if(SpecMeas->SetNumbOfAv(NumberOfAverages) && ham->SetIntegrationTime(IntTime)){

			if(ThreeLightMeas==true){

				SpecMeas->Measurement3LWithDC(ham, led, current1, current2, current3, path.str());
				path.str("");
				
			}

			else if(ThreeLightMeas==false){

				led->SetCurr(current1);
				SpecMeas->SingleMeasurementWithDC(ham, led, path.str());
				path.str("");
				
			}


			else exit(EXIT_FAILURE);
		}

		else exit(EXIT_FAILURE);

		//*******************************************//

		//********** Move Motor to next Position **********//
		posx=i*this->Get_x_Dis();

		if(posx < this->Get_x_NumbOfMeas()*this->Get_x_Dis()){

			Mot->MoveRelative("x", Mot->CalcStepsX(this->Get_x_Dis()));
			
			// Wait 2 second to let Motor move //
			sleep(2);
		}

	}



}

void xyTable::xyTableMeasurementBothAxis(Motor *Mot){

	
}
//	xyTable.cpp to use the xyTable
//
//	Created by Janine Müller on 07.03.2017
//
//
//
//
//

#include "xyTable.h"
#include <iostream>

using namespace std;

xyTable::xyTable(){
	this->_y_StartPosition = 0; // in mm, used to calculate no of steps for the motor
	this->_y_Dis = 0; // distance between two measurements
	this->_y_NumbOfMeas = 0;
	//x axis
	this->_x_StartPosition = 0; // in mm, used to calculate no of steps for the motor
	this->_x_Dis = 0; // distance between two measurements
	this->_x_NumbOfMeas = 0;

	this->_yaxis = false;

}

bool xyTable::Set_y_StartPosition(double ystart){
	if(ystart > 0 && ystart < 100){
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

void xyTable::xyTableMeasurementOnlyXAxis(){


}

void xyTable::xyTableMeasurementBothAxis(){

	
}
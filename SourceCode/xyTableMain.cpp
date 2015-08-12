//
//
//
//
//

//Restore factory settings; Nur im Notfall benutzen
//SendCmd(Motor_handle[0], 1, 137, 0, 0, 1234); 	
//SendCmd(Motor_handle[1], 1, 137, 0, 0, 1234);


#include "MotorControl.h"
#include "/home/laborlinux/src/SpectrometerClass/Spectrometer.h"
#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[])
{
	bool verbosity=false;
	int arg=1;
	vector<int> Motor(0);

	char input;

	unsigned char Address, Status;
	int Value;

	// Excution with -v, function checks how many comports are available
	for (arg=1;arg<argc;arg++){
		//cout << arg << " of " << argc << " parameters parsed. argv= " << argv[arg] << endl;
		//if (arg + 1 != argc) cout << "last parameter reached"<< endl;
		if (string(argv[arg]) == "-v"){
			verbosity = true;
		}
	}

	if (verbosity)
		{
			MotorControl::CheckComports();
		}	


	
	// Connect motors, motors are saved in an array	
	MotorControl::ConnectMotor(verbosity, Motor);

	
	cout << "Do you want to do a reference run? [y/n]" << endl;
	cin >> input;

	if((input=='y') | (input=='Y')){

		cout << "Starting reference run on x axis!" << endl;
		sleep(2);
		MotorControl::ReferenceRunX(Motor, Address, Status, Value);
		cout << "Starting reference run on y axis!" << endl;
		sleep(2);
		MotorControl::ReferenceRunY(Motor, Address, Status, Value);

		cout << "Finished reference run!" << endl;

	}

	//Fix parameters
	//y axis
	int y_StartPosition = 0;
	int y_Step = 0;
	int y_NumbOfSteps = 0;
	//x axis
	int x_StartPosition = 0;
	int x_Step = 0;
	int x_NumbOfSteps = 0;


	cout << "What is the start position (in mm) on the x axis?" << endl;
	cin >> x_StartPosition;
	x_StartPosition = MotorControl::CalcStepsX(x_StartPosition);

	cout << "What is the distance (in mm) between two measurements on the x axis?" << endl;
	cin >> x_Step;
	x_Step = MotorControl::CalcStepsX(x_Step);

	cout << "How many measurements you would like to take on the x axis?" << endl;
	cin >> x_NumbOfSteps;

	cout << "What is the start position (in mm) on the y axis?" << endl;
	cin >> y_StartPosition;
	y_StartPosition = MotorControl::CalcStepsY(y_StartPosition);

	cout << "What is the distance (in mm) between two measurements on the y axis?" << endl;
	cin >> y_Step;
	y_Step = MotorControl::CalcStepsY(y_Step);

	cout << "How many measurements you would like to take on the y axis?" << endl;
	cin >> y_NumbOfSteps;


	//Go to start position
	cout << "Going to start position..." << endl;
	// x axis
	MotorControl::MoveAbsolute(Motor, "x", x_StartPosition, Address, Status, Value);
	//y axis
	MotorControl::MoveAbsolute(Motor, "y", y_StartPosition, Address, Status, Value);

	cout << "Starting measurements..." << endl;

	


	
	return 0;	
}	

























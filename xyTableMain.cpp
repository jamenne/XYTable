//	Main to use the xyTable
//
//	Created by Janine Müller on 30.07.2015
//
//
//
//
//

//Restore factory settings; Nur im Notfall benutzen
//SendCmd(Motor_handle[0], 1, 137, 0, 0, 1234); 	
//SendCmd(Motor_handle[1], 1, 137, 0, 0, 1234);

// Motor Control
#include "../MotorControl/Motor.h"
// Spectrometer
#include "../SpectrometerClass/Spectrometer.h"
// Spectrometer Measurements
#include "../SpectrometerClass/SpecMeasurement/SpecMeasurement.h"
// LED
#include "../LEDClass/LED.h"
// xyTable
#include "xyTable.h"


#include <iostream>
#include <unistd.h>
#include <limits>
#include <errno.h>
#include <signal.h>


using namespace std;

// To do an action when somebody press Ctrl+C or if there is an error than interrupt the running of the code
void Error_handler(int sig) 
{
	cout << "" << endl;
	cout << "" << endl;
	cout << "It was terminated by a signal" << endl;

    // To print why it was terminated
    switch(sig) {
    	case SIGINT:
    		printf("Interrupt from keyboard; interactive attention signal.");
    		break;
    	case SIGTERM:
    		printf("Termination request.");
    		break;
    	case SIGABRT:
    		printf("Abnormal termination; abort signal from abort(3).");
    		break;
    	case SIGSEGV:
    		printf("„Segmentation violation“: invalid memory reference.");
    		break; 
    	default:
    		printf("Signal unknown"); 
    		break; 		
    }
    cout << "" << endl;
    cout << "" << endl;

    exit(1); // To shut down the program
}


int main(int argc, char* argv[])
{
	signal (SIGINT, Error_handler); 	// If the program is shut down using CTRL+C
	signal (SIGTERM, Error_handler); 	// If the program is shut down because of thermination request (kill)
	signal (SIGABRT, Error_handler); 	// If the program is shut down because of abnormal termination (?)
	signal (SIGSEGV, Error_handler); 	// If the program is shut down because of segmentation violation (the usual reason)

	unsigned char Address = 0;
	unsigned char Status = 0;
	int Value = 0;

	Motor *Mot = new Motor(Address,Status,Value);

	xyTable *Table = new xyTable();

	char input;
	
	cout << "Do you want to do a reference run? [y/n]" << endl;
	cin >> input;

	if((input=='y') | (input=='Y')){

		cout << "On both axis? [y/n]" << endl;
		cin >> input;

		if((input=='y') | (input=='Y')){

			cout << "Starting reference run on x axis!" << endl;
			sleep(2);
			Mot->ReferenceRunX();

			cout << "Starting reference run on y axis!" << endl;
			sleep(2);
			Mot->ReferenceRunY();

			cout << "Finished reference run!" << endl;
		}

		if((input=='n') | (input=='N')){

			cout << "Which axis? [x/y]" << endl;
			cin >> input;

			if((input=='x') | (input=='X')){

				cout << "Starting reference run on x axis!" << endl;
				sleep(2);
				Mot->ReferenceRunX();
				cout << "Finished reference run!" << endl;


			}

			else if((input=='y') | (input=='Y')){

				cout << "Starting reference run on y axis!" << endl;
				sleep(2);
				Mot->ReferenceRunY();
				cout << "Finished reference run!" << endl;

			}

			else{
				cout << "You're too stupid to work with the xy table! Go away!" << endl;
				exit(EXIT_FAILURE);
			}
		}

	}

	else if ((input == 'n') | (input == 'N'))
	{
		cout << "Skipping Reference Run" << endl;
	}

	else{
		cout << "You're too stupid to work with the xy table! Go away!" << endl;
		exit(EXIT_FAILURE);
	}

	cout << "Do you want to use the y axis?" << endl;
	cin >> input;

	if((input=='y') | (input=='Y')){

		//yaxis = true;

		cout << "What is the start position (in mm) on the x axis?" << endl;
		cin >> x_StartPosition;
		int x_MotorSteps = Mot->CalcStepsX(x_StartPosition);

		cout << "What is the distance (in mm) between two measurements on the x axis?" << endl;
		cin >> x_Dis;

		cout << "How many measurements you would like to take on the x axis?" << endl;
		cin >> x_NumbOfMeas;

		cout << "What is the start position (in mm) on the y axis?" << endl;
		cin >> y_StartPosition;
		int y_MotorSteps = Mot->CalcStepsY(y_StartPosition);

		cout << "What is the distance (in mm) between two measurements on the y axis?" << endl;
		cin >> y_Dis;

		cout << "How many measurements you would like to take on the y axis?" << endl;
		cin >> y_NumbOfMeas;

		//Go to start position
		cout << "Going to start position..." << endl;

		// x axis
		Mot->MoveAbsolute("x", x_MotorSteps);
		//y axis
		Mot->MoveAbsolute("y", y_MotorSteps);
	}

	else if ((input == 'n') | (input == 'N')){
		//yaxis = false;

		cout << "Only using  X AXIS!" << endl;

		cout << "What is the start position (in mm) on the x axis?" << endl;
		cin >> x_StartPosition;
		x_MotorSteps = Mot->CalcStepsX(x_StartPosition);

		cout << "What is the distance (in mm) between two measurements on the x axis?" << endl;
		cin >> x_Dis;

		cout << "How many measurements you would like to take on the x axis?" << endl;
		cin >> x_NumbOfMeas;

		//Go to start position
		cout << "Going to start position..." << endl;
		// x axis
		Mot->MoveAbsolute("x", x_MotorSteps);

	}

	if(yaxis == true){
		Table->xyTableMeasurementBothAxis();
	}

	else if(yaxis == false){
		Table->xyTableMeasurementOnlyXAxis();
	}

	else cout << "Something went wrong..." << endl; exit(EXIT_FAILURE);


	return 0;	
}	




















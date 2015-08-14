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
#include "/home/laborlinux/src/SpectrometerClass/SpecMeasurement.h"
#include <iostream>
#include <unistd.h>
#include <limits>
#include <ncurses.h>
#include "/home/laborlinux/src/LED/LED.h"

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

	else if ((input == 'n') | (input == 'N'))
	{
		
	}

	else{
		cout << "Wrong input. Use Y or N!" << endl;

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

		else{

			cout << "You're to stupid to work with the xy table! Go away!" << endl;

			return 1;

		}
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

	cout << "How many measurements you would like to take on the x axis?" << endl;
	cin >> x_NumbOfSteps;

	cout << "What is the start position (in mm) on the y axis?" << endl;
	cin >> y_StartPosition;
	y_StartPosition = MotorControl::CalcStepsY(y_StartPosition);

	cout << "What is the distance (in mm) between two measurements on the y axis?" << endl;
	cin >> y_Step;

	cout << "How many measurements you would like to take on the y axis?" << endl;
	cin >> y_NumbOfSteps;


	//Go to start position
	cout << "Going to start position..." << endl;
	// x axis
	MotorControl::MoveAbsolute(Motor, "x", x_StartPosition, Address, Status, Value);
	//y axis
	MotorControl::MoveAbsolute(Motor, "y", y_StartPosition, Address, Status, Value);

	//Maybe need to be changed, if start position is far away
	//sleep(5); // sleep in sec

	// Initialize Spectrometer
	int IntTime = 0;
	int NumberOfAverages = 0;
	//string SerialNumber = "114CAA01";

	cout << "Which integration time would you like to use (10000-10000000µs)?" << endl;
	cin >> IntTime;

	cout << "At each position how many averages would you like to take?" << endl;
	cin >> NumberOfAverages;

	double current1, current2, current3;

	cout << "Which currents you would like to use? (current1, current2, current3, in mA)" << endl;
	cin >> current1;
	cin >> current2;
	cin >> current3;
	
	// create your spectrometer
	Spectrometer* ham = new Spectrometer();

	char temp;
	cout << "Press any key to Start Measurement!" << endl;
	cin >> temp;


	cout << "Starting measurements..." << endl;

	double posx=0, posy=0;
	vector<vector<double> > Result;
	vector<vector<double> > Result1;
	vector<vector<double> > Result2;
	vector<vector<double> > Result3;
	stringstream path;


	cout << "Current x position: " << posx << "mm" << endl;
	LEDoff();
	cout << "Start dark measurement..." << endl;
	Result = StartMeasurement(ham, IntTime, NumberOfAverages);

	sleep(5);

	LEDon(current1);
	cout << "Start light measurement with " << current1 << " mA..." << endl;
	Result1 = StartMeasurement(ham, IntTime, NumberOfAverages);

	sleep(5);

	LEDon(current2);
	cout << "Start light measurement with " << current2 << " mA..." << endl;
	Result2 = StartMeasurement(ham, IntTime, NumberOfAverages);

	sleep(5);

	LEDon(current3);
	cout << "Start light measurement with " << current3 << " mA..." << endl;
	Result3 = StartMeasurement(ham, IntTime, NumberOfAverages);
	

	sleep(5);
	LEDoff();

	path << "/home/laborlinux/Data/Spectrometer/Spectrum_" << posx << "mm.txt";
	//cout << path.str() << endl;
	SaveMeasurementDL(Result, Result1, Result2, Result3, path.str());
	path.str("");


	for (int i = 1; i <= x_NumbOfSteps; i++)
	{	
		MotorControl::MoveAbsolute(Motor, "y", y_StartPosition, Address, Status, Value);

		posx=i*x_Step;
		MotorControl::MoveRelative(Motor, "x", MotorControl::CalcStepsX(x_Step), Address, Status, Value);
		sleep(2);

		cout << "Current x position: " << posx << "mm" << endl;
		LEDoff();
		cout << "Start dark measurement..." << endl;
		Result = StartMeasurement(ham, IntTime, NumberOfAverages);

		sleep(5);

		LEDon(current1);
		cout << "Start light measurement with " << current1 << " mA..." << endl;
		Result1 = StartMeasurement(ham, IntTime, NumberOfAverages);

		sleep(5);

		LEDon(current2);
		cout << "Start light measurement with " << current2 << " mA..." << endl;
		Result2 = StartMeasurement(ham, IntTime, NumberOfAverages);

		sleep(5);

		LEDon(current3);
		cout << "Start light measurement with " << current3 << " mA..." << endl;
		Result3 = StartMeasurement(ham, IntTime, NumberOfAverages);
		

		sleep(5);
		LEDoff();

		path << "/home/laborlinux/Data/Spectrometer/Spectrum_" << posx << "mm.txt";
		//cout << path.str() << endl;
		SaveMeasurementDL(Result, Result1, Result2, Result3, path.str());
		path.str("");

		for (int j = 1; j <= y_NumbOfSteps; j++)
		{	
			vector<vector<double> > Result = StartMeasurement(ham, IntTime, NumberOfAverages);
			posy = j*y_Step;
			cout << "Current y position: " << posy << "mm" << endl;		
			MotorControl::MoveRelative(Motor, "y", MotorControl::CalcStepsY(y_Step), Address, Status, Value);
			sleep(2);

		}

	}


	return 0;	
}	

























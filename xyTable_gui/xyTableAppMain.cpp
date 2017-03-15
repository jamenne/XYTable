#include "xyTableApp.h"

#include <iostream>
#include <QApplication>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <errno.h>
#include <signal.h>

// xyTable
#include "../xyTable.h"
// Motor Control
#include "../../MotorControl/Motor.h"
// Spectrometer
#include "../../SpectrometerClass/Spectrometer.h"
// Spectrometer Measurements
#include "../../SpectrometerClass/SpecMeasurement/SpecMeasurement.h"
// LED
#include "../../LEDClass/LED.h"


int main(int argc, char *argv[]) {
    
	QApplication app(argc, argv);

	unsigned char Address = 0;
	unsigned char Status = 0;
	int Value = 0;

	xyTable_App window(Address, Status, Value);

	window.resize(1200, 300);
	window.setWindowTitle("xyTable Measurement");
	window.show();

	return app.exec();

}
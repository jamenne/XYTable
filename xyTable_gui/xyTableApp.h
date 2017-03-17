#pragma once

#include <QWidget>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QVector>

// xyTable
#include "../xyTable.h"
// Motor Control
#include "../../MotorControl/Motor.h"
// LED Class
#include "../../LEDClass/LED.h"
//Spectrometer Class
#include "../../SpectrometerClass/Spectrometer.h"
// Spectrometer Measurements
#include "../../SpectrometerClass/SpecMeasurement/SpecMeasurement.h"
// QCustom Plots
#include "../../../qcustomplot/qcustomplot.h"

class xyTable_App : public QWidget {
    
	Q_OBJECT

	public:
		xyTable_App(unsigned char &Address, unsigned char &Status, int &Value, bool verbosity=false, QWidget *parent = 0);
		virtual ~xyTable_App() {};
		void ShowMessage(QStatusBar *StatusBar, string &Message, int Time);
		void ReloadPlot(QCustomPlot *Plot);
		void SpecMeasChanged(QStackedWidget *MeasButton, QLineEdit *PathLineEdit, QComboBox *SpecMeasComboBox, QDoubleSpinBox *Current1SpinBox, QDoubleSpinBox *Current2SpinBox, QDoubleSpinBox *Current3SpinBox, QPushButton *Btn_Plot);
		void xyTableMeasurementOnlyXAxis(QStatusBar *StatusBar, QLineEdit *PathLineEdit, QDoubleSpinBox *Current1SpinBox, QDoubleSpinBox *Current2SpinBox, QDoubleSpinBox *Current3SpinBox);
		void xyTableMeasurementBothAxis(QStatusBar *StatusBar, QLineEdit *PathLineEdit, QDoubleSpinBox *Current1SpinBox, QDoubleSpinBox *Current2SpinBox, QDoubleSpinBox *Current3SpinBox);

	private:
		Spectrometer *_Spec;
		SpecMeasurement *_SpecMeas;
		LED *_Led;
		Motor *_Mot;
		xyTable *_Table;
		QVector<double> _Spec_x; 
		QVector<double> _Spec_y;
		vector<vector<double> > _SpecData;
		bool _ThreeLightMeas;
};
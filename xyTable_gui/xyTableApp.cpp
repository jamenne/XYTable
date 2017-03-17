#include "xyTableApp.h"

// QT stuff
#include <QtGlobal>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QVector>
#include <QPen>
#include <QStackedWidget>

#include <iostream>
#include <unistd.h>
#include <functional>
#include <sstream>

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
// QCustom PLots
#include "../../../qcustomplot/qcustomplot.h"


using namespace std;


xyTable_App::xyTable_App(unsigned char &Address, unsigned char &Status, int &Value, bool verbosity, QWidget *parent):
  QWidget(parent),
  _Spec(new Spectrometer()),
  _SpecMeas(new SpecMeasurement()),
  _Led(new LED()),
  _Mot(new Motor(Address, Status, Value, verbosity)),
  _Table(new xyTable()),
  _Spec_x(),
  _Spec_y()
{

this->_Mot->ConnectMotor();

  QSize buttonSize(200,20);
  QStatusBar *StatusBar = new QStatusBar();
  stringstream helper;

  //************* LED BOX *************//
  QGroupBox *LEDGroup = new QGroupBox(tr("LED"));

  // LED on
  QLabel *Current1Label = new QLabel(tr("Current1 / mA:"));
  QDoubleSpinBox *Current1SpinBox = new QDoubleSpinBox;
  Current1SpinBox->setRange(0., 20.);
  Current1SpinBox->setSingleStep(0.01);
  Current1SpinBox->setValue(0.00);
  this->_Led->SetCurr(0.00);
  QLabel *Current2Label = new QLabel(tr("Current2 / mA:"));
  QDoubleSpinBox *Current2SpinBox = new QDoubleSpinBox;
  Current2SpinBox->setRange(0., 20.);
  Current2SpinBox->setSingleStep(0.01);
  Current2SpinBox->setValue(0.00);
  this->_Led->SetCurr(0.00);
  QLabel *Current3Label = new QLabel(tr("Current3 / mA:"));
  QDoubleSpinBox *Current3SpinBox = new QDoubleSpinBox;
  Current3SpinBox->setRange(0., 20.);
  Current3SpinBox->setSingleStep(0.01);
  Current3SpinBox->setValue(0.00);
  this->_Led->SetCurr(0.00);
  
  Current1SpinBox->setEnabled( false );
  Current2SpinBox->setEnabled( false );
  Current3SpinBox->setEnabled( false );

  //DoubleSpin Box
  QObject::connect(Current1SpinBox, QOverload< double >::of(&QDoubleSpinBox::valueChanged), [=](double newValue) {
  this->_Led->SetCurr(newValue);
  stringstream helper;
  helper << "Changed Current1 to " << newValue << "mA";
  string helper2 = helper.str();
  this->ShowMessage(StatusBar, helper2, 2000);
  });
    QObject::connect(Current2SpinBox, QOverload< double >::of(&QDoubleSpinBox::valueChanged), [=](double newValue) {
  this->_Led->SetCurr(newValue);
  stringstream helper;
  helper << "Changed Current2 to " << newValue << "mA";
  string helper2 = helper.str();
  this->ShowMessage(StatusBar, helper2, 2000);
  });
      QObject::connect(Current3SpinBox, QOverload< double >::of(&QDoubleSpinBox::valueChanged), [=](double newValue) {
  this->_Led->SetCurr(newValue);
  stringstream helper;
  helper << "Changed Current2 to " << newValue << "mA";
  string helper2 = helper.str();
  this->ShowMessage(StatusBar, helper2, 2000);
  });

  QGridLayout *LEDLayout = new QGridLayout;
  LEDLayout->addWidget(Current1Label, 0, 0);
  LEDLayout->addWidget(Current1SpinBox, 0, 1);
  LEDLayout->addWidget(Current2Label, 1, 0);
  LEDLayout->addWidget(Current2SpinBox, 1, 1);
  LEDLayout->addWidget(Current3Label, 2, 0);
  LEDLayout->addWidget(Current3SpinBox, 2, 1);
  LEDGroup->setLayout(LEDLayout);


  //************* Spectrometer BOX *************//
  QGroupBox *SpecGroup = new QGroupBox(tr("Spectrometer"));

  // Int Time
  QLabel *IntTimeLabel = new QLabel(tr("Integration Time / µs:"));
  QSpinBox *IntTimeSpinBox = new QSpinBox;
  IntTimeSpinBox->setRange(10000, 10000000);
  IntTimeSpinBox->setSingleStep(1);
  IntTimeSpinBox->setValue(10000);
  this->_Spec->SetIntegrationTime(10000);

  // Spin Box
  QObject::connect(IntTimeSpinBox, QOverload< int >::of(&QSpinBox::valueChanged), [=](double newValue) {
  this->_Spec->SetIntegrationTime(newValue);
  stringstream helper;
  helper << "Changed IntegrationTime to " << newValue;
  string helper2 = helper.str();
  this->ShowMessage(StatusBar, helper2, 2000);
  });

  // Numb of Averages
  QLabel *NumbOfAvLabel = new QLabel(tr("Number of Averages:"));
  QSpinBox *NumbOfAvSpinBox = new QSpinBox;
  NumbOfAvSpinBox->setRange(1, 100);
  NumbOfAvSpinBox->setSingleStep(1);
  NumbOfAvSpinBox->setValue(5);
  this->_SpecMeas->SetNumbOfAv(5);


  // Spin Box
  QObject::connect(NumbOfAvSpinBox, QOverload< int >::of(&QSpinBox::valueChanged), [=](double newValue) {
  this->_SpecMeas->SetNumbOfAv(newValue);
  stringstream helper;
  helper << "Changed Number of Averages to " << newValue;
  string helper2 = helper.str();
  this->ShowMessage(StatusBar, helper2, 2000);
  });

  // Measurement Mode
  QLabel *SpecMeasLabel = new QLabel(tr("Measurement Mode:"));
  QComboBox *SpecMeasComboBox = new QComboBox;
  SpecMeasComboBox->addItem(tr("<Select Mode>"));
  SpecMeasComboBox->addItem(tr("Single"));
  SpecMeasComboBox->addItem(tr("Dark + 1 Light"));
  SpecMeasComboBox->addItem(tr("Dark + 3 Light"));

  // Path
  QLabel *PathLabel = new QLabel(tr("Filename:"));
  QLineEdit *PathLineEdit = new QLineEdit;
  PathLineEdit->setPlaceholderText("../Test.txt");
  PathLineEdit->setEnabled( false );

  //************* Spectrometer Plot *************//
  QCustomPlot *Plot = new QCustomPlot();

  Plot->replot();

  // Reload Plot
  QPushButton *Btn_Plot = new QPushButton("Plot", this);
  Btn_Plot->setEnabled(false);
  Btn_Plot->setFixedSize(buttonSize);
  connect(Btn_Plot, &QPushButton::clicked, [=]() {this->ReloadPlot(Plot);
  												  stringstream helper;
												  helper << "Reload Plot";
												  string helper2 = helper.str();
												  this->ShowMessage(StatusBar, helper2, 2000);
  													;});

  
  // ************* Spec Measurement Button ************* //
  QPushButton *Button0 = new QPushButton("Start Measurement", this);
  QPushButton *Button1 = new QPushButton("Start Measurement", this);
  QPushButton *Button2 = new QPushButton("Start Measurement", this);
  QPushButton *Button3 = new QPushButton("Start Measurement", this);

  Button0->setFixedSize(buttonSize);
  Button1->setFixedSize(buttonSize);
  Button2->setFixedSize(buttonSize);
  Button3->setFixedSize(buttonSize);

  Button0->setEnabled( false );

  QStackedWidget *MeasButton = new QStackedWidget();
  MeasButton->addWidget(Button0);
  MeasButton->addWidget(Button1);
  MeasButton->addWidget(Button2);
  MeasButton->addWidget(Button3);

  MeasButton->setFixedSize(buttonSize);

  connect(Button1, &QPushButton::clicked, [=]() {
		this->_Led->LEDon();
		this->_SpecData = this->_SpecMeas->SingleMeasurement(this->_Spec);
		this->_Led->LEDoff();
		stringstream helper;
		helper << "Data fetched";
		string helper2 = helper.str();
		this->ShowMessage(StatusBar, helper2, 2000);
          });

  connect(Button2, &QPushButton::clicked, [=]() {this->_SpecMeas->SingleMeasurementWithDC(this->_Spec, this->_Led, PathLineEdit->text().toStdString());
  		stringstream helper;
		helper << "Measurement finished";
		string helper2 = helper.str();
		this->ShowMessage(StatusBar, helper2, 2000);
  												});

  connect(Button3, &QPushButton::clicked, [=]() {this->_SpecMeas->Measurement3LWithDC(this->_Spec, this->_Led, Current1SpinBox->value(), Current2SpinBox->value(), Current3SpinBox->value(), PathLineEdit->text().toStdString());  
		stringstream helper;
		helper << "Measurement finished";
		string helper2 = helper.str();
		this->ShowMessage(StatusBar, helper2, 2000);
												});


  // Connect Measurement Types to ComboBox
  connect( SpecMeasComboBox, QOverload< int >::of(&QComboBox::currentIndexChanged), [=]() {this->SpecMeasChanged(MeasButton, PathLineEdit, SpecMeasComboBox, Current1SpinBox, Current2SpinBox, Current3SpinBox, Btn_Plot);  
  		stringstream helper;
		helper << "Measurement Mode changed";
		string helper2 = helper.str();
		this->ShowMessage(StatusBar, helper2, 2000);
  });

  //************* Spectrometer Action Grid *************//
  QGridLayout *SpecLayout = new QGridLayout;
  SpecLayout->addWidget(IntTimeLabel, 0, 0);
  SpecLayout->addWidget(IntTimeSpinBox, 1, 0);
  SpecLayout->addWidget(NumbOfAvLabel, 0, 1);
  SpecLayout->addWidget(NumbOfAvSpinBox, 1, 1);
  SpecLayout->addWidget(Plot, 0, 2, 7, 3);
  SpecLayout->addWidget(SpecMeasLabel, 2, 0, 1, 2);
  SpecLayout->addWidget(SpecMeasComboBox, 3, 0, 1, 2);
  SpecLayout->addWidget(PathLabel, 4, 0, 1, 2);
  SpecLayout->addWidget(PathLineEdit, 5, 0, 1, 2);
  SpecLayout->addWidget(MeasButton, 6, 0, 1, 1);
  SpecLayout->addWidget(Btn_Plot, 6, 1, 1, 1);
  SpecGroup->setLayout(SpecLayout);

  //************* XyTable BOX *************//
  QGroupBox *xyGroup = new QGroupBox(tr("XY Table"));

  QPushButton *Btn_SearchMotor = new QPushButton("Connect Motor", this);
  Btn_SearchMotor->setFixedSize(buttonSize);
  Btn_SearchMotor->setEnabled(false);
  connect(Btn_SearchMotor, &QPushButton::clicked, [=]() {
		this->_Mot->ConnectMotor();
    Btn_SearchMotor->setEnabled(false);
	});

  // Reference Run
  QPushButton *Btn_ReferenceX = new QPushButton("Reference Run X", this);
  Btn_ReferenceX->setFixedSize(buttonSize);
  connect(Btn_SearchMotor, &QPushButton::clicked, [=]() { this->_Mot->ReferenceRunX(); 
  		stringstream helper;
		helper << "Reference Run on X finished.";
		string helper2 = helper.str();
		this->ShowMessage(StatusBar, helper2, 2000);
  	});

  QPushButton *Btn_ReferenceY = new QPushButton("Reference Run Y", this);
  Btn_ReferenceY->setFixedSize(buttonSize);
  connect(Btn_SearchMotor, &QPushButton::clicked, [=]() { this->_Mot->ReferenceRunY(); 
  	  	stringstream helper;
		helper << "Reference Run on Y finished.";
		string helper2 = helper.str();
		this->ShowMessage(StatusBar, helper2, 2000);													
  	});

  // Start Measurement Button
  QPushButton *Start0 = new QPushButton("Start Measurement", this);
  QPushButton *Start1 = new QPushButton("Start Measurement", this);

  Start0->setFixedSize(buttonSize);
  Start1->setFixedSize(buttonSize);
  Start0->setEnabled( false );

  QStackedWidget *MeasStart = new QStackedWidget();
  MeasStart->addWidget(Start0);
  MeasStart->addWidget(Start1);
  MeasStart->setFixedSize(buttonSize);

  connect(Start0, &QPushButton::clicked, [=]() {this->xyTableMeasurementBothAxis(StatusBar, PathLineEdit, Current1SpinBox, Current2SpinBox, Current3SpinBox); 
  												});
  connect(Start1, &QPushButton::clicked, [=]() {this->xyTableMeasurementOnlyXAxis(StatusBar, PathLineEdit, Current1SpinBox, Current2SpinBox, Current3SpinBox); 
  												});

  // Spin Boxes for XY Table Parameters
  QLabel *StartPosXLabel = new QLabel(tr("Startpostion X / mm:"));
  QDoubleSpinBox *StartPosXBox = new QDoubleSpinBox;
  StartPosXBox->setRange(0., 2500.);
  StartPosXBox->setSingleStep(0.1);
  StartPosXBox->setValue(0.00);
  this->_Table->Set_x_StartPosition(0.00);
  QObject::connect(StartPosXBox, QOverload< double >::of(&QDoubleSpinBox::valueChanged), [=](double newValue) {
  this->_Table->Set_x_StartPosition(newValue);
  stringstream helper;
  helper << "Startposition on X set to  " << newValue << "mm";
  string helper2 = helper.str();
  this->ShowMessage(StatusBar, helper2, 2000);
	});

  QLabel *StartPosYLabel = new QLabel(tr("Startpostion Y / mm:"));
  QDoubleSpinBox *StartPosYBox = new QDoubleSpinBox;
  StartPosYBox->setRange(0., 190.);
  StartPosYBox->setSingleStep(0.01);
  StartPosYBox->setValue(0.00);
  this->_Table->Set_y_StartPosition(0.00);
  QObject::connect(StartPosYBox, QOverload< double >::of(&QDoubleSpinBox::valueChanged), [=](double newValue) {
  this->_Table->Set_y_StartPosition(newValue);
  stringstream helper;
  helper << "Startposition on Y set to  " << newValue << "mm";
  string helper2 = helper.str();
  this->ShowMessage(StatusBar, helper2, 2000);
	});

  QLabel *DisXLabel = new QLabel(tr("Distance in X / mm:"));
  QDoubleSpinBox *DisXBox = new QDoubleSpinBox;
  DisXBox->setRange(0., 2500.);
  DisXBox->setSingleStep(0.01);
  DisXBox->setValue(1.00);
  this->_Table->Set_x_Dis(1.00);
  QObject::connect(DisXBox, QOverload< double >::of(&QDoubleSpinBox::valueChanged), [=](double newValue) {
  this->_Table->Set_x_Dis(newValue);
  stringstream helper;
  helper << "Distance on X set to  " << newValue << "mm";
  string helper2 = helper.str();
  this->ShowMessage(StatusBar, helper2, 2000);
	});

  QLabel *DisYLabel = new QLabel(tr("Distance in Y / mm:"));
  QDoubleSpinBox *DisYBox = new QDoubleSpinBox;
  DisYBox->setRange(0., 100.);
  DisYBox->setSingleStep(0.01);
  DisYBox->setValue(1.00);
  this->_Table->Set_y_Dis(1.00);
  QObject::connect(DisYBox, QOverload< double >::of(&QDoubleSpinBox::valueChanged), [=](double newValue) {
  this->_Table->Set_y_Dis(newValue); 
  stringstream helper;
  helper << "Distance on Y set to  " << newValue << "mm";
  string helper2 = helper.str();
  this->ShowMessage(StatusBar, helper2, 2000);
	});

  QLabel *NumMeasXLabel = new QLabel(tr("Number of Measurements in X:"));
  QSpinBox *NumMeasXBox = new QSpinBox;
  NumMeasXBox->setRange(1, 10000);
  NumMeasXBox->setSingleStep(1);
  NumMeasXBox->setValue(1);
  this->_Table->Set_x_NumbOfMeas(1);
  QObject::connect(NumMeasXBox, QOverload< int >::of(&QSpinBox::valueChanged), [=](int newValue) {
  this->_Table->Set_x_NumbOfMeas(newValue);
  stringstream helper;
  helper << "Number of Measurements on X set to  " << newValue;
  string helper2 = helper.str();
  this->ShowMessage(StatusBar, helper2, 2000);
	});


  QLabel *NumMeasYLabel = new QLabel(tr("Number of Measurements in Y:"));
  QSpinBox *NumMeasYBox = new QSpinBox;
  NumMeasYBox->setRange(1, 10000);
  NumMeasYBox->setSingleStep(1);
  NumMeasYBox->setValue(1);
  this->_Table->Set_y_NumbOfMeas(1);
  QObject::connect(NumMeasYBox, QOverload< int >::of(&QSpinBox::valueChanged), [=](int newValue) {
  this->_Table->Set_y_NumbOfMeas(newValue); 
  stringstream helper;
  helper << "Number of Measurements on Y set to  " << newValue;
  string helper2 = helper.str();
  this->ShowMessage(StatusBar, helper2, 2000);
	});

  StartPosXBox->setEnabled( true );
  StartPosYBox->setEnabled( true );
  DisXBox->setEnabled( true );
  DisYBox->setEnabled( true );
  NumMeasXBox->setEnabled( true );
  NumMeasYBox->setEnabled( true );

  // XYAxis or OnlyXAxis
  QRadioButton *Btn_XYAxis = new QRadioButton("XY-Axis-Measurement");
  QRadioButton *Btn_OnlyXAxis = new QRadioButton("Only X-Axis-Measurement");

  connect(Btn_XYAxis, &QRadioButton::clicked, [=]() { MeasStart->setCurrentIndex(0);
                                                      Start0->setEnabled(true);
                                                      StartPosYBox->setEnabled(true);
                                                      DisYBox->setEnabled(true);
                                                      NumMeasYBox->setEnabled(true); });
  connect(Btn_OnlyXAxis, &QRadioButton::clicked, [=]() { MeasStart->setCurrentIndex(1);
                                                      Start1->setEnabled(true); 
                                                      StartPosYBox->setEnabled(false);
                                                      DisYBox->setEnabled(false);
                                                      NumMeasYBox->setEnabled(false); });


  //************* xyTable Action Grid *************//
  QGridLayout *xyLayout = new QGridLayout;
  xyLayout->addWidget(Btn_SearchMotor, 0, 0, 1, 1);
  xyLayout->addWidget(Btn_ReferenceX, 0, 1, 1, 1);
  xyLayout->addWidget(Btn_ReferenceY, 0, 2, 1, 1);
  xyLayout->addWidget(StartPosXLabel, 1, 0, 1, 1);
  xyLayout->addWidget(DisXLabel, 1, 1, 1, 1);
  xyLayout->addWidget(NumMeasXLabel, 1, 2, 1, 1);
  xyLayout->addWidget(StartPosXBox, 2, 0, 1, 1);
  xyLayout->addWidget(DisXBox, 2, 1, 1, 1);
  xyLayout->addWidget(NumMeasXBox, 2, 2, 1, 1);
  xyLayout->addWidget(StartPosYLabel, 3, 0, 1, 1);
  xyLayout->addWidget(DisYLabel, 3, 1, 1, 1);
  xyLayout->addWidget(NumMeasYLabel, 3, 2, 1, 1);
  xyLayout->addWidget(StartPosYBox, 4, 0, 1, 1);
  xyLayout->addWidget(DisYBox, 4, 1, 1, 1);
  xyLayout->addWidget(NumMeasYBox, 4, 2, 1, 1);
  xyLayout->addWidget(Btn_OnlyXAxis, 5, 0, 1, 1);
  xyLayout->addWidget(Btn_XYAxis, 5, 1, 1, 1);
  xyLayout->addWidget(MeasStart, 5, 2, 1, 1);

  xyGroup->setLayout(xyLayout);

  //************* QUIT *************//
  QPushButton *Btn_quit = new QPushButton("Quit", this);

  // Actions at Button Press
  connect(Btn_quit, &QPushButton::clicked, &QApplication::quit);

  //********** Put Layout together **********//        
  QGridLayout *layout = new QGridLayout;
  layout->addWidget(LEDGroup, 0, 0, 1, 1);
  layout->addWidget(xyGroup, 0, 1, 1, 1);
  layout->addWidget(SpecGroup, 1, 0, 1, 2);
  layout->addWidget(Btn_quit, 2, 0, 1, 2);
  layout->addWidget(StatusBar, 3, 0, 1, 2);
  setLayout(layout);

  setWindowTitle(tr("xyTable Measurement"));
  
}

void xyTable_App::ShowMessage(QStatusBar *StatusBar, string &Message, int Time){

	QString QMessage = QString::fromStdString(Message);

	StatusBar->showMessage(QMessage, Time);
}

void xyTable_App::ReloadPlot(QCustomPlot *Plot){

  this->_Spec_x = QVector<double>::fromStdVector(this->_SpecData[0]);
  this->_Spec_y = QVector<double>::fromStdVector(this->_SpecData[1]);


  Plot->addGraph();
  // give the axes some labels:
  Plot->xAxis->setLabel("wavelength /nm");
  Plot->yAxis->setLabel("counts");
  // set axes ranges, so we see all data:
  Plot->xAxis->setRange(350, 700);
  //Plot->yAxis->setRange(0, 60000);
  Plot->graph()->setLineStyle(QCPGraph::LineStyle::lsLine);
  // create graph and assign data to it:
  Plot->graph()->setData(this->_Spec_x, this->_Spec_y);
  Plot->graph()->rescaleAxes(true);
  Plot->replot();

}

// Disabled Current Spin Boxes depending on Measurement Mode
void xyTable_App::SpecMeasChanged(QStackedWidget *MeasButton, QLineEdit *PathLineEdit, QComboBox *SpecMeasComboBox, QDoubleSpinBox *Current1SpinBox, QDoubleSpinBox *Current2SpinBox, QDoubleSpinBox *Current3SpinBox, QPushButton *Btn_Plot) {
    if( SpecMeasComboBox->currentText() == "Single" ){ 
      Current1SpinBox->setEnabled( true );
      Current2SpinBox->setEnabled( false );
      Current3SpinBox->setEnabled( false );

      MeasButton->setCurrentIndex(1);

      Btn_Plot->setEnabled( true );

      PathLineEdit->setEnabled( false );

      this->_ThreeLightMeas = false;
      

    }

    else if( SpecMeasComboBox->currentText() == "Dark + 1 Light" ){ 
      Current1SpinBox->setEnabled( true );
      Current2SpinBox->setEnabled( false );
      Current3SpinBox->setEnabled( false );

      MeasButton->setCurrentIndex(2);

      PathLineEdit->setEnabled( true );

      Btn_Plot->setEnabled(false);

      this->_ThreeLightMeas = false;

    }

    else if( SpecMeasComboBox->currentText() == "Dark + 3 Light" ){ 
      Current1SpinBox->setEnabled( true );
      Current2SpinBox->setEnabled( true );
      Current3SpinBox->setEnabled( true );

      MeasButton->setCurrentIndex(3);

      PathLineEdit->setEnabled( true );

      Btn_Plot->setEnabled(false);

      this->_ThreeLightMeas = true;

    }
    else{
      Current1SpinBox->setEnabled( false );
      Current2SpinBox->setEnabled( false );
      Current3SpinBox->setEnabled( false );

      this->_ThreeLightMeas = false;
    }
}


void xyTable_App::xyTableMeasurementOnlyXAxis(QStatusBar *StatusBar, QLineEdit *PathLineEdit, QDoubleSpinBox *Current1SpinBox, QDoubleSpinBox *Current2SpinBox, QDoubleSpinBox *Current3SpinBox){

	cout << "Going to Startposition..." << endl;
	stringstream helper;
	helper << "Going to Startposition...";
	string helper2 = helper.str();
	this->ShowMessage(StatusBar, helper2, 2000);
	helper.str("");

	this->_Mot->MoveAbsolute("x", this->_Mot->CalcStepsX(this->_Table->Get_x_StartPosition()));

	//********** Start of Measurement **********//

	stringstream path;

	cout << "Starting measurements..." << endl;
	helper << "Starting measurements...";
	helper2 = helper.str();
	this->ShowMessage(StatusBar, helper2, 2000);
	helper.str("");

	double posx=0;

	for(int i = 1; i<=this->_Table->Get_x_NumbOfMeas(); i++){

		cout << "Current x position: " << posx << "mm" << endl;
		helper << "Current x position: " << posx << "mm";
		helper2 = helper.str();
		this->ShowMessage(StatusBar, helper2, 2000);
		helper.str("");

		path << PathLineEdit->text().toStdString() << "Spectrum_x=" << posx << "mm.txt";

		//path << "/home/xytable/data/Spectrometer/Spectrum_x=" << posx << "mm.txt";

		//********** Spectrometer Measurement **********//


		if(this->_ThreeLightMeas==true){

			this->_SpecMeas->Measurement3LWithDC(this->_Spec, this->_Led, Current1SpinBox->value(), Current2SpinBox->value(), Current3SpinBox->value(), path.str());
			path.str("");
			
		}

		else if(this->_ThreeLightMeas==false){

			this->_Led->SetCurr(Current1SpinBox->value());
			this->_SpecMeas->SingleMeasurementWithDC(this->_Spec, this->_Led, path.str());
			path.str("");
			
		}


		else exit(EXIT_FAILURE);


		//*******************************************//

		//********** Move Motor to next Position **********//
		posx=i*this->_Table->Get_x_Dis();

		if(posx < this->_Table->Get_x_NumbOfMeas()*this->_Table->Get_x_Dis()){

			this->_Mot->MoveRelative("x", this->_Mot->CalcStepsX(this->_Table->Get_x_Dis()));
			
			// Wait 2 second to let Motor move //
			sleep(2);
		}

	}

	//Go back to start position
	cout << "Going back to start position..." << endl;
	helper << "Going back to start position...";
	helper2 = helper.str();
	this->ShowMessage(StatusBar, helper2, 2000);
	helper.str("");
	// x axis
	this->_Mot->MoveAbsolute("x", this->_Mot->CalcStepsX(this->_Table->Get_x_StartPosition()));
	cout << "Measurements finished!" << endl;
	helper << "Measurements finished!";
	helper2 = helper.str();
	this->ShowMessage(StatusBar, helper2, 2000);
	helper.str("");



}

void xyTable_App::xyTableMeasurementBothAxis(QStatusBar *StatusBar, QLineEdit *PathLineEdit, QDoubleSpinBox *Current1SpinBox, QDoubleSpinBox *Current2SpinBox, QDoubleSpinBox *Current3SpinBox){

	stringstream path;

	//********** Start of Measurement **********//

	cout << "Starting measurements..." << endl;
	double posx, posy=0;

	for(int i = 1; i<=this->_Table->Get_x_NumbOfMeas(); i++){

		cout << "Current x position: " << posx << "mm" << endl;

		posy = 0;

		for(int j = 1; j <=this->_Table->Get_y_NumbOfMeas(); j++){

			cout << "Current y position: " << posy << "mm" << endl;

			//path << PathLineEdit->text().toStdString() << "Spectrum_x=" << posx << "mm_y=" << posy <<"mm.txt";

			path << "/home/xytable/data/Spectrometer/Spectrum_x=" << posx << "mm_y=" << posy <<"mm.txt";

			//********** Spectrometer Measurement **********//

		
			if(this->_ThreeLightMeas==true){

				this->_SpecMeas->Measurement3LWithDC(this->_Spec, this->_Led, Current1SpinBox->value(), Current2SpinBox->value(), Current3SpinBox->value(), path.str());
				path.str("");
				
			}

			else if(this->_ThreeLightMeas==false){

				this->_Led->SetCurr(Current1SpinBox->value());
				this->_SpecMeas->SingleMeasurementWithDC(this->_Spec, this->_Led, path.str());
				path.str("");
				
			}


			else exit(EXIT_FAILURE);

			//*******************************************//

			// Move to the next y position

			posy=j*this->_Table->Get_y_Dis();

			if(posy < this->_Table->Get_y_NumbOfMeas()*this->_Table->Get_y_Dis()){

				this->_Mot->MoveRelative("y", this->_Mot->CalcStepsY(this->_Table->Get_y_Dis()));
				
				// Wait 1 second to let Motor move //
				sleep(1);
			}

		}
		//********** Go back to y-Startposition for the next x-Position **********//
		this->_Mot->MoveAbsolute("y", this->_Mot->CalcStepsY(this->_Table->Get_y_StartPosition()));

		
		//********** Move Motor to next x Position **********//
		posx=i*this->_Table->Get_x_Dis();

		if(posx < this->_Table->Get_x_NumbOfMeas()*this->_Table->Get_x_Dis()){

			this->_Mot->MoveRelative("x", this->_Mot->CalcStepsX(this->_Table->Get_x_Dis()));
			
			// Wait 2 second to let Motor move //
			sleep(2);
		}

	}

	//Go back to start position
	cout << "Going back to start position..." << endl;
	// x axis
	this->_Mot->MoveAbsolute("x", this->_Mot->CalcStepsX(this->_Table->Get_x_StartPosition()));
	cout << "Measurements finished!" << endl;
	
}






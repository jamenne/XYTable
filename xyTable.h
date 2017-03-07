//	xyTable.h to use the xyTable
//
//	Created by Janine Müller on 07.03.2017
//
//
//
//
//

#ifndef ____xyTable__
#define ____xyTable__

class xyTable
{
	public:
		xyTable();
		virtual ~xyTable(){};
		void xyTableMeasurementOnlyXAxis();
		void xyTableMeasurementBothAxis();
		bool Set_y_StartPosition(double ystart);
		bool Set_x_StartPosition(double xstart);
		bool Set_x_Dis(double xDis);
		bool Set_y_Dis(double yDis);


	private:
		//y axis
		double _y_StartPosition; // in mm, used to calculate no of steps for the motor
		double _y_Dis; // distance between two measurements
		int _y_NumbOfMeas;
		//x axis
		double _x_StartPosition; // in mm, used to calculate no of steps for the motor
		double _x_Dis; // distance between two measurements
		int _x_NumbOfMeas;

		bool _yaxis;
};

#endif /* defined(____xyTable__) */
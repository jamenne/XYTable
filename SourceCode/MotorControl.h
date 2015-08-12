//
//  MotorControl.h
//
//
//  Created by Janine Müller on 30.07.2015.
//
//

#ifndef ____MotorControl__
#define ____MotorControl__

#include <vector>
#include <string>

namespace MotorControl {

    void CheckComports();
    void ConnectMotor(bool verbosity, std::vector<int> &Motor);
    void ReferenceRunX(std::vector<int> &Motor, unsigned char Address, unsigned char Status, int Value);
    void ReferenceRunY(std::vector<int> &Motor, unsigned char Address, unsigned char Status, int Value);
    void MoveRelative(std::vector<int> &Motor, std::string xy, int pos, unsigned char Address, unsigned char Status, int Value, int speed=100);
    void MoveAbsolute(std::vector<int> &Motor, std::string xy, int pos, unsigned char Address, unsigned char Status, int Value, int speed=100);
    int CalcStepsX(double pos);
    int CalcStepsY(double pos);

}
    

#endif /* defined(____MotorControl__) */
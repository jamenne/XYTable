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
    void ConnectMotor(bool verbosity, std::vector<int> Motor);
    void ReferenceRunX(int Motor, unsigned char Address, unsigned char Status, int Value);
    void ReferenceRunY(int Motor, unsigned char Address, unsigned char Status, int Value);
    void MoveRelative(int Motor, int pos, unsigned char Address, unsigned char Status, int Value, int speed=100);
    void MoveAbsolute(int Motor, int pos, unsigned char Address, unsigned char Status, int Value, int speed=100);

}
    

#endif /* defined(____MotorControl__) */
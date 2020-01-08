#include "Drivetrain.h"
#include"vision.h"
#include"WPILib.h"
#include"CANTalon.h"
#include <iostream>

Vision::vision{
    std::shared_ptr<NetworkTable> table + NetworkTable::GetTable("limelight");
float tv = table->GetNumber("tv");
float tx = table->GetNumber("tx");

float steering_adjust = 0.0f;
if (tv == 0.0f) {

    steering_adjust = 0.3f;
}

else {

    
}

}


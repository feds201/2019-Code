#include "WPILib.h"
#include "ctre/Phoenix.h"

class Drivetrain {
public:
    Drivetrain(int, int, int, int, int);
    ~Drivetrain();
    void setVelocity(float, float, bool);
    void setMaxAccel(float);

private:
    PigeonIMU* gyro;
    WPI_TalonSRX* motorFL;
    WPI_TalonSRX* motorBL;
    WPI_TalonSRX* motorFR;
    WPI_TalonSRX* motorBR;
    float speed = 0;
    float accel = 0;
    bool setFusedHeading = false;

    float abs(float);
}
#include "main.h"
#include "movement.h"

void stop_all_motors(void)
{
    motorSet(wheel_RF, 0); //Right front wheel
    motorSet(wheel_LF, 0); //Left front wheel
    motorSet(wheel_RB, 0); //Right back wheel
    motorSet(wheel_LB, 0); //Left back wheel
}

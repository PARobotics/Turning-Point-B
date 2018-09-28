#include "main.h"
#include "movement.h"

void stop_all_motors(void)
{
    motorSet(wheel_RF, 0); //Right front wheel
    motorSet(wheel_LF, 0); //Left front wheel
    motorSet(wheel_RB, 0); //Right back wheel
    motorSet(wheel_LB, 0); //Left back wheel
}

void move_full_forward(void)
{
    motorSet(wheel_RF, 127); //Right front wheel
    motorSet(wheel_LF, 127); //Left front wheel
    motorSet(wheel_RB, 127); //Right back wheel
    motorSet(wheel_LB, 127); //Left back wheel
}
void move_full_backward(void)
{
    motorSet(wheel_RF, -127); //Right front wheel
    motorSet(wheel_LF, -127); //Left front wheel
    motorSet(wheel_RB, -127); //Right back wheel
    motorSet(wheel_LB, -127); //Left back wheel
}
void turn_full_left(void)
{
    motorSet(wheel_RF, 127); //Right front wheel
    motorSet(wheel_LF, 0); //Left front wheel
    motorSet(wheel_RB, 127); //Right back wheel
    motorSet(wheel_LB, 0); //Left back wheel
}
void turn_full_right(void)
{
    motorSet(wheel_RF, 0); //Right front wheel
    motorSet(wheel_LF, 127); //Left front wheel
    motorSet(wheel_RB, 0); //Right back wheel
    motorSet(wheel_LB, 127); //Left back wheel
}

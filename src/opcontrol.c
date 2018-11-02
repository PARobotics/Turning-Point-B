/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#include "utility.h"

//TODO: REMOVE DEPENDENCY AFTER TESTING ENCODERS
#include "auto.h"

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */
void operatorControl() {
    int counter = 0;
    while (1) {
        //TODO: Remove encoder testing/counter in future
        if (counter % 200 == 0) //Get encoder value about one time every second
        {
            print_encoder_state();
        }

        int V = joystickGetAnalog(MOVE_JOYSTICK_SLOT, 2);
        if (V > -10 && V < 10) //Thresholded
            V = 0;

        int H = joystickGetAnalog(MOVE_JOYSTICK_SLOT, 1);
        if (H > -10 && H < 10) //Thresholded
           H = 0;

        motorSet(wheel_RF, min(127, max(-127, H-V)));
        motorSet(wheel_RB, min(127, max(-127, H-V)));

        motorSet(wheel_LF, min(127, max(-127, V+H)));
        motorSet(wheel_LB, min(127, max(-127, V+H)));

        delay(5);
        ++counter;
    }
}

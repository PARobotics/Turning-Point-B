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
#include "auto.h"
#include "opcontrol.h"
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
    int pr_down = 0;
    int pr_up = 0;
    int holding = 0;
    while (1) {
        /*
        //TODO: Remove encoder testing/counter in future
        if (counter % 200 == 0) //Get encoder value about one time every second
        {
            print_encoder_state();
        }
        */

        // BUTTON TO TEST AUTONOMOUS and put down the prongs
        if(joystickGetDigital(1, 7, JOY_LEFT)) {
          autonomous();
        }

        // DRIVE TRAIN (left joystick)
        drive_train_control();

        // ROTATE LBAR
        // if(joystickGetDigital(1, 7, JOY_UP)) {
        //     motorSet(L_bar, -1*L_bar_speed);
        //     //delay(L_bar_time);
        //     motorSet(L_bar, 0);
        // }
        //
        // if(joystickGetDigital(1, 7, JOY_DOWN)) {
        //     motorSet(L_bar, L_bar_speed/3);
        //     //delay(L_bar_time/2);
        //     motorSet(L_bar, 0);
        // }


        // holding voltage
        // if (holding) { // if already on
        //   if(joystickGetDigital(1, 7, JOY_UP)) { // if button pressed
        //     // do nothing
        //   } else { // if un-pressed
        //     holding = 0;
        //     motorSet(L_bar, 0);
        //     motorSet(L_bar2, 0);
        //   }
        // } else { // if not already on
        //   if(joystickGetDigital(1, 7, JOY_UP)) { // if button pressed
        //     holding = 1;
        //     motorSet(L_bar, -1*L_bar_holding_speed);
        //     motorSet(L_bar2, -1*L_bar_holding_speed);
        //
        //   } else { // if un-pressed
        //     // do nothing
        //   }
        // }

        // LBAR UP
        if (!pr_up) { // not pressed
          if(joystickGetDigital(1, 8, JOY_UP)) { // pressed
            motorSet(L_bar, -1*(L_bar_up_speed));
            motorSet(L_bar2, -1*(L_bar_up_speed));

            pr_up = 1;
          }
        } else { // pressed
          if(!joystickGetDigital(1, 8, JOY_UP)) { // un-pressed
            motorSet(L_bar, -1*(L_bar_holding_speed));
            motorSet(L_bar2, -1*(L_bar_up_speed));

            pr_up = 0;
          }
        }
        // LBAR DOWN
        if (!pr_down) { // not pressed
          if(joystickGetDigital(1, 8, JOY_DOWN)) { // pressed
            motorSet(L_bar, L_bar_down_speed);
            motorSet(L_bar2, L_bar_down_speed);
            pr_down = 1;
          }
        } else { // pressed
          if(!joystickGetDigital(1, 7, JOY_DOWN)) { // un-pressed
            motorSet(L_bar, 0);
            motorSet(L_bar2, 0);
            pr_down = 0;
          }
        }

        // OPERATE LIFT WITH RIGHT JOYSTICK
        int lift_up = joystickGetAnalog(MOVE_JOYSTICK_SLOT, 2);
        if (lift_up > -10 && lift_up < 10) //Thresholded
          lift_up = 0;
        lift_up = -1*lift_up; // toggle up and down

        motorSet(lift_rtop, lift_up);
        motorSet(lift_rbottom, lift_up);
        motorSet(lift_ltop, lift_up);
        motorSet(lift_lbottom, lift_up);

        ++counter;
    }
}

void drive_train_control(void)
{
    int V = joystickGetAnalog(MOVE_JOYSTICK_SLOT, 3);
    if (V > -10 && V < 10) //Thresholded
        V = 0;
    V=-1*V;

    int H = joystickGetAnalog(MOVE_JOYSTICK_SLOT, 4);
    if (H > -10 && H < 10) //Thresholded
       H = 0;
    H=-1*H;
    H=turn_scale*H; // TODO: see if this is effective, adjust turn_scale value
    motorSet(wheel_RF, min(127, max(-127, 1*(H-V))));
    motorSet(wheel_RB, min(127, max(-127, 1*(H-V))));

    motorSet(wheel_LF, min(127, max(-127, -1*(V+H))));
    motorSet(wheel_LB, min(127, max(-127, -1*(V+H))));
}

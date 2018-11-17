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
#include "opcontrol.h"
#include "lbar.h"
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

 // void lbar_up(void * parameter) {
 //   int press_up = 0;
 //   while(true) {
 //     if (!press_up) { // not pressed
 //       if(joystickGetDigital(1, 7, JOY_UP)) { // pressed
 //         motorSet(L_bar, -1*L_bar_speed);
 //         press_up = 1;
 //       }
 //     } else { // pressed
 //       if(!joystickGetDigital(1, 7, JOY_UP)) { // un-pressed
 //         motorSet(L_bar, 0);
 //         press_up = 0;
 //       }
 //     }
 //   }
 // }
 //
 // void lbar_down(void * parameter) {
 //   int press_down = 0;
 //   while(true) {
 //     if (!press_down) { // not pressed
 //       if(joystickGetDigital(1, 7, JOY_UP)) { // pressed
 //         motorSet(L_bar, L_bar_speed/3);
 //         press_down = 1;
 //       }
 //     } else { // pressed
 //       if(!joystickGetDigital(1, 7, JOY_UP)) { // un-pressed
 //         motorSet(L_bar, 0);
 //         press_down = 0;
 //       }
 //     }
 //   }
 // }

void operatorControl() {
    int counter = 0;
    int lawn_on = 1; // should be turned on in init
    int mid_on = 0;
    int pr_down = 0;
    int pr_up = 0;

    // TaskHandle lbar_up_task =
    //   taskCreate(lbar_up, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
    // TaskHandle lbar_down_task =
    //   taskCreate(lbar_down, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);

    while (1) {
        /*
        //TODO: Remove encoder testing/counter in future
        if (counter % 200 == 0) //Get encoder value about one time every second
        {
            print_encoder_state();
        }
        */

        // DRIVE TRAIN
        drive_train_control();

        // BOTTOM LAWNMOWER (toggle)
        if(joystickGetDigital(1, 7, JOY_LEFT)) {
            if (lawn_on) { // initially on
              motorSet(lawnmower_bottom, 0);
            } else { // initially off
                motorSet(lawnmower_bottom, lawn_bottom_speed);
            }
            lawn_on = (lawn_on+1)%2; // flip condition
        }

        // MIDDLE LAWNMOWER AND FLYWHEEL
        if(joystickGetDigital(1, 7, JOY_RIGHT)) {
            motorSet(flywheel, flywheel_speed);
            motorSet(lawnmower_middle, lawn_mid_speed);
            delay(5000);
            motorSet(lawnmower_middle, 0);
            motorSet(flywheel, 0);
        }

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


        // LBAR UP
        if (!pr_up) { // not pressed
          if(joystickGetDigital(1, 7, JOY_UP)) { // pressed
            motorSet(L_bar, -1*L_bar_speed);
            pr_up = 1;
          }
        } else { // pressed
          if(!joystickGetDigital(1, 7, JOY_UP)) { // un-pressed
            motorSet(L_bar, 0);
            pr_up = 0;
          }
        }

        // LBAR DOWN
        if (!pr_down) { // not pressed
          if(joystickGetDigital(1, 7, JOY_DOWN)) { // pressed
            motorSet(L_bar, L_bar_speed/3);
            pr_down = 1;
          }
        } else { // pressed
          if(!joystickGetDigital(1, 7, JOY_DOWN)) { // un-pressed
            motorSet(L_bar, 0);
            pr_down = 0;
          }
        }

        // OPERATE LIFT WITH LEFT JOYSTICK
        int lift_up = joystickGetAnalog(MOVE_JOYSTICK_SLOT, 3);
        if (lift_up > -10 && lift_up < 10) //Thresholded
          lift_up = 0;
        motorSet(lift_1, -1*lift_up);
        motorSet(lift_2, lift_up);

        ++counter;
    }
}

void drive_train_control(void)
{
    int V = joystickGetAnalog(MOVE_JOYSTICK_SLOT, 2);
    if (V > -10 && V < 10) //Thresholded
        V = 0;

    int H = joystickGetAnalog(MOVE_JOYSTICK_SLOT, 1);
    if (H > -10 && H < 10) //Thresholded
       H = 0;

    motorSet(wheel_RF, min(127, max(-127, 1*(H-V))));
    motorSet(wheel_RB, min(127, max(-127, 1*(H-V))));

    motorSet(wheel_LF, min(127, max(-127, 1*(V+H))));
    motorSet(wheel_LB, min(127, max(-127, 1*(V+H))));
}

/** @file auto.c
 * @brief File for autonomous code
 *
 * This file should contain the user autonomous() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#include "auto.h"
#include "movement.h"

/*
 * Runs the user autonomous code. This function will be started in its own task with the default
 * priority and stack size whenever the robot is enabled via the Field Management System or the
 * VEX Competition Switch in the autonomous mode. If the robot is disabled or communications is
 * lost, the autonomous task will be stopped by the kernel. Re-enabling the robot will restart
 * the task, not re-start it from where it left off.
 *
 * Code running in the autonomous task cannot access information from the VEX Joystick. However,
 * the autonomous function can be invoked from another task if a VEX Competition Switch is not
 * available, and it can access joystick information if called in this way.
 *
 * The autonomous task may exit, unlike operatorControl() which should never exit. If it does
 * so, the robot will await a switch to another mode or disable/enable cycle.
 */
void autonomous() {
}

void auton_move(double revs, direction d)
{
    int counts, terminate = 0;

    switch (d) {
      case FORWARD: move_full_forward(); break;
      case BACKWARD: move_full_backward(); break;
      case RIGHT: turn_full_right(); break;
      case LEFT: turn_full_left(); break;
      default:
        /* TODO: Report a horrible error; no valid direction provided */
        return;
    }
    
    imeReset(LEFT_BACK_ENCODER);
    while (!terminate) {
      imeGet(LEFT_BACK_ENCODER, &counts);
      if ((double)counts >= (revs * TICKS_PER_REV))
        terminate = 1;
    }

    stop_all_motors();
}

void print_encoder_state()
{
    printf("Attempting to get encoder value!\n");
    int left_back_counts = -1, right_back_counts = -1;
    imeGet(LEFT_BACK_ENCODER, &left_back_counts);
    imeGet(RIGHT_BACK_ENCODER, &right_back_counts);

    printf("Left back encoder value: %d, right back encoder value: %d", left_back_counts, right_back_counts);
}

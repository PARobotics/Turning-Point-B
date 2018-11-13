#ifndef AUTO_H_
#define AUTO_H_

typedef enum {FORWARD, BACKWARD, LEFT, RIGHT } direction;

void auton_move_encoder(double revs, direction d);
void auton_move_timer(int move_time, direction d);

void print_encoder_state();

#endif

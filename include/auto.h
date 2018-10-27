#ifndef AUTO_H_
#define AUTO_H_

typedef enum {FORWARD, BACKWARD, LEFT, RIGHT } direction;

void auton_move(double revs, direction d);
void print_encoder_state();

#endif

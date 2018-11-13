#include "main.h"
#include "lbar.h"

void rotate_lbar()
{
    static int is_flipped = 0;

    //Flip lbar

    // Get IME tick count in the "counts" variable
    // int cts = 0;
    // imeReset(L_bar);
    // //imeGet(L_bar, &cts);
    // motorSet(L_bar, L_bar_speed);
    //
    // //printf("counts: %d\n", cts);

    // while (cts < L_bar_ticks) {
    //     printf("looping");
    //     if(!imeGet(L_bar, &cts))
    //     {
    //         printf("!!!!Failed to get IME ticks!");
    //         break;
    //     }
    // }
    // imeReset(L_bar);

    if (!is_flipped) {
        motorSet(L_bar, L_bar_speed);
        is_flipped = 1;
    } else {
        motorSet(L_bar, -1*L_bar_speed);
        is_flipped = 0;
    }

    delay(lbar_time);
    motorSet(L_bar, 0);
}

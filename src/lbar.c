#include "main.h"
#include "lbar.h"

void rotate_lbar()
{
    //Flip lbar

    static int is_flipped = 0;
    if (!is_flipped)
        is_flipped = 1;
    else
        is_flipped = 0;

    // Get IME tick count in the "counts" variable
    int counts;
    imeGet(L_bar, &counts);
    motorSet(L_bar, L_bar_speed);

    while (counts < L_bar_ticks) {
        imeGet(L_bar, &counts);
    }
    motorSet(L_bar, 0);
    // Reset IME to zero
    imeReset(L_bar);

}

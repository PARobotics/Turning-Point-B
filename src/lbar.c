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

    imeReset(L_bar);
    imeGet(L_bar, &counts);
    motorSet(L_bar, L_bar_speed);

    while (counts < L_bar_ticks) {
        if(!imeGet(L_bar, &counts))
        {
            printf("Failed to get IME ticks!");
            break;
        }
    }

    motorSet(L_bar, 0);
    // Reset IME to zero
    imeReset(L_bar);

}

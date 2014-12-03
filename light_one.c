#include "reg52.h"
// 0xA? P2
__sbit __at (0xA6) dula;
__sbit __at (0xA7) wela;
uchar Num[] = {
    ZERO, ONE, TWO, THREE, FOUR,
    FIVE, SIX, SEVEN, EIGHT, NINE,
};
usint tally = 0;

void main()
{
    wela = 1;
    P0 = 0xC0;
    wela = 0;

    while (1)
    {
        dula = 0;
        P0 = Num[tally++];
        dula = 1;
        delay(1000);
        if (tally > 9)
            tally = 0;
    }
}

#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "thread.h"


Thread *fourth;

uint32_t millis(void)
{
    const int msPerS = 1000;

    return (int)((float)clock()/(CLOCKS_PER_SEC/msPerS));
}

void delay(uint32_t del)
{
    const uint32_t maxMillis = (uint32_t)(0 - 1);

    uint32_t start, curr;
    uint32_t diff;

    start = millis();

    while(1)
    {
        curr = millis();

        if(start > curr)
        {
            diff = curr + (maxMillis - start);
        }
        else
        {
            diff = curr - start;
        }

        if(del <= diff)
        {
            break;
        }
    }
}

void th1(int state)
{
    printf("Prvi %d\n", millis());
    wait_ms(1500, 0);
}

void th2(int state)
{
    printf("Drugi %d\n", millis());
    threadSendSignal(fourth, 4);
    wait_ms(1000, 0);
}

void th3(int state)
{
    switch (state)
    {
        case 0:
            printf("Treci_0 %d\n", millis());
            wait_ms(500, 1);
        break;

        case 1:
            printf("Treci_1 %d\n", millis());
            threadSendSignal(fourth, 1);
            wait_ms(1000, 2);
        break;

        case 2:
            printf("Treci_2 %d\n", millis());
            wait_ms(1500, 0);
        break;
    }
}

void th4(int state)
{
    switch (state)
    {
        case 0:
            printf("Cetvrti_0 %d\n", millis());
            wait_signal(1, 1);
        break;

        case 1:
            printf("Cetvrti_1 %d\n", millis());
            wait_signal(4, 0);
        break;
    }
}

int main(void)
{
    /*
    delay(1000);
    printf("Ulaz1\n");
    delay(2000);
    printf("Ulaz2\n");
*/
    threadInit(millis, delay);

    threadAdd(th1);
    threadAdd(th2);
    threadAdd(th3);
    fourth = threadAdd(th4);

    threadStart();

    getchar();
    return 0;
}

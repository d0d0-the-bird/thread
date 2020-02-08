/*
MIT License

Copyright (c) 2018 D0D0

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include "thread.h"
#include "thread_config.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __NO_GLOBAL_PROJECT_CONFIGURATION_FILE__
    #error Project conf doesnt exist
#endif

struct _ThreadInternal
{
    Thread *currThread;
    Thread *t[THREAD_MAX_THREAD];
    uint32_t (*_millis)(void);
    void (*_delay)(uint32_t);
} threadGen;


static bool waitingForSignal(Thread *t);


void threadInit(uint32_t (*_millis)(void), void (*_delay)(uint32_t))
{
    uint8_t i;

    threadGen.currThread = NULL;
    threadGen._millis = _millis;
    threadGen._delay = _delay;

    for( i = 0; i < THREAD_MAX_THREAD; i++)
        threadGen.t[i] = NULL;
}

Thread *threadAdd(void (*callback)(int))
{
    Thread **freeSlot;

    for(freeSlot = threadGen.t; *freeSlot != NULL; freeSlot++);

    *freeSlot = (Thread*)malloc(sizeof(Thread));

    (*freeSlot)->waitFor = 0;
    (*freeSlot)->waitStart = 0;
    (*freeSlot)->nextState = 0;
    (*freeSlot)->awaitingSignal = NO_SIGNAL;
    (*freeSlot)->callback = callback;

    return *freeSlot;
}

Thread *threadGetCurr(void)
{
    return threadGen.currThread;
}

bool threadSetWait(uint32_t waitFor, int nextState)
{
    bool retval = false;

    if(waitFor > MAX_WAIT)
    {
        waitFor = MAX_WAIT;
    }

    if(threadGen.currThread)
    {
        retval = true;

        threadGen.currThread->waitStart = threadGen._millis();
        threadGen.currThread->waitFor = waitFor;

        threadGen.currThread->nextState = nextState;
    }

    return retval;
}

void threadStart(void)
{
    const uint32_t maxMillis = (uint32_t)(0 - 1);

    Thread **curr;

    while(1)
    {
        uint32_t minWait = MAX_WAIT;

        for(curr = threadGen.t; *curr != NULL; curr++)
        {
            uint32_t diff;
            uint32_t currMinWait = MAX_WAIT;
            uint32_t currTime = threadGen._millis();

            if( waitingForSignal(*curr) )
                continue;

            threadGen.currThread = *curr;

            if((*curr)->waitStart > currTime)
            {
                diff = currTime + (maxMillis - (*curr)->waitStart);
            }
            else
            {
                diff = currTime - (*curr)->waitStart;
            }

            if((*curr)->waitFor <= diff)
            {
                (*curr)->callback((*curr)->nextState);

                currMinWait = (*curr)->waitFor;
            }
            else
            {
                currMinWait = (*curr)->waitFor - diff;
            }

            if(minWait > currMinWait)
            {
                minWait = currMinWait;
            }
        }

        // Check if any thread awaits immediate execution. This can happen if
        // a thread with lower priority sent a signal to a thread of higher
        // priority.
        for(curr = threadGen.t; *curr != NULL; curr++)
        {
            if( (*curr)->waitFor == 0 )
                minWait = 0;
        }

        threadGen._delay(minWait);
    }
}

bool threadWaitForSignal(uint32_t sig, int nextState)
{
    bool retval = false;

    if( threadSetWait(MAX_WAIT, nextState) )
    {
        retval = true;

        threadGen.currThread->awaitingSignal = sig;
    }

    return retval;
}

void threadSendSignal(Thread *t, uint32_t sig)
{
    if( t->awaitingSignal == sig )
    {
        t->awaitingSignal = NO_SIGNAL;
        t->waitFor = 0;
    }
}

static bool waitingForSignal(Thread *t)
{
    return t->awaitingSignal != NO_SIGNAL;
}

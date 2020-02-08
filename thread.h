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


#ifndef __THREAD_H__
#define __THREAD_H__


#include <stdint.h>
#include <stdbool.h>


#define MAX_WAIT                    ((uint32_t)(0-1) >> 1)

#define NO_SIGNAL                   ((uint32_t)0)

#define wait_ms(ms, nextState)                                                 \
    do {                                                                       \
        if( threadSetWait((ms), (nextState)) )                                 \
            return;                                                            \
    } while(0)

#define wait_signal(sig, nextState)                                            \
    do {                                                                       \
        if( threadWaitForSignal((sig), (nextState)) )                          \
            return;                                                            \
    } while(0)


typedef struct
{
    uint32_t waitFor;
    uint32_t waitStart;
    int nextState;
    uint32_t awaitingSignal;
    void (*callback)(int);
} Thread;


void threadInit(uint32_t (*_millis)(void), void (*_delay)(uint32_t));

Thread *threadAdd(void (*callback)(int));

bool threadSetWait(uint32_t waitFor, int nextState);

void threadStart(void);

bool threadWaitForSignal(uint32_t sig, int nextState);

void threadSendSignal(Thread *t, uint32_t sig);


#endif//__THREAD_H__

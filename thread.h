#ifndef __THREAD_H__
#define __THREAD_H__


#include <stdint.h>
#include <stdbool.h>

#define MAX_THREAD                  (20)
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

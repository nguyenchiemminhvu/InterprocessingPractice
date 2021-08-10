#ifndef __SHARED_SEM_H__
#define __SHARED_SEM_H__

#include <semaphore.h>

struct shared_locker
{
    sem_t level1;
    sem_t level2;
};

#endif // __SHARED_SEM_H__
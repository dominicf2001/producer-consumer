#ifndef SHM_H
#define SHM_H

#include <stdlib.h>
#include <semaphore.h>

#define BUFFER_SIZE 10

struct shared_memory {
    sem_t sem;
    int resources[BUFFER_SIZE];
};
#endif // SHM_H

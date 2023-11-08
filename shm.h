#ifndef SHM_H
#define SHM_H

#include <stdlib.h>
#include <semaphore.h>
#include <stdio.h>

#define BUFFER_SIZE 2

struct shared_memory {
    sem_t sem;
    
    int resources[BUFFER_SIZE];
};

void print_resources(int* resources) {
    printf("[");
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        if (i == (BUFFER_SIZE - 1)) {
            printf("%d]\n\n", resources[i]);
        }
        else {
            printf("%d, ", resources[i]);
        }
    }
}

#endif // SHM_H

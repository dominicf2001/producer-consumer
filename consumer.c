#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include "./shm.h"

int main(int argc, char** argv){
    const char* sm_path = "/test";
    
    // open the shared memory object
    int oflag = O_RDWR;
    int file_descriptor = shm_open(sm_path, oflag, S_IRWXU);
    if (file_descriptor == -1) {
        printf("Error on shm_open (c)\n");
        return -1;
    }

    // map the shared memory object into this processes' memory space
    struct shared_memory* shmp = mmap(NULL, sizeof(struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);
    if (shmp == MAP_FAILED) {
        printf("Error on mmap (c)\n");
        shm_unlink(sm_path);
        return -1;
    }

    // wait for the producer to produce
    if (sem_wait(&shmp->sem) == -1) {
        printf("Error on sem_init (c)\n");
        shm_unlink(sm_path);
        return -1;
    }

    printf("OUTPUT: %d\n", shmp->resources[0]);

    // unlink the shared memory object
    shm_unlink(sm_path);
}

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
    
    // create the shared memory object
    int oflag = O_RDWR | O_CREAT | O_EXCL;
    int file_descriptor = shm_open(sm_path, oflag, S_IRWXU);
    if (file_descriptor == -1) {
        printf("Error on shm_open (p)\n");
        return -1;
    }

    // set the memory size of the shared memory object to store the struct
    if (ftruncate(file_descriptor, sizeof(struct shared_memory)) == -1) {
        printf("Error on ftruncate (p)\n");
        shm_unlink(sm_path);
        return -1;
    }
    
    // map the shared memory object into this processes' memory space
    struct shared_memory* shmp = mmap(NULL, sizeof(struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);
    if (shmp == MAP_FAILED) {
        printf("Error on mmap (p)\n");
        shm_unlink(sm_path);
        return -1;
    }

    // initialize the semaphore
    if (sem_init(&shmp->sem, 1, 0) == -1) {
        printf("Error on sem_init (p)\n");
        shm_unlink(sm_path);
        return -1;
    }
    
    // set a test value
    shmp->resources[0] = 2;

    sem_post(&shmp->sem);
}

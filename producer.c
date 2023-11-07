#include <iostream>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

struct shared_memory {
    int** resources;
    sem_t sem;
};

int main(int argc, char** argv){
    const char* sm_path = "/test";
    
    // create the shared memory object
    int oflag = O_RDWR | O_CREAT | O_EXCL;
    int file_descriptor = shm_open(sm_path, oflag, S_IRWXU);
    if (file_descriptor == -1) {
        std::cerr << "Error on shm_open\n";
        return -1;
    }

    // set the memory size of the shared memory object to store the struct
    if (ftruncate(file_descriptor, sizeof(struct shared_memory)) == -1) {
        std::cerr << "Error on ftruncate\n";
        return -1;
    }

    // map the shared memory object into this processes' memory space
    struct shared_memory* shmp = mmap(NULL, sizeof(struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);
    if (shmp == MAP_FAILED) {
        std::cerr << "Error on mmap\n";
        return -1;
    }

    if (sem_init(shmp->sem, 1, 0) == -1) {
        std::cerr << "Error on sem_init\n";
        return -1;        
    }

    // unlink the shared memory object
    shm_unlink(sm_path);
}

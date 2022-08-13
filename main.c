//
// main.c
// Author - Shuber Ali Mirza
// ID ----- 20027047
//

#include <stdio.h>     // printf, fopen, fscanf, fclose
#include <stdlib.h>    // malloc/calloc, atoi, free, exit
#include <unistd.h>    // pid_t, fork, getpid, getppid
#include <sys/wait.h>  // wait
#include <sys/mman.h>  // mmap and its flags
#include <semaphore.h> // sem_t, sem_open, sem_close, sem_wait, sem_post, sem_unlink
#include <fcntl.h>     // O_CREAT and O_EXCL
#include <pthread.h>   /* pthread_t, pthread_mutex_t, pthread_create, pthread_join, pthread_mutex_init,
                       pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock */
#include "macros.h"    // Subtotal struct and function declarations

int main(int argc, char* argv[]){
    if(argc < 6 || argc > 6){
        printf("ERROR - Wrong number of arguments given\n");
        printf("Usage - $./pmms <matFile1> <matFile2> <rowsOfMat1> <colsOfMat1/rowsOfMat2> <colsOfMat2>\n");
        exit(EXIT_FAILURE);
    }
    int m = atoi(argv[3]);                                        // no. of rows in A
    int n = atoi(argv[4]);                                        // no. of columns in A and rows in B
    int k = atoi(argv[5]);                                        // no. of columns in B
    int i, j;                                                     // Interation
    pid_t pids[m];                                                // Array to store pids of children
    sem_t* sem;                                                   // Semaphore for processes
    unsigned int semVal = 1;                                      // Number of processes allowed to access sempahore simultaneously
    pthread_t th[m];                                              // Array to store pointers to threads
    int* res;                                                     // Used for getting return value from threads
    Subtotal* subtotal = mmap(NULL, sizeof(Subtotal),
                              PROT_READ | PROT_WRITE,
                              MAP_SHARED | MAP_ANONYMOUS, -1, 0); // Subtotal struct
    int* total = mmap(NULL, sizeof(int),
                      PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);         // Total
    
    // Init 2d arrays
    int** mtxA = matrixCreate(m, n); // Matrix A
    int** mtxB = matrixCreate(n, k); // Matrix B
    int** mtxC;                      // Init Matrix C
    
    // Reading matrices from files
    matrixRead(argv[1], m, n, mtxA);
    matrixRead(argv[2], n, k, mtxB);
    mtxC = matrixProduct(mtxA, mtxB, m, n, k); // Finds product matrix
    
    // Printing matrices
    printf("Matrix A\n");
    matrixPrint(mtxA, m, n);
    printf("Matrix B\n");
    matrixPrint(mtxB, n, k);
    printf("Product Matrix\n");
    matrixPrint(mtxC, m, k);
    
    subtotal->product = mtxC; // Setting product as product matrix, so it can be passed as argument to thread routine
    subtotal->cols = k;       // Cols remains constant
    
    // PROCESSES
    *total = 0;
    sem = sem_open("semaphore", O_CREAT | O_EXCL, 0644, semVal);
    for(i=0; i<m; i++){
        pids[i] = fork(); // Creating child
        if(pids[i] < 0){  // Error checking child creation
            printf("EROOR - Child not created\n");
            sem_unlink("semaphore");
            sem_close(sem);
            exit(EXIT_FAILURE);
        }
        if(pids[i] == 0){  // Accessing child
            sem_wait(sem); // Decrementing semaphore
            subtotal->row = i;
            getSub(subtotal); // Getting subtotal
            *total += subtotal->sub;
            sem_post(sem); // Signalling semaphore
            // Freeing resources from children
            sem_unlink("semaphore");
            sem_close(sem);
            matrixFree(mtxA, m, n);
            matrixFree(mtxB, n, k);
            matrixFree(mtxC, m, k);
            exit(getpid()); // Exiting child
        }
        // wait(NULL); // For demo purposes
    }
    // Parent waiting for process execution to complete
    for(i=0; i<m; i++){
        wait(NULL);
    }
    printf("\nTotal:\t\t\t\t\t\t\t%d\n", *total);
    
    sem_unlink("semaphore");
    sem_close(sem);
    
    printf("\n");
    
    // THREADS
    *total = 0;
    pthread_mutex_init(&(subtotal->mutex), NULL); // Init mutex
    for(i=0; i<m; i++){
        pthread_mutex_lock(&(subtotal->mutex)); // Locking while accessing shared variable
        subtotal->sub = 0;
        subtotal->row = i;
        pthread_create(th + i, NULL, &getSub_thread, subtotal); // Creating thread
    }
    for(i=0; i<m; i++){
        pthread_join(th[i], (void**) &res); // Used for parent to wait for threads, and return value from routine
        printf("Subtotal from thread %d, from parent %d, row %d:\t%d\n", i+1, getpid(), i+1, *(int*)res);
        *total += *(int*)res; // return from thread routine added to total
        free(res); // Freeing return pointer from routine
    }
    pthread_mutex_destroy(&(subtotal->mutex)); // Destroying mutex
    printf("Total:\t\t\t\t\t\t\t%d\n", *total);
    
    // Freeing resources
    munmap(subtotal, sizeof(Subtotal));
    munmap(total, sizeof(total));
    matrixFree(mtxA, m, n);
    matrixFree(mtxB, n, k);
    matrixFree(mtxC, m, k);
    
    return 0;
}
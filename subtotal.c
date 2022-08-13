//
// Author - Shuber Ali Mirza
// ID ----- 20027047
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "macros.h"

void getSub(Subtotal* subtotal){
    subtotal->sub = 0;
    for(int j=0; j<subtotal->cols; j++){
        subtotal->sub += subtotal->product[subtotal->row][j]; // Children accessing subtotal
    }
    printf("\nSubtotal from child %d, from Parent %d, row %d:\t%d", getpid(), getppid(), (subtotal->row)+1, subtotal->sub);
    fflush(stdout);
}

void* getSub_thread(void* arg){
    int* data = malloc(sizeof(int)); // Variable for return value
    int i = ((Subtotal*)arg)->row;
    int** matrix = ((Subtotal*)arg)->product;
    int k = ((Subtotal*)arg)->cols;
    
    for(int j=0; j<k; j++){
        ((Subtotal*)arg)->sub += matrix[i][j]; // Shared variable changes made here
    }
    *data = ((Subtotal*)arg)->sub;
    pthread_mutex_unlock(&(((Subtotal*)arg)->mutex)); // Unlocking mutex after access complete
    
    return (void*)data;
}
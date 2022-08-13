//
// Author - Shuber Ali Mirza
// ID ----- 20027047
//

#include <stdio.h> // printf, fopen, fscanf, fclose
#include <stdlib.h> // malloc/calloc, free
#include "macros.h"

int** matrixCreate(int r, int c)
{
    int i,j;
    int** a = calloc(r, sizeof(int*)); /* Making pointers to each row */
    
    for (i=0 ; i<r ; i++){
        a[i] = calloc(c, sizeof(int)); /* Assigning seperate arrays to each pointer */
    }
    return a;
}

// Reference: https://stackoverflow.com/questions/48936647/read-a-matrix-from-a-file
void matrixRead(char* filename, int r, int c, int** matrix){
    int i,j;
    FILE* fp;
    fp = fopen(filename, "r");
    if(fp == NULL){
        printf("ERROR - File not read\n");
    }
    else{
        for(i=0; i<r; i++){
            for(j=0; j<c; j++){
                fscanf(fp, "%d", matrix[i] + j);
            }
        }
        fclose(fp);
    }
}

int** matrixProduct(int** matrixA, int** matrixB, int m, int n, int o){
    int i,j,k;
    int** product = matrixCreate(m, o);
    
    for(i=0; i<m; i++){
        for(j=0; j<o; j++){
            for(k=0; k<n; k++){
                product[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
    return product;
}

void matrixPrint(int** matrix, int r, int c){
    int i,j;
    for(i=0; i<r; i++){
        for(j=0; j<c; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void matrixFree(int** matrix, int r, int c){
    int i,j;
    for(i=0; i<r; i++){
        free(matrix[i]);
    }
    free(matrix);
}

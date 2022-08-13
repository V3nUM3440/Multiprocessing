#ifndef MATRIX
#define MATRIX

typedef struct{
    int sub; // For processes and threads
    int** product; // For threads
    int row; // For threads
    int cols; // For threads
    pthread_mutex_t mutex; // mutex var for threads
}Subtotal;

// From matrix.c
int** matrixCreate(int r, int c);
void matrixRead(char* filename, int r, int c, int** matrix);
int** matrixProduct(int** matrixA, int** matrixB, int m, int n, int o);
void matrixPrint(int** matrix, int r, int c);
void matrixFree(int** matrix, int r, int c);

// From subtotal.c
void getSub(Subtotal* subtotal);
void* getSub_thread(void* arg);

#endif // MATRIX
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define MAX 100

// -------- STRUCTURES --------
typedef struct {
    float heap[MAX];
    int size;
} Heap;

// -------- FUNCTION DECLARATIONS --------
void insertMax(Heap *h, float val);
void insertMin(Heap *h, float val);
float getMax(Heap *h);
float getMin(Heap *h);
float getAverage(float arr[], int n);
void checkThreshold(float arr[], int n, float threshold);
void displayTopK(Heap h, int k);
void swap(float *a, float *b);
#endif


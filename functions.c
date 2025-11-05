#include <stdio.h>
#include "functions.h"

// ---------- SWAP -----------

void swap(float *a , float *b)
{
    float temp = *a;
    *a = *b;
    *b = temp;
}

// ---------- MAX HEAP INSERT ----------

void insertMax(Heap *h, float val)
{
    h->heap[h->size] = val;
    int i = h->size;
    h->size++;

    // Heapify up
    while (i > 0)
    {
        int parent = (i - 1)/2;
        int leftChild = 2 * parent + 1;
        int rightChild = 2  * parent + 2;

        //comparing
        if (h->heap[i] > h->heap[parent])
        {
            swap(&h->heap[i] , &h->heap[parent]);
            i = parent;
        }
        else
        {
            break;
        }
    }   
}

// ---------- MIN HEAP INSERT ----------

void insertMin(Heap *h, float val)
{
    h->heap[h->size] = val;
    int i = h->size;
    h->size++;

    // heapify

    while( i > 0)
    {
        int parent = (i-1)/2;
        int leftChild = 2 * parent + 1;
        int rightChild = 2 * parent + 2;

        if (h->heap[i] < h->heap[parent])
        {
            swap(&h->heap[i] , &h->heap[parent]);
            i = parent;
        }
        else
        {
            break;
        }
    }
}

// ---------- GET MAX ----------

float getMax(Heap *h)
{
    if (h->size == 0) 
    { return -1; }
    return h->heap[0];
}

// ---------- GET MIN ----------

float getMin(Heap *h)
{
    if (h->size == 0) 
    { return -1; }
    return h->heap[0];
}

// ---------- AVERAGE ----------

float getAverage(float arr[], int n)
{
    if ( n <= 0)
    {
        return 0;
    }
    float sum = 0;
    for (int i = 0; i < n; i++)
        sum += arr[i];
    return sum / n;
}

// ---------- THRESHOLD CHECK ----------

void checkThreshold(float arr[], int n, float threshold)
{
    int flag = 0;
    for (int i = 0; i < n; i++)
    {
        if (arr[i] > threshold)
        {
            printf("High CPU usage detected: %.2f%%\n", arr[i]);
            flag = 1;
        }
    }
    if (!flag)
    {
        printf("No usage exceeded %.2f%%\n", threshold);
    }
}

// ---------- DISPLAY TOP K ----------

void displayTopK(Heap h, int k)
{
    if (k <= 0)
    {
        printf("\nInvalid number of spikes requested ( k = %d). Must be >0.\n" , k);
        return;
    }

    if (h.size == 0)
    {
        printf("\nNo data available to display.\n");
        return;
    }

    if (k > h.size)
    {
        k = h.size;
    }
    printf("\nTop %d CPU spikes:\n", k);
    for (int i = 0; i < k; i++)
    {
        int maxIdx = i;

        for (int j = i + 1; j < h.size; j++)
        {
            if (h.heap[j] > h.heap[maxIdx])
            { maxIdx = j; }
        }
        swap(&h.heap[i], &h.heap[maxIdx]);
        printf("%.2f%%  ", h.heap[i]);
    }
    printf("\n");
}


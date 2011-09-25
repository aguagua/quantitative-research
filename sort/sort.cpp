/*
 *  sort.cpp
 *  
 *
 *  Created by Wei Wang on 17/07/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "sort.h"
#include <cstdlib>
#include <cstring>
#include <pthread.h>

// bubble sort
void __inline__ swap(int * a, int * b)
{
	int temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSort(int * v, int n)
{
    for (int j = n-1; j > 0; j--) {
        for (int i = 0; i < j; i++) {
            if (v[i] > v[i+1])
                swap(v+i, v+i+1);        
        }	
    }
}

// insertion sort
void insertionSort(int * v, int n)
{
	for(int j=1; j<n; j++){
    	int key = v[j];
        int i = j-1;
        while (i>=0 && v[i]>key) {
            v[i+1] = v[i];
            i--;
        }
        v[i+1] = key;
    }
}

// selection sort 
int __inline__ findMin(int * v, int n)
{
    for (int i=1; i<n; i++) {
        if (v[0] > v[i]) {
            swap(v, v+i);
        }
    }
    return v[0];
}

void selectionSort(int * v, int n)
{
    int i;
    for (i=0; i<n; i++) {
        v[i] = findMin(v+i, n-i);
    }
}

// quick sort
void quickSort(int *v, int n)
{
    if(n<=1) 
        return;
    
    int i=0,j=n-1;
    int pivot = v[n/2];
    
    // partition
    do{
        while (v[i]<pivot) i++;
        while (v[j]>pivot) j--;
        if(i<=j) {
            swap(v+i, v+j); 
            i++; 
            j--;
        }
    } while (i<=j);
    
    // recursion
    quickSort(v,j+1);
    quickSort(v+i,n-i);
}

// merge sort
void __inline__ merge (int *a, int *b, int n)
{
	int i, *p, *q;
    int x[n];
    for(i=0, p=a, q=b; i< n; i++)
        x[i] = p==b ? *q++ : q==a+n ? *p++ : *p < *q ? *p++ : *q++;
    memcpy(a, x, n*sizeof(int));
}

void mergeSort(int *v, int n)
{
	if (n<=1) return;
    
    int m = n/2;
    
    mergeSort(v,m);
    mergeSort(v+m,n-m);
    
    merge(v,v+m,n);
}

// heap sort
void heapSort(int *v, int n)
{
	int t;
    int parent = n/2;
    int index, child;
    
    while(1){
    	if (parent>0) {
        	t = v[--parent];
        } else {
            n--;
            if (n==0) {
            	return;
            }
        	t = v[n];
            v[n] = v[0];
        }
		index = parent;
        child = index*2 + 1;
        while (child < n) {
            if (child +1 < n && v[child+1] > v[child]) {
            	child++;
            }
            if(v[child]>t) {
            	v[index] = v[child];
                index = child;
                child = index*2 + 1;
            } else {
                break;
            }
        }
        v[index] = t;
    }
}

// shellsort
void __inline__ shellSortPhase(int * a, int length, int gap) 
{
    int i;
    for (i = gap; i < length; ++i) {
        int value = a[i];
        int j;
        for (j = i - gap; j >= 0 && a[j] > value; j -= gap) {
            a[j + gap] = a[j];
        }
        a[j + gap] = value;
    }
}

void shellSort(int * v, int n) 
{
    static const int gaps[] = {
        1, 4, 10, 23, 57, 132, 301, 701
    };
    int sizeIndex;
    
    for (sizeIndex = sizeof(gaps)/sizeof(gaps[0]) - 1;
         sizeIndex >= 0;
         --sizeIndex)
        shellSortPhase(v, n, gaps[sizeIndex]);
}

//parallel quick sort
void * quickSort(void * para)
{	
    Parameter p = *(Parameter *)para;
    int *v = p.v;
    int n =  p.n;
    
    if(n<=1) 
        return 0;
    
    int i=0, j=n-1;
	int pivot = v[n/2];
    
    // partition
    do{
        while (v[i]<pivot) i++;
        while (v[j]>pivot) j--;
        if(i<=j) {
            swap(v+i, v+j); 
            i++; 
            j--;
        }
    } while (i<=j);
    
    pthread_t Thread[2];
    Parameter param1 = {v,j+1};
    Parameter param2 = {v+i,n-i};
    
    pthread_create(&Thread[0], NULL, &quickSort, &param1);
    pthread_create(&Thread[1], NULL, &quickSort, &param2);
    pthread_join(Thread[0],NULL);
    pthread_join(Thread[1],NULL);
    
    return NULL;
}

void parallelQuickSort(int * v, int n)
{
    Parameter para = {v, n};
    quickSort(&para);
}

/*
 *  sort.h
 *  
 *
 *  Created by Wei Wang on 17/07/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SORT_H
#define SORT_H

// pthreads parameter
struct Parameter
{
    int * v;
    int	n;
};

// sequential sort
void quickSort(int * v, int n);
void selectionSort(int * v, int n);
void heapSort(int * v, int n);
void mergeSort(int * v, int n);
void insertionSort(int * v, int n);
void bubbleSort(int * v, int n);
void shellSort(int * v, int n);

// parallel quicksort
void parallelQuickSort(int *v, int n);

#endif

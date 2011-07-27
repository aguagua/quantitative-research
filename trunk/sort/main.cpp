/*
 *  main.cpp
 *  lucid
 *
 *  Created by Wei Wang on 24/07/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "profiler.h"
#include "sort.h"

int main (int argc, char * const argv[]) {
    // sorting algorithms    
	algos sortingAlgos[] = {
        {&bubbleSort, "bubblesort"}, 
        {&insertionSort, "insertionsort"},
        {&selectionSort,"selectionsort"},
        {&quickSort, "quicksort"},
        {&mergeSort, "mergesort"},
        {&heapSort, "heapsort"},
        {&shellSort, "shellsort"},
        {&parallelQuickSort, "parallelquicksort"}
    };
    
    // number of algorithms
    int numofAlgos = sizeof(sortingAlgos)/sizeof(sortingAlgos[0]);
    
    // maximum input data array size
    int arraySize = 1000;
    
    // profile sorting algorithms
    profiler(sortingAlgos, numofAlgos, arraySize);
    
    return 0;
}

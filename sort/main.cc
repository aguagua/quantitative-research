/*
 * Copyright (c) 2011-2012, William Wang
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: William Wang
 *
 */

#include "profiler.hh"
#include "sort.hh"

/**
 * This benchmark profiles the internal sorting algorithms performance
 */
int main (int argc, char * const argv[]) {
    
    // sorting algorithms, function and name
    algos sortingAlgos[] = {
        //{&bubbleSort, "bubblesort"},
        //{&insertionSort, "insertionsort"},
        //{&selectionSort,"selectionsort"},
        //{&shellSort, "shellsort"},
        {&quickSort, "quicksort"},
        //{&mergeSort, "mergesort"},
        {&heapSort, "heapsort"},
        {&stlSort, "STLsort"},
        {&stlStableSort,"STLStableSort"},
        {&timSort, "timsort"},
        {&parallelQuickSort, "parquicksort"}
    };

    // number of algorithms
    int numofAlgos = sizeof(sortingAlgos)/sizeof(sortingAlgos[0]);

    // maximum input data array size 10 million
    int arraySize = 1<<29+1;

    // profile sorting algorithms
    profiler(sortingAlgos, numofAlgos, arraySize);

    return 0;
}

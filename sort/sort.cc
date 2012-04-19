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

#include "sort.hh"
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <vector>
#include <algorithm>

using namespace std;

// bubble sort
// compare and swap
void __inline__ swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// In-place sort
void bubbleSort(int* v, int n)
{
    for (int j=n-1; j>0; j--) {
        for (int i=0; i<j; i++) {
            if (v[i] > v[i+1])
                swap(v+i, v+i+1);
        }
    }
}

// insertion sort
void insertionSort(int* v, int n)
{
    for (int j=1; j<n; j++) {
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
// compare and swap
int __inline__ findMin(int* v, int n)
{
    for (int i=1; i<n; i++) {
        if (v[0] > v[i]) {
            swap(v, v+i);
        }
    }
    return v[0];
}

void selectionSort(int* v, int n)
{
    int i;
    for (i=0; i<n; i++) {
        v[i] = findMin(v+i, n-i);
    }
}

// quick sort
void quickSort(int* v, int n)
{
    if (n<=1)
        return;

    int i=0, j=n-1;
    int pivot = v[n/2];

    // partition
    do{
        while (v[i]<pivot) i++;
        while (v[j]>pivot) j--;
        if (i<=j) {
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
void __inline__ merge (int* a, int* b, int n)
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
void heapSort(int* v, int n)
{
    int t;
    int parent = n/2;
    int index, child;

    while (1) {
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
void __inline__ shellSortPhase(int* a, int length, int gap)
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

void shellSort(int* v, int n)
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

// c++ default sort
void stlSort(int* v, int n)
{
    vector<int> data;
    data.reserve(n);
    copy(v, v+n, data.begin());
    sort(data.begin(), data.end());
    copy(data.begin(),data.end(),v);
}

void stlStableSort(int* v, int n)
{
    vector<int> data;
    data.reserve(n);
    copy(v, v+n, data.begin());
    stable_sort(data.begin(), data.end());
    copy(data.begin(),data.end(),v);
}

//parallel quick sort
void* quickSort(void* para)
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

    // threads take ~70us to create
    if (n >= 1<<16) {
        // parallel version
        pthread_create(&Thread[0], NULL, &quickSort, &param1);
        pthread_create(&Thread[1], NULL, &quickSort, &param2);
        pthread_join(Thread[0],NULL);
        pthread_join(Thread[1],NULL);
    } else {
        // sequential version
        quickSort(v,j+1);
        quickSort(v+i,n-i);
    }

    return NULL;
}

void parallelQuickSort(int* v, int n)
{
    Parameter para = {v, n};
    quickSort(&para);
}

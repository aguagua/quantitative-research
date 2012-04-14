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
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <sys/time.h>
#include <pthread.h>
#include <exception>
#include <fstream>
#include <sys/file.h>

using namespace std;

static ofstream sa;

void printSortedArray(algos sAlgo, int * array, int n)
{
    // check the functionality of the algorithm
    sa << sAlgo.name << " sorted array in ascending order: "
       << "("<< n << ")"<< std::endl;
    for (int i = 0 ; i < n; ++i) {
        sa << std::setw(WIDTH) << array[i];
        if ((n>=16) && (i%16 ==0) && (i>=16))
            sa << std::endl;
    }
    sa << std::endl;
}

// algorithm benchmark function
clock_t benchmark(algos sAlgo, int * v, int n)
{
    struct timeval startTime, endTime;
    clock_t duration;

    int* array;

    // copy the data onto the stack
    try {
        array = new int[n];
    } catch (bad_alloc&) {
        cout << "heap allocation failed"<< endl;
    }

    memcpy(array, v, n*sizeof(int));

    // use gettimeofday for microseconds Wall time
    // or clock_gettime() for CPU time in CLOCKS_PER_SECOND

    // start the timer
    gettimeofday(&startTime, NULL);

    // do the real work
    sAlgo.pfunc(array, n);

    // stop the timer
    gettimeofday(&endTime, NULL);

#ifdef PRINT_SORTED_ARRAY
    printSortedArray(sAlgo, array, n);
#endif

    // delete the array
    delete [] array;

    // return benchmark runtime in microseconds
    duration = (endTime.tv_sec - startTime.tv_sec)*1000000 + (endTime.tv_usec - startTime.tv_usec) + 0.5;
    return duration;
}

// wrapper function for the benchmark
void* pBenchmark(void* p_par)
{
    Par p = *(Par *)p_par;
    algos algo = p.a;
    int* vec = p.v;
    int   num = p.n;

    *p.r = benchmark(algo, vec, num);

    return NULL;
}

// profile the sorting algos
void profiler(algos sAlgos[], int num, int arraySize)
{
#ifdef PRINT_RESULTS
    std::cout << "[name of the algorithm] "  << " [lowest input size at which it is optimal]  " << std::endl;
#endif

    srand(time(NULL));

    // find the best algorithms for a certain input data sizes
    bool hits[num];

#ifdef PRINT_SORTED_ARRAY
    sa.open("sorted_array.txt");
#endif

#ifdef PRINT_RUN_TIME
    ofstream rt;
    rt.open("run_time.txt");
    rt << std::setw(WIDTH) <<"Size";
    for (int i= 0 ; i < num; i++) {
        rt << std::setw(WIDTH) << sAlgos[i].name;
    }
    rt << std::endl;
#endif

    // increase input data size and find the best performing algorithm
    for (int n = 2; n < arraySize; n= n<<1 ) {

        // generate input data set (random integers)
        int size = n;
        int v[size];

        for (int i = 0; i < size; ++i) {
            v[i] = rand();
        }

        // benchmark the algorithms with input data sets
        clock_t runTime[num];
        pthread_t Thread[num];
        Par p[num];

        for (int i = 0; i < num; ++i) {
            p[i] = {sAlgos[i], v, size, &runTime[i]};
            pthread_create(&Thread[i], NULL, &pBenchmark, &p[i]);
        }

        // wait for the threads to finish
        for (int i = 0; i < num; ++i)
        {
            pthread_join(Thread[i],NULL);
        }

#ifdef PRINT_RUN_TIME
        rt << std::setw(WIDTH) << size;
        for (int i= 0 ; i < num; i++) {
            rt << std::setw(WIDTH) << runTime[i];
        }
        rt << std::endl;
#endif

        // printout the optimal sorting algorithm at the least input size
        for (int i = 0; i < num; ++i) {
            int j;

            for (j = 0; j < num; ++j) {
                if (runTime[i]>=runTime[j]&&i!=j) {
                    break;
                }
            }

            if (j==num && !hits[i]) {
#ifdef PRINT_RESULTS
                std::cout <<std::setw(22) << sAlgos[i].name << std::setw(44)<< size << std::endl;
#endif
                hits[i] = true;
            }
        }
    }

#ifdef PRINT_RUN_TIME
    rt.close();
#endif

#ifdef PRINT_SORTED_ARRAY
    sa.close();
#endif

}

/*
 *  profiler.cpp
 *  lucid
 *
 *  Created by Wei Wang on 17/07/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "profiler.h"
#include "sort.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <iomanip>

//#define PRINT_SORTED_ARRAY
//#define PRINT_RUN_TIME
#define PRINT_RESULTS

// algorithm benchmark function
clock_t benchmark(algos sAlgo, int * v, int n)
{
    clock_t startTime, endTime;
    clock_t duration;
    
	int array[n];
    memcpy(array, v, n*sizeof(int));
    
    startTime = clock();
    sAlgo.pfunc(array, n);
    endTime = clock();
    
#ifdef PRINT_SORTED_ARRAY
    std::cout << sAlgo.name << " sorted array in ascending order: " << std::endl;	
    for (int i = 0 ; i < n; ++i) {
        std::cout << array[i] << std::endl;
    }
    std::cout << std::endl;
#endif
    
    duration = endTime - startTime;
    return duration;
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
    
#ifdef PRINT_RUN_TIME
    std::cout << std::setw(6) <<"Size";
    for (int i= 0 ; i < num; i++) {
        std::cout << std::setw(15) << sAlgos[i].name;
    }
    std::cout << std::endl;
#endif
    
    // increase input data size and find the best performing algorithm
    for (int n = 2; n<arraySize; n=n+1) {
        
        // generate input data set (random integers)
        int size = n;
        int v[size];
        for (int i = 0; i < size; ++i) {
            v[i] = rand();
        }
        
        // benchmark the algorithms with input data sets
        clock_t runTime[num];
        for (int i = 0; i < num; ++i) {
            runTime[i] = benchmark(sAlgos[i], v, size);
        }
        
#ifdef PRINT_RUN_TIME
        std::cout << std::setw(6) << size;
        for (int i= 0 ; i < num; i++) {
            std::cout << std::setw(15) << runTime[i];
        }
        std::cout << std::endl;
#endif
        
        // print out the optimal sorting algorithm at the least input size
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
}


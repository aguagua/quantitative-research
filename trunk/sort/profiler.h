/*
 *  profiler.h
 *  lucid
 *
 *  Created by Wei Wang on 17/07/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PROFILER_H
#define PROFILER_H

// functional pointer to different sorting algos
typedef void (*algo)(int *, int);

// data structure to contain algorithm name
struct algos {
    algo pfunc;
    const char * name;
};

// profiler function
void profiler(algos sAlgos[], int num, int arraySize);

#endif

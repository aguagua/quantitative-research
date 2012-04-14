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

#ifndef PROFILER_HH
#define PROFILER_HH

#include <ctime>
#include <pthread.h>

#define PRINT_RESULTS
#define PRINT_RUN_TIME
//#define PRINT_SORTED_ARRAY

// printout width
static const int WIDTH = 16;

// functional pointer to different sorting algos
typedef void (*algo)(int *, int);

// data structure to hold algorithm (the pointer to algorithm function)
// and its name
struct algos {
  algo pfunc;
  const char * name;
};

// profiler function
void profiler(algos sAlgos[], int num, int arraySize);

// pthread parameter
typedef struct Par{
    algos a;
    int* v;
    int n;
    clock_t* r;
} Par;

// Mutex class
class Mutex
{
  public:
    Mutex() {}
    virtual ~Mutex() {}
    void lock() { pthread_mutex_lock(&mutex); }
    void unlock() { pthread_mutex_unlock(&mutex); }

  private:
    pthread_mutex_t mutex;
};

// Guard class
template <typename T>
class Guard
{
  public:
    explicit Guard(T& t) : gate(t) { gate.lock(); }
    ~Guard() { gate.unlock(); }

  private:
    // gate can be a semaphore or mutex
    T& gate;
    Guard(const Guard&);
    Guard& operator=(const Guard&);
};

#endif

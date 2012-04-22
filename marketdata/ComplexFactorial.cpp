/**
 * William Wang
 */

#include <cstdlib>
#include <iostream>
#include <cassert>

double Fac(int n)
{
    if (n < 2)
        return 1;
    
    return n*Fac(n-1);
}

double ComplexFactorial(int n)
{
    assert(n>=1);
    
    return Fac(n) + Fac(n-1);
}

/** Test program */
int main(int argc, char* argv[]) 
{
    // check the correct number of inputs
    if (argc != 2) {
        std::cout << "Please specify n (n>=1)" << std::endl;
        return -1;
    }
    
    int num = atoi(argv[1]);
    
    double CF = ComplexFactorial(num);
    std::cout << CF << std::endl;
}

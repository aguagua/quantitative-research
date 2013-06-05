/**
 * William Wang
 */

#include<cmath>
#include<iostream>
#include<cassert>
#include<cstdlib>

/** Based on the prime number theorem, an asymptotic expression for the
    nth prime number is n*ln(n), and according to Rosser's theorem,
    the nth prime number is larger than n*ln(n) but upper bounded by 
    n*ln(n) + nlnln(n) for n >= 6. 
*/
int getNthPrime(int n)
{
    // check n is valid
    assert(n>=1);
    
    // return the 1st to 5th prime number
    if (n==1) {
        return 2;
    } else if (n==2) {
        return 3;
    } else if (n==3) {
        return 5;
    } else if (n==4) {
        return 7;
    } else if (n==5) {
        return 11;
    }
    
    // return the nth (n>=6) prime number according to Rosser's bounds
    
    // step 1: compute the lower bound and upper bound
    int loBound = ceil(n*log(n));
    int upBound = floor(n*log(n) + n*log(log(n)));
    
    // step 2: find the prime number within the bounds

    // find the number of prime numbers smaller than the lower bound
    int count = 5;
    for (int i = 12; i< loBound; ++i) {
        int sqrtBound = static_cast<int>(sqrt(i));
        for (int j = 2; j <= sqrtBound; ++j) {
            if (i%j==0) {
                break;
            } else if (j == sqrtBound) {
                count++;
            }
        }   
    }
    
    // count towards the upper bound and return the nth prime number
    for(int i = loBound; i< upBound; ++i) {
        int sqrtBound = static_cast<int>(sqrt(i));
        for (int j = 2; j <= sqrtBound; ++j) {
            if (i%j == 0) {
                break;
            } else if (j == sqrtBound) {
                if (++count == n)
                    return i;
            }
        }
    }
}

/** Test program */
int main(int argc, char* argv[]) 
{
    // check the correct number of inputs
    if (argc != 2) {
        std::cout << "Please specify n (n>=1), the nth prime number!" << std::endl;
        return -1;
    }
    
    int num = atoi(argv[1]);
    
    int NthPrime = getNthPrime(num);
    std::cout << NthPrime << std::endl;
}

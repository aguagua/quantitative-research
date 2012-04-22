/**
 * William Wang
 */

#include "bigInt.hh"
#include <cstdlib>

BigInteger zero(0);
BigInteger one(1);
BigInteger two(2);

BigInteger Fac(BigInteger n)
{
    if(n < two)
        return one;

    return n*Fac(n-one);
}

BigInteger ComplexFactorial(BigInteger n)
{
    return Fac(n) + Fac(n-one);
}

int main (int argc, char * const argv[])
{
    // check the correct number of inputs
    if (argc != 2) {
        std::cout << "Please specify n (n>=1)" << std::endl;
        return -1;
    }
    
    int num = atoi(argv[1]);
    
    BigInteger bigint(num);
    bigint = ComplexFactorial(bigint);
    std::cout << bigint << std::endl;

    return 0;
}

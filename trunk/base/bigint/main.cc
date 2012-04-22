#include "BigInteger.hh"

using namespace std;

BigInteger one(1);
BigInteger two(2);

BigInteger factorial(BigInteger n)
{
    if(n < two)
    	return one;
    
    return n*factorial(n-one);
}

int main (int argc, char * const argv[])
{
    BigInteger a("11223344556677889900");
    BigInteger b("-98765432109876543210");
    std::cout << a << " + " << b << " = " << a + b << std::endl;
    std::cout << a << " - " << b << " = " << a - b << std::endl;
	std::cout << a << " * " << b << " = " << a * b << std::endl;
    
    if (a < b)
        std::cout << "a is smaller than b." << std::endl;
    else if (a == b)
        std::cout << "a is equal to b." << std::endl;
    else
        std::cout << "a is larger than b." << std::endl;
    
    if (a * b < BigInteger("0"))
        std::cout << "The product is negative." << std::endl;
    else
        std::cout << "The product is non-negative." << std::endl;
    
    BigInteger bigint(2011);
    bigint = factorial(bigint);
    std::cout << bigint << std::endl;
    
    return 0;
}

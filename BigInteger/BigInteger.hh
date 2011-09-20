/**
 * William Wang
 */

#ifndef _BigInteger_H_
#define _BigInteger_H_

#include <iostream>
#include <string>
#include <deque>

using namespace std;

class BigInteger
{
  private:
    enum signType {neg, pos};
    typedef deque<int>::size_type sizeType;
    typedef deque<int> magnitudeType;
    
    // sign + magnitude
    signType sign;
    magnitudeType magnitude;
    
    // helper function
    void remove_leading_zeros(magnitudeType& num);
    
    // arithmetic cores
    magnitudeType addCore(const magnitudeType &lhs, const magnitudeType &rhs);
    magnitudeType subCore(const magnitudeType &lhs, const magnitudeType &rhs);
    magnitudeType multCore(const magnitudeType &lhs, const magnitudeType &rhs);
    
    // relational
    bool magnitudeLess(const BigInteger& rhs) const;
    bool magnitudeEqual(const BigInteger& rhs) const;
    
  public:
    /**
     * Arithmetic assignment operators
     */
    const BigInteger& operator += (const BigInteger& );
    const BigInteger& operator -= (const BigInteger& );
    const BigInteger& operator *= (const BigInteger& );
    
    BigInteger& operator += (int num);
    BigInteger& operator -= (int num);
    BigInteger& operator *= (int num);
    
    /**
     * Relational operators 
     */
    bool equal(const BigInteger& rhs) const;
    bool less(const BigInteger& rhs) const;
    
    /**
     * I/O operators
     */
    virtual string toString() const;
    
    /**
     * Copy and assignment
     */
    BigInteger(const BigInteger&);
    BigInteger& operator=(const BigInteger&);

    /**
     * Constructors
     */
    BigInteger(int value, bool negative = false);
    BigInteger(string value);
    BigInteger();
};

// Arithmetic binary operators
BigInteger operator + (const BigInteger& lhs, const BigInteger & rhs);
BigInteger operator + (const BigInteger& lhs, int num);
BigInteger operator + (int num, const BigInteger& rhs);

BigInteger operator - (const BigInteger& lhs, const BigInteger & rhs);
BigInteger operator - (const BigInteger& lhs, int num);
BigInteger operator - (int num, const BigInteger& rhs);

BigInteger operator * (const BigInteger& lhs, const BigInteger& rhs);
BigInteger operator * (const BigInteger& lhs, int num);
BigInteger operator * (int num, const BigInteger& rhs);

// Rational Operators
bool operator == (const BigInteger& lhs, const BigInteger& rhs);
bool operator == (const BigInteger& lhs, int num);
bool operator == (int num, const BigInteger& rhs);

bool operator <  (const BigInteger& lhs, const BigInteger& rhs);
bool operator <  (const BigInteger& lhs, int num);
bool operator <  (int num, const BigInteger& rhs);

// IO operators
ostream& operator<<(ostream& out, const BigInteger& rhs);
#endif

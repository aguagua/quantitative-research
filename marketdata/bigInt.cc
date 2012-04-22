/**
 * William Wang
 */

#include "bigInt.hh"
#include <algorithm>
#include <sstream>

void BigInteger::remove_leading_zeros(magnitudeType& num)
{
    sizeType i;
    for ( i = num.size(); i>0;--i)
        if(num[i-1]!=0)
            break;
    if(i!=num.size())
        num.resize(i);
}

// ctor
BigInteger::BigInteger(): sign(pos), magnitude(0)
{
}

BigInteger::BigInteger(int num, bool negative)
: sign(negative ? neg : pos), magnitude(0)
{
    do {
        magnitude.push_back(num%10);
        num /= 10;
    } while (num);
}

BigInteger::BigInteger(string str): sign(pos), magnitude(0)
{
        if (str[0]=='-') {
        sign = neg;
        for(int i = 1; i < str.size(); ++i) {
            string strnum = str.substr(i,1);
            magnitude.push_front(atoi(strnum.c_str()));
        }
    } else {
        for(int i = 0; i < str.size(); ++i) {
            string strnum = str.substr(i,1);
            magnitude.push_front(atoi(strnum.c_str()));
        }
    }
}

// copy and assignment ctor
BigInteger::BigInteger(const BigInteger& rhs)
{
    sign = rhs.sign;
        magnitude = rhs.magnitude;
}

BigInteger& BigInteger::operator=(const BigInteger& rhs)
{
    sign = rhs.sign;
    magnitude = rhs.magnitude;
    return *this;
}

// I/O operator function
string BigInteger::toString() const
{
    ostringstream oss;

    if (sign == neg) {
        oss << "-";
    }

    for(magnitudeType::const_iterator it = magnitude.end()-1; it >= magnitude.begin(); --it)
        oss << *it;

    return oss.str();
}

BigInteger::magnitudeType BigInteger::addCore(const magnitudeType &lhs, const magnitudeType &rhs)
{
    sizeType lsize = lhs.size();
    sizeType rsize = rhs.size();
    sizeType n = std::max(lsize, rsize);

    int lv, rv, tv;
    magnitudeType temp;
    int carry = 0;

    for (sizeType i = 0; i < n; ++i) {
        lv = (i>=lsize) ? 0 : lhs[i];
        rv = (i>=rsize) ? 0 : rhs[i];
        tv = lv + rv + carry;
        carry = tv/10;
        tv %= 10;
        temp.push_back(tv);
    }

    if (carry) {
        temp.push_back(carry);
    }

    return temp;
}

BigInteger::magnitudeType BigInteger::subCore(const magnitudeType &lhs, const magnitudeType &rhs)
{
    sizeType lsize = lhs.size();
    sizeType rsize = rhs.size();

    int lv, rv, tv;
    magnitudeType temp;
    int carry = 1;

    for (sizeType i=0; i<lsize; ++i) {
        lv = lhs[i];
        rv = (i>=rsize) ? 0 : rhs[i];
        tv = lv - rv + carry + 10 - 1;
        carry = tv/10;
        tv %= 10;
        temp.push_back(tv);
    }

    remove_leading_zeros(temp);

    return temp;
}

BigInteger::magnitudeType BigInteger::multCore(const magnitudeType &lhs, const magnitudeType &rhs)
{

    int lv, rv, tv;
    sizeType lsize, rsize, tsize;

    lsize = lhs.size();
    rsize = rhs.size();
    tsize = lsize + rsize;

    magnitudeType temp(tsize);
    int carry;

    for (sizeType i = 0; i < rsize; ++i) {
        rv = rhs[i];
        if (rhs[i]==0)
            continue;
        carry = 0;
        for (sizeType j = 0; j < lsize; ++j) {
            lv = lhs[j];
            tv = rv*lv + temp[i+j] + carry;
            carry = tv/10;
            temp[i+j] = tv%10;
        }
        temp[i+lsize] = carry;
    }

    remove_leading_zeros(temp);

    return temp;
}

bool BigInteger::magnitudeLess(const BigInteger& rhs) const
{
    sizeType lsize = magnitude.size();
    sizeType rsize = rhs.magnitude.size();

    if (lsize < rsize) {
        return 1;
    } else if (lsize > rsize) {
        return 0;
    } else {
        for (int i = rsize - 1; i >= 0; --i) {
            if(magnitude[i] < rhs.magnitude[i])
                return 1;
        }
    }
    return 0;
}

bool BigInteger::magnitudeEqual(const BigInteger& rhs) const
{
    sizeType lsize = magnitude.size();
    sizeType rsize = rhs.magnitude.size();

    if (lsize == rsize) {
        for (sizeType i= rsize - 1; i >= 0; --i) {
            if(magnitude[i] != rhs.magnitude[i])
                return 0;
        }
        return 1;
    }
    return 0;
}

// arithmetic assignment operators
const BigInteger& BigInteger::operator+=(const BigInteger & rhs)
{
    if (sign != rhs.sign) {
        if (magnitudeLess(rhs)) {
            magnitude = subCore(rhs.magnitude, magnitude);
            sign = rhs.sign;
        } else {
            magnitude = subCore(magnitude, rhs.magnitude);
        }
    } else {
        magnitude = addCore(magnitude, rhs.magnitude);
    }

    return *this;
}

const BigInteger& BigInteger::operator-=(const BigInteger & rhs)
{
    if (sign == rhs.sign) {
        if (magnitudeLess(rhs)) {
            magnitude = subCore(rhs.magnitude, magnitude);
            sign = rhs.sign == neg ? pos : neg;
        } else {
                magnitude = subCore(magnitude, rhs.magnitude);
        }
    } else {
        magnitude = addCore(magnitude, rhs.magnitude);
    }

    return *this;
}

const BigInteger& BigInteger::operator *= (const BigInteger& rhs)
{
    if (sign != rhs.sign) {
        sign = neg;
    } else {
        sign = pos;
    }

    magnitude = multCore(magnitude, rhs.magnitude);

    return *this;
}

// relational operators
bool BigInteger::less(const BigInteger& rhs) const
{
        if (sign == pos && rhs.sign != pos) {
        return 0;
    }

    if (sign == neg && rhs.sign != neg) {
        return 1;
    }

    if (sign == pos && rhs.sign == pos) {
        return magnitudeLess(rhs);
    }

    if (sign == neg && rhs.sign == neg) {
        return !magnitudeLess(rhs);
    }

    return 0;
}

bool BigInteger::equal(const BigInteger& rhs) const
{
    if (sign == rhs.sign) {
        return magnitudeEqual(rhs);
    } else {
        return 0;
    }
}

// add operator
BigInteger operator + (const BigInteger& lhs, const BigInteger & rhs)
{
    return BigInteger(lhs) += rhs;
}

// substract operators
BigInteger operator - (const BigInteger& lhs, const BigInteger & rhs)
{
    return BigInteger(lhs) -= rhs;
}

// multiply operator
BigInteger operator * (const BigInteger& lhs, const BigInteger& rhs)
{
    return BigInteger(lhs) *= rhs;
}

// less than operator
bool operator <  (const BigInteger& lhs, const BigInteger& rhs)
{
    return lhs.less(rhs);
}

// equal operator
bool operator == (const BigInteger& lhs, const BigInteger& rhs)
{
    return lhs.equal(rhs);
}

// IO operators
ostream& operator<<(ostream& out, const BigInteger& rhs)
{
    out << rhs.toString();
    return out;
}

// Operators for BigInteger with Int types are not implemented here

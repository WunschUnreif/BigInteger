#ifndef __BIG_INTEGER_H
#define __BIG_INTEGER_H

#include <iostream>
#include <utility>
#include <cstring>

class BigInteger {
    friend std::istream& operator >> (std::istream& is, BigInteger& b);
    friend std::ostream& operator << (std::ostream& os, const BigInteger& b);
protected: 
    const static int BASE = 10000;
    int * data;
    int maxSize = 10000;
    int length = 0;
    bool positive;
public:
    BigInteger(long long number, int size = 1000);
    BigInteger(int number, int size = 10000);
    explicit BigInteger(const char* numstr);

    BigInteger(const BigInteger& b);
    BigInteger operator=(const BigInteger& b);

    ~BigInteger();

    operator long long () const;
    operator int () const;

    BigInteger operator+(const BigInteger& b) const;
    BigInteger operator-() const;
    BigInteger operator-(const BigInteger& b) const;
    BigInteger operator*(const BigInteger& b) const;
    BigInteger operator/(const BigInteger& b) const;
    BigInteger operator%(const BigInteger& b) const;
    BigInteger operator^(int exp) const;

    BigInteger& operator++();
    BigInteger operator++(int);
    BigInteger& operator--();
    BigInteger operator--(int);

    BigInteger abs() const;
    
    bool operator==(const BigInteger& b) const;
    bool operator<(const BigInteger& b) const;
    bool operator!=(const BigInteger& b) const;
    bool operator>(const BigInteger& b) const;
    bool operator<=(const BigInteger& b) const;
    bool operator>=(const BigInteger& b) const;

protected:
    void clear();
    std::pair<BigInteger, BigInteger> divide(const BigInteger& b) const;
    void trim();
};

#endif
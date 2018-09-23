#include "BigInteger.hpp"

std::istream& operator >> (std::istream& is, BigInteger& b) {
    char * str = new char[4000];
    is >> str;
    b = BigInteger(str);
    delete [] str;
    return is;
}

std::ostream& operator << (std::ostream& os, const BigInteger& b) {
    if(b.length == 0) return os;
    if(!b.positive) os << '-';
    os << b.data[b.length - 1];
    for(int i = b.length - 2; i >= 0; --i) {
        int num = b.data[i];
        if(num == 0) os << "0000";
        else {
            while((num *= 10) < b.BASE) os << '0';
            os << b.data[i];
        }
    }
    return os;
}

BigInteger::BigInteger(long long number, int size) {
    data = new int[size];
    maxSize = size;
    if(number >= 0) {
        positive = true;
    } else {
        positive = false;
        number = -number;
    }
    if(number == 0) data[length++] = 0;
    while(number) {
        data[length++] = number % BASE;
        number /= BASE;
    }
}

BigInteger::BigInteger(int number, int size) {
    data = new int[size];
    maxSize = size;
    if(number >= 0) {
        positive = true;
    } else {
        positive = false;
        number = -number;
    }
    if(number == 0) data[length++] = 0;
    while(number) {
        data[length++] = number % BASE;
        number /= BASE;
    }
}

unsigned int convert(const char* str, int len = 4) {
    int ans = 0;
    for(int i = 0; i < len; ++i) {
        ans = ans * 10 + str[i] - '0';
    }
    return ans;
}

BigInteger::BigInteger(const char* numstr) {
    data = new int[maxSize];
    if(numstr[0] == '-') {
        positive = false;
        numstr++;
    } else {
        positive = true;
    }
    while(*numstr == '0') numstr++;
    int i;
    for(i = strlen(numstr); i >= 4; i -= 4) {
        data[length++] = convert(numstr + i - 4);
    }
    if(i) data[length++] = convert(numstr, i);
}

BigInteger::BigInteger(const BigInteger& b) {
    maxSize = b.maxSize;
    positive = b.positive;
    data = new int[maxSize];
    for(length = 0; length < b.length; ++length) {
        data[length] = b.data[length];
    }
}

BigInteger BigInteger::operator=(const BigInteger& b) {
    if(this != &b) {
        clear();
        maxSize = b.maxSize;
        positive = b.positive;
        data = new int[maxSize];
        for(length = 0; length < b.length; ++length) {
            data[length] = b.data[length];
        }
    }
    return *this;
}

BigInteger::~BigInteger() {
    clear();
}

BigInteger::operator int() const {
    int ans = 0;
    for(int i = length - 1; i >= 0; --i) {
        ans = ans * BASE + data[i];
    }
    if(!positive) ans = -ans;
    return ans;
}

BigInteger::operator long long() const {
    long long ans = 0;
    for(int i = length - 1; i >= 0; --i) {
        ans = ans * BASE + data[i];
    }
    if(!positive) ans = -ans;
    return ans;
}

void BigInteger::clear() {
    delete [] data;
    length = 0;
}

void BigInteger::trim() {
    while(length > 1 && data[length - 1] == 0) length--;
    if(length == 1 && data[0] == 0) positive = true;
}

BigInteger BigInteger::abs() const {
    BigInteger ans = *this;
    ans.positive = true;
    return ans;
}

BigInteger BigInteger::operator+(const BigInteger& b) const {
    BigInteger ans = 0;
    ans.length = 0;
    if((b.positive && positive) || (!b.positive && !positive)) {
        ans.positive = positive;
        int carry = 0;
        while(ans.length < length && ans.length < b.length) {
            ans.data[ans.length] = (carry + data[ans.length] + b.data[ans.length]) % BASE;
            carry = (carry + data[ans.length] + b.data[ans.length]) / BASE;
            ans.length++;
        }
        while(ans.length < length) {
            ans.data[ans.length] = (carry + data[ans.length]) % BASE;
            carry = (carry + data[ans.length]) / BASE;
            ans.length++;
        }
        while(ans.length < b.length) {
            ans.data[ans.length] = (carry + b.data[ans.length]) % BASE;
            carry = (carry + b.data[ans.length]) / BASE;
            ans.length++;
        }
        if(carry) ans.data[ans.length++] = 1;
    } else {
        if(abs() < b.abs()) return(b + *this);
        int carry = 0;
        while(ans.length < length && ans.length < b.length) {
            ans.data[ans.length] = (data[ans.length] - carry + BASE - b.data[ans.length]) % BASE;
            carry = 1 - (data[ans.length] - carry + BASE - b.data[ans.length]) / BASE;
            ans.length++;
        }
        while(ans.length < length) {
            ans.data[ans.length] = (data[ans.length] - carry + BASE) % BASE;
            carry = 1 - (data[ans.length] - carry + BASE) / BASE;
            ans.length++;
        }
        while(ans.length < b.length) {
            ans.data[ans.length] = (0 - carry + BASE - b.data[ans.length]) % BASE;
            carry = 1 - (0 - carry + BASE - b.data[ans.length]) / BASE;
            ans.length++;
        }
        if(positive) ans.positive = true;
        else ans.positive = false;
    }
    ans.trim();
    if(ans.length == 1 && ans.data[0] == 0) ans.positive = true;
    return ans;
}

BigInteger BigInteger::operator-() const {
    BigInteger ans = *this;
    ans.positive = !ans.positive;
    return ans;
}

BigInteger BigInteger::operator-(const BigInteger& b) const {
    return *this + (-b);
}

BigInteger BigInteger::operator*(const BigInteger& b) const {
    BigInteger ans = 0;
    ans.positive = !(positive ^ b.positive);
    for(int i = 0; i < length; ++i) {
        BigInteger tmp = 0;
        tmp.length = 0;
        for(int j = 0; j < i; ++j) tmp.data[tmp.length++] = 0;
        int carry = 0;
        for(int j = 0; j < b.length; ++j) {
            tmp.data[tmp.length++] = (carry + data[i] * b.data[j]) % BASE;
            carry = (carry + data[i] * b.data[j]) / BASE;
        }
        if(carry) tmp.data[tmp.length++] = carry;
        ans = ans + tmp;
    }
    ans.trim();
    return ans;
}

BigInteger BigInteger::operator^(int exp) const {
    BigInteger ans = 1, tmp = *this;
    while(exp) {
        if((exp & 0x01) == 1) ans = ans * tmp;
        tmp = tmp * tmp;
        exp = exp >> 1;
    }
    return ans;
}

BigInteger& BigInteger::operator++() {
    *this = *this + BigInteger(1);
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger ans = *this;
    *this = *this + BigInteger(1);
    return ans;
}

BigInteger& BigInteger::operator--() {
    *this = *this - BigInteger(1);
    return *this;
}

BigInteger BigInteger::operator--(int) {
    BigInteger ans = *this;
    *this = *this - BigInteger(1);
    return ans;
}

bool BigInteger::operator<(const BigInteger& b) const {
    if(!positive && b.positive) return true;
    if(positive && !b.positive) return false;
    bool less = false;
    if(length < b.length) less = true;
    else if(length > b.length) less = false;
    else {
        for(int i = length - 1; i >= 0; --i) {
            if(data[i] < b.data[i]) {
                less = true;
                break;
            }
            if(data[i] > b.data[i]) {
                less = false;
                break;
            }
        }
    }
    if(!positive) less = !less;
    return less;
}

bool BigInteger::operator==(const BigInteger& b) const {
    if(positive != b.positive || length != b.length) return false;
    for(int i = 0; i < length; ++i) {
        if(data[i] != b.data[i]) return false;
    }
    return true;
}

bool BigInteger::operator!=(const BigInteger& b) const {return !(*this == b);}
bool BigInteger::operator<=(const BigInteger& b) const {return *this < b || *this == b;}
bool BigInteger::operator>(const BigInteger& b) const {return !(*this < b || *this == b);}
bool BigInteger::operator>=(const BigInteger& b) const {return !(*this < b);}

//test
int main() {
    BigInteger a = 0, b = 1;
    std::cin >> a >> b;
    std::cout << (a^b) << std::endl;
}

#ifndef BIGNUMINT_H
#define BIGNUMINT_H

#include <vector>
#include <cstdlib>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>

typedef std::vector<int> SVI;
typedef std::string STR;

#define INIT_VECTOR(type, name, ...) \
static const type name##_a[] = __VA_ARGS__; \
std::vector<type> name(name##_a, name##_a + sizeof(name##_a) / sizeof(*name##_a))

class BignumInt
{
private:
    bool sign;
    SVI num_list;

    SVI _do_add(SVI A, SVI B);
    SVI _do_sub(SVI A, SVI B);
    SVI _do_mul(SVI A, SVI B);
    SVI _do_div(SVI A, SVI B);
    SVI _do_div_rem(SVI A, SVI B);

    //bool SVIeq(SVI A, SVI B);
    bool SVIcomp(SVI A, SVI B);

public:
    BignumInt operator+(const BignumInt& other);
    BignumInt operator-(const BignumInt& other);
    BignumInt operator*(const BignumInt& other);
    BignumInt operator/(const BignumInt& other);
    BignumInt operator%(const BignumInt& other);

    STR str(); // print out current BignumInt as a cstring.
    void SetList(SVI input_list);
    SVI GetList();
    bool GetSign();
    void SetSign(bool input_sign);
    void Set(int num);
    void Set(char* num);
    void Set(const char* num);

    bool operator>(const BignumInt& other);
    bool operator<(const BignumInt& other);
    bool operator>=(const BignumInt& other);
    bool operator<=(const BignumInt& other);
    bool operator==(const BignumInt& other);
    bool operator!=(const BignumInt& other);

    BignumInt& operator=(int num);
    BignumInt& operator=(char* num);
    BignumInt& operator=(const char* num);

    //void printSVI(SVI array);

    BignumInt();
    BignumInt(int num);
    BignumInt(const char* num);
    BignumInt(char* num);
    virtual ~BignumInt();
};

#endif
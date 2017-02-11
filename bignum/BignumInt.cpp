#ifndef BIGNUMINT_CPP
#define BIGNUMINT_CPP

#include "BignumInt.h"

/******************************************
              Private Members
*******************************************/
// Operations for positive (x) positive
SVI BignumInt::_do_add(SVI A, SVI B)
{
    SVI ret;
    int i_A = A.size() - 1;
    int i_B = B.size() - 1;
    int an; int bn; int rn;
    int carry = 0;

    while(1) {
        an = 0; bn = 0; rn = 0;
        if (i_A >= 0 && i_B >= 0) {
            an = A.at(i_A);
            bn = B.at(i_B);
        }
        else if (i_A >= 0 && i_B < 0) {
            an = A.at(i_A);
            //bn = 0;
        }
        else if (i_A < 0 && i_B >= 0) {
            //an = 0;
            bn = B.at(i_B);
        }
        else if (i_A <= -1 || i_B <= -1)
            break;

        rn = an + bn + carry;
        if (rn > 9) {
            carry = rn/10;
            rn = rn%10;
        }
        else
            carry = 0;

        if (ret.size() == 0)
            ret.push_back(rn);
        else
            ret.insert(ret.begin(), rn);

        i_A--; i_B--;
    }

    if (carry > 0) {
        ret.insert(ret.begin(), carry);
    }

    return ret;
}

SVI BignumInt::_do_sub(SVI A, SVI B)
{
    SVI ret;
    int i_A = A.size() - 1;
    int i_B = B.size() - 1;
    int an; int bn; int rn;
    int carry = 0;

    while (1) {
        an = 0; bn = 0; rn = 0;
        if (i_A >= 0 && i_B >= 0) {
            an = A.at(i_A);
            bn = B.at(i_B);
        }
        else if (i_A >= 0 && i_B < 0) {
            an = A.at(i_A);
            //bn = 0;
        }
        else if (i_A < 0 && i_B >= 0) {
            //an = 0;
            bn = B.at(i_B);
        }
        else if (i_A <= -1 || i_B <= -1)
            break;

        rn = an - bn - carry;

        if (rn < 0) {
            carry = 1;
            rn = 10 + rn;
        }
        else
            carry = 0;

        if (ret.size() == 0)
            ret.push_back(rn);
        else
            ret.insert(ret.begin(), rn);

        i_A--; i_B--;
    }

    if (ret.at(0) == 0)
        ret.erase(ret.begin());

    return ret;
}

SVI BignumInt::_do_mul(SVI A, SVI B)
{
    SVI ret; ret.push_back(0);
    SVI temp;
    int an; int bn; int rn; int carry = 0;
    int i_A;
    int i_B;
    int A_len = A.size();
    int B_len = B.size();
    int i;

    for (i_B = B_len - 1; i_B >= 0; i_B--) {
        bn = B.at(i_B);
        if (bn > 0) {
            for (i_A = A_len - 1; i_A >= 0; i_A--) {
                an = A.at(i_A);
                rn = an*bn + carry;
                if (rn > 9) {
                    carry = rn/10;
                    rn = rn%10;
                }
                else
                    carry = 0;

                if (temp.size() == 0)
                    temp.push_back(rn);
                else
                    temp.insert(temp.begin(), rn);
            }
            if (carry > 0) {
                temp.insert(temp.begin(), carry);
                carry = 0;
            }
        }
        else {
            // do nothing
        }

        for (i = 0; i < (B_len - 1 - i_B); i++)
            temp.push_back(0);

        ret = this->_do_add(ret, temp);
        temp.clear();
    }

    return ret;
}

SVI BignumInt::_do_div(SVI A, SVI B)
{
    //SVI ret; ret.push_back(0);
    //int i_A; int i_B;
    //int A_len = A.size(); int B_len = B.size();
    //int an, bn, rn;
    //int carry = 0;
    SVI zero(1); zero[0] = 0;
    SVI one(1); one[0] = 1;
    SVI Index; Index.push_back(0);

    if (B == zero) {
        throw std::invalid_argument("Divide by zero!!\n");
    }

    while (SVIcomp(A, B) || A == B) {
        A = _do_sub(A, B);
        Index = _do_add(Index, one);
    }

    return Index;
}

SVI BignumInt::_do_div_rem(SVI A, SVI B)
{
    SVI zero(1); zero[0] = 0;
    SVI one(1); one[0] = 1;
    SVI Index; Index.push_back(0);

    if (B == zero) {
        throw std::invalid_argument("Divide by zero!!\n");
    }

    if (!SVIcomp(A, B) || A == B)
        return zero;

    while (SVIcomp(A, B) || A == B) {
        A = _do_sub(A, B);
        Index = _do_add(Index, one);
    }

    return A;
}

/*
bool BignumInt::SVIeq(SVI A, SVI B)
{

}
*/

bool BignumInt::SVIcomp(SVI A, SVI B)
{
    int A_len = A.size();
    int B_len = B.size();
    int i;

    if (A_len > B_len)
        return true;
    else if (A_len < B_len)
        return false;
    else {
        for (i = 0; i < A_len; i++) {
            if (A.at(i) < B.at(i))
                return false;
            else if (A.at(i) > B.at(i))
                return true;
            else
                continue;
        }
        return false;
    }
}

/******************************************
                Utilities
*******************************************/
STR BignumInt::str()
{
    //char* ret;
    STR ret;
    int num_list_len = num_list.size();
    int i;

    if (num_list_len) {
        if (sign) {
            //ret = new char[num_list_len];
            for (i = 0; i < num_list_len; i++)
                ret.push_back((char)(num_list.at(i) + (int)'0'));
        }
        else {
            //ret = new char[num_list_len+1];
            ret.push_back('-');
            for (i = 0; i < num_list_len; i++)
                ret.push_back((char)(num_list.at(i) + (int)'0'));
        }
        //ret[strlen(ret)] = '\0';
        return ret;
    }
    else {
        return NULL;
    }

}

void BignumInt::SetList(SVI input_list)
{
    this->num_list.clear();
    this->num_list = input_list;
}

SVI BignumInt::GetList()
{
    return this->num_list;
}

bool BignumInt::GetSign()
{
    return this->sign;
}

void BignumInt::SetSign(bool input_sign)
{
    this->sign = input_sign;
}


void BignumInt::Set(int num)
{
    if (num >= 0)
        sign = true;
    else {
        sign = false;
        num *= -1;
    }

    this->num_list.clear();
    if (num == 0) num_list.push_back(0);
    else {
        for (; num; num /= 10) {
            num_list.push_back(num%10);
        }
        std::reverse(num_list.begin(), num_list.end());
    }
}

void BignumInt::Set(char* num)
{
    if (num == NULL) {
        throw std::invalid_argument("Empty string received!!\n");
        //exit(-1);
    }

    if (*num != '-')
        sign = true;
    else {
        sign = false;
        num++;
    }

    int num_len = strlen(num);
    int i;

    num_list.clear();
    for (i = 0; i < num_len; i++) {
        num_list.push_back((int)(num[i]-(int)'0'));
    }
}

void BignumInt::Set(const char* num)
{
    if (num == NULL) {
        throw std::invalid_argument("Empty string received!!\n");
        //exit(-1);
    }

    if (*num != '-')
        sign = true;
    else {
        sign = false;
        num++;
    }

    int num_len = strlen(num);
    int i;

    num_list.clear();
    for (i = 0; i < num_len; i++) {
        num_list.push_back((int)(num[i]-(int)'0'));
    }

}

/*
void BignumInt::printSVI(SVI array)
{
    unsigned long long i;
    unsigned long long a_size = array.size();

    for (i = 0; i < a_size; i++) {
        std::cout << array.at(i);
    }
    std::cout<<std::endl;
}
*/

bool BignumInt::operator>(const BignumInt& other)
{
    if (this->sign == true && other.sign == false)
        return true;
    else if (this->sign == false && other.sign == true)
        return false;
    else if (this->sign == true && other.sign == true)
        return SVIcomp(this->num_list, other.num_list);
    else if (this->sign == false && other.sign == false)
        return !SVIcomp(this->num_list, other.num_list);
    else
        return false;
}

bool BignumInt::operator<(const BignumInt& other)
{
    if (this->sign == true && other.sign == false)
        return false;
    else if (this->sign == false && other.sign == true)
        return true;
    else if (this->sign == true && other.sign == true)
        return !SVIcomp(this->num_list, other.num_list);
    else if (this->sign == false && other.sign == false)
        return SVIcomp(this->num_list, other.num_list);
    else
        return false;
}

bool BignumInt::operator>=(const BignumInt& other)
{
    if (this->sign == true && other.sign == false)
        return true;
    else if (this->sign == false && other.sign == true)
        return false;
    else if (this->sign == true && other.sign == true)
        return SVIcomp(this->num_list, other.num_list);
    else if (this->sign == false && other.sign == false)
        return !SVIcomp(this->num_list, other.num_list);
    else
        return true;
}

bool BignumInt::operator<=(const BignumInt& other)
{
    if (this->sign == true && other.sign == false)
        return false;
    else if (this->sign == false && other.sign == true)
        return true;
    else if (this->sign == true && other.sign == true)
        return !SVIcomp(this->num_list, other.num_list);
    else if (this->sign == false && other.sign == false)
        return SVIcomp(this->num_list, other.num_list);
    else
        return true;
}

bool BignumInt::operator==(const BignumInt& other)
{
    if (this->sign == other.sign)
        return (this->num_list == other.num_list);
    else
        return false;
}

bool BignumInt::operator!=(const BignumInt& other)
{
    if (this->sign == other.sign)
        return !(this->num_list == other.num_list);
    else
        return true;
}

BignumInt& BignumInt::operator=(int num)
{
    this->Set(num);
    return *this;
}

BignumInt& BignumInt::operator=(char* num)
{
    this->Set(num);
    return *this;
}

BignumInt& BignumInt::operator=(const char* num)
{
    this->Set(num);
    return *this;
}

/******************************************
          Arithematic Operations
*******************************************/

BignumInt BignumInt::operator+(const BignumInt& other)
{
    BignumInt ret(0);
    if (this->sign == true && other.sign == true) {
        ret.SetList(_do_add(this->num_list, other.num_list));
        ret.SetSign(true);
    }
    else if (this->sign == false && other.sign == false) {
        ret.SetList(_do_add(this->num_list, other.num_list));
        ret.SetSign(false);
    }
    else if (this->sign == true && other.sign == false) {
        if (SVIcomp(this->num_list, other.num_list)) {
            ret.SetList(_do_sub(this->num_list, other.num_list));
            ret.SetSign(true);
        }
        else if (this->num_list == other.num_list)
            ret.Set(0);
        else {
            ret.SetList(_do_sub(other.num_list, this->num_list));
            ret.SetSign(false);
        }
    }
    else if (this->sign == false && other.sign == true) {
        if (SVIcomp(this->num_list, other.num_list)) {
            ret.SetList(_do_sub(this->num_list, other.num_list));
            ret.SetSign(false);
        }
        else if (this->num_list == other.num_list)
            ret.Set(0);
        else {
            ret.SetList(_do_sub(other.num_list, this->num_list));
            ret.SetSign(true);
        }
    }

    return ret;
}

BignumInt BignumInt::operator-(const BignumInt& other)
{
    BignumInt ret(0);

    if (this->sign == true && other.sign == false) {
        ret.SetList(_do_add(this->num_list, other.num_list));
        ret.SetSign(true);
    }
    if (this->sign == false && other.sign == true) {
        ret.SetList(_do_add(this->num_list, other.num_list));
        ret.SetSign(false);
    }
    if (this->sign == true && other.sign == true) {
        if (SVIcomp(this->num_list, other.num_list)) {
            ret.SetList(_do_sub(this->num_list, other.num_list));
            ret.SetSign(true);
        }
        else if (this->num_list == other.num_list)
            ret.Set(0);
        else {
            ret.SetList(_do_sub(other.num_list, this->num_list));
            ret.SetSign(false);
        }
    }
    if (this->sign == false && other.sign == false) {
        if (SVIcomp(this->num_list, other.num_list)) {
            ret.SetList(_do_sub(this->num_list, other.num_list));
            ret.SetSign(false);
        }
        else if (this->num_list == other.num_list)
            ret.Set(0);
        else {
            ret.SetList(_do_sub(other.num_list, this->num_list));
            ret.SetSign(true);
        }
    }

    return ret;
}

BignumInt BignumInt::operator*(const BignumInt& other)
{
    BignumInt ret(0);
    const int zero_ary[] = {0};
    const int one_ary[] = {1};
    SVI zero(zero_ary, zero_ary + sizeof(zero_ary)/sizeof(zero_ary[0]));
    SVI one(one_ary, one_ary + sizeof(one_ary)/sizeof(one_ary[0]));

    if (this->num_list == zero || other.num_list == zero)
        ret.Set(0);
    else if (this->num_list == one && other.num_list != one) {
        ret.SetSign(other.sign);
        ret.SetList(other.num_list);
    }
    else if (this->num_list != one && other.num_list == one) {
        ret.SetSign(this->sign);
        ret.SetList(this->num_list);
    }
    else if (this->num_list == one && other.num_list == one) {
        ret.Set(1);
    }
    else {
        ret.SetList(_do_mul(this->num_list, other.num_list));
        if (this->sign == other.sign)
            ret.SetSign(true);
        else
            ret.SetSign(false);
    }

    return ret;
}

BignumInt BignumInt::operator/(const BignumInt& other)
{
    BignumInt ret(0);
    const int zero_ary[] = {0};
    SVI zero(zero_ary, zero_ary + sizeof(zero_ary)/sizeof(zero_ary[0]));
    const int one_ary[] = {1};
    SVI one(one_ary, one_ary + sizeof(one_ary)/sizeof(one_ary[0]));

    if (other.num_list == zero)
        throw std::invalid_argument("Divide by zero!!!\n");

    if (this->num_list == zero)
        return ret;
    else if (other.num_list == one)
        return *this;
    else {
        ret.SetList(_do_div(this->num_list, other.num_list));
        if (this->sign == other.sign)
            ret.SetSign(true);
        else
            ret.SetSign(false);
        return ret;
    }
    return ret;
}

BignumInt BignumInt::operator%(const BignumInt& other)
{
    BignumInt ret(0);
    const int zero_ary[] = {0};
    SVI zero(zero_ary, zero_ary + sizeof(zero_ary)/sizeof(zero_ary[0]));
    const int one_ary[] = {1};
    SVI one(one_ary, one_ary + sizeof(one_ary)/sizeof(one_ary[0]));

    if (other.num_list == zero)
        throw std::invalid_argument("Divide by zero!!!\n");

    if (this->num_list == zero)
        return ret;
    else if (other.num_list == one)
        return *this;
    else {
        ret.SetList(_do_div_rem(this->num_list, other.num_list));
        if (this->sign == other.sign)
            ret.SetSign(true);
        else
            ret.SetSign(false);
        return ret;
    }
    return ret;
}


/******************************************
       Constructors and Destructors
*******************************************/

BignumInt::BignumInt()
{
    sign = true;
    num_list = SVI(0);
}

BignumInt::BignumInt(int num)
{
    if (num >= 0)
        sign = true;
    else {
        sign = false;
        num *= -1;
    }

    if (num == 0) num_list.push_back(0);
    else {
        for (; num; num /= 10) {
            num_list.push_back(num%10);
        }
        std::reverse(num_list.begin(), num_list.end());
    }
}

BignumInt::BignumInt(const char* num)
{
    if (num == NULL) {
        throw std::invalid_argument("Empty string received!!\n");
        //exit(-1);
    }

    if (*num != '-')
        sign = true;
    else {
        sign = false;
        num++;
    }

    int num_len = strlen(num);
    int i;

    for (i = 0; i < num_len; i++) {
        num_list.push_back((int)(num[i]-(int)'0'));
    }

}

BignumInt::BignumInt(char* num)
{
    if (num == NULL) {
        throw std::invalid_argument("Empty string received!!\n");
        //exit(-1);
    }

    if (*num != '-')
        sign = true;
    else {
        sign = false;
        num++;
    }

    int num_len = strlen(num);
    int i;

    for (i = 0; i < num_len; i++) {
        num_list.push_back((int)(num[i]-(int)'0'));
    }

}

BignumInt::~BignumInt()
{
    num_list.clear();
}

#endif
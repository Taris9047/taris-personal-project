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
		if (i_A >= 0 && i_B >= 0) {
			an = A.at(i_A);
			bn = B.at(i_B);
		}
		else if (i_A >= 0 && i_B < 0) {
			an = A.at(i_A);
			bn = 0;
		}
		else if (i_A < 0 && i_B >= 0) {
			an = 0;
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
		if (i_A >= 0 && i_B >= 0) {
			an = A.at(i_A);
			bn = B.at(i_B);
		}
		else if (i_A >= 0 && i_B < 0) {
			an = A.at(i_A);
			bn = 0;
		}
		else if (i_A < 0 && i_B >= 0) {
			an = 0;
			bn = B.at(i_B);
		}
		else if (i_A <= -1 || i_B <= -1)
			break;

		rn = an - bn - carry;
		if (rn < 0) {
			carry = 1;
			rn = rn*(-1)%10;
		}
		else
			carry = 0;

		if (ret.size() == 0)
			ret.push_back(rn);
		else
			ret.insert(ret.begin(), rn);
	}

	if (ret.at(0) == 0)
		ret.erase(ret.begin());

	return ret;
}

SVI BignumInt::_do_mul(SVI A, SVI B)
{
	SVI ret;
	return ret;
}

SVI BignumInt::_do_div(SVI A, SVI B)
{
	SVI ret;
	return ret;
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
char* BignumInt::c_str()
{
	char* ret;
	int num_list_len = num_list.size();
	int i;

	if (num_list_len) {
		if (sign) {
			ret = new char[num_list_len];
			for (i = 0; i < num_list_len; i++)
				ret[i] = (char)(num_list.at(i) + (int)'0');
		}
		else {
			ret = new char[num_list_len+1];
			ret[0] = '-';
			for (i = 0; i < num_list_len; i++)
				ret[i+1] = (char)(num_list.at(i) + (int)'0');
		}
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
BignumInt BignumInt::abs()
{
	BignumInt ret(this->c_str());
	ret.sign = true;
	return ret;
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

	}
	else if (this->sign == false && other.sign == true) {

	}

	return ret;
}

BignumInt BignumInt::operator-(const BignumInt& other)
{
	BignumInt ret(0);

	return ret;
}

BignumInt BignumInt::operator*(const BignumInt& other)
{
	BignumInt ret(0);

	return ret;
}

BignumInt BignumInt::operator/(const BignumInt& other)
{
	BignumInt ret(0);

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
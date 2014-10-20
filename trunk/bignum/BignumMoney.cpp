#ifndef BIGNUMMONEY_CPP
#define BIGNUMMONEY_CPP

#include "BignumMoney.h"

/******************************************
           String Output Methods
*******************************************/

STR BignumMoney::str()
{
	STR money_str;
	if (!this->sign)
		money_str.push_back('-');

	unsigned int i = 0;
	unsigned int j = 0;
	do {
		if (i == 2)
			money_str.insert(money_str.begin(), '.');
		else if (i >= 6 && i%3 == 0)
			money_str.insert(money_str.begin(), ',');
		else {
			money_str.insert(money_str.begin(), \
				(char)(num_list.at(j) + (int)'0'));
			j++;
			//std::cout << j << std::endl;
		}
		i++;
	} while(j < num_list.size());

	if (money_str.at(0) == ',') {
		money_str.erase(money_str.at(0));
	}

	if (money_str.size() < 4) {
		
	}

	money_str = money_str + " " + this->currency;
	return money_str;

}

void BignumMoney::SetNum(int num)
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

void BignumMoney::SetSign(bool new_sign)
{
	this->sign = new_sign;
}

void BignumMoney::SetCurrency(STR currency_str)
{
	this->currency = currency_str;
}

void BignumMoney::SetComma(unsigned int commaloc)
{
	this->comma_separation = commaloc;
}

/******************************************
       Constructors and Destructors
*******************************************/

BignumMoney::BignumMoney()
{
	sign = true;
	INIT_VECTOR(int, num_list, {0});
	currency = "USD";
	comma_separation = 3;
}

BignumMoney::BignumMoney(int num, STR currency_str)
{
	//INIT_VECTOR(int, num_list, {0});
	currency = currency_str;
	comma_separation = 3;

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

BignumMoney::BignumMoney(char* num, STR currency_str)
{
	currency = currency_str;
	comma_separation = 3;

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

BignumMoney::BignumMoney(const char* num, STR currency_str)
{
	currency = currency_str;
	comma_separation = 3;

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


#endif
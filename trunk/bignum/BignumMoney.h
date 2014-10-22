#ifndef BIGNUMMONEY_H
#define BIGNUMMONEY_H

#include "BignumInt.h"

class BignumMoney : public virtual BignumInt
{
private:
	SVI num_list;
	bool sign;
	std::string currency;
	int comma_separation;

public:
	STR str(); // returns money value as string.
	void SetVal(int num);
	void SetVal(float num);
	void SetSign(bool new_sign);
	void SetCurrency(STR currency_str);
	void SetComma(unsigned int commaloc);

	// Constructors and destructors
	BignumMoney();
	BignumMoney(int num, STR currency_str);
	BignumMoney(char* num, STR currency_str);
	BignumMoney(const char* num, STR currency_str);

};

#endif
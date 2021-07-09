#pragma once
#ifndef CHECKINGACCOUNT_H
#define CHECKINGACCOUNT_H
#include "Account.h"

class CheckingAccount : public Account {
public:
	//CheckingAccount();
	CheckingAccount(const double&, const double&);
	void credit(double); // add an amount to the account balance
	bool debit(double); // subtract an amount from the account balance
private:
	double fee;
};

#endif
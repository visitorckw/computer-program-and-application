#include "CheckingAccount.h"
#include <iostream>
using namespace std;

//CheckingAccount::CheckingAccount(){}
CheckingAccount::CheckingAccount(const double& balance, const double& fee) {
	if (balance >= 0.0)
		setBalance(balance);
	else { // otherwise, output message and set balance to 0.0
		cout << "Error: Initial balance cannot be negative." << endl;
		setBalance(0.0);
	}
	if (fee >= 0.0)
		this->fee = fee;
	else { // otherwise, output message and set balance to 0.0
		cout << "Error: Initial balance cannot be negative." << endl;
		this->fee = 0.0;
	}
}
void CheckingAccount::credit(double amount) {
	setBalance(getBalance() + amount - fee);
}
bool CheckingAccount::debit(double amount) {
	if (amount + fee > getBalance()) // debit amount exceeds balance
	{
		cout << "Debit amount exceeded account balance." << endl;
		return false;
	} // end if
	else // debit amount does not exceed balance
	{
		setBalance(getBalance() - amount - fee);
		return true;
	} // end else
}
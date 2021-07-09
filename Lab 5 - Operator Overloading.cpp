#include <iostream>
using namespace std;

class complex{
	public:
		friend ostream & operator<<(ostream &, const complex &);
		friend istream & operator>>(istream &, complex &);
	private:
		int r, i;
};
ostream & operator<<(ostream & cout, const complex &c){
	cout << '(' << c.r << ',' << c.i << "i)\n";
	return cout;
}
istream & operator>>(istream & cin, complex &c){
	cin.ignore();
	cin >> c.r;
	cin.ignore();
	cin >> c.i;
	return cin;
}
int main()
{
	complex x;
	cin >> x;
	cout << x;
	return 0;
}


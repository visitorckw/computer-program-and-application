#include <iostream>
using namespace std;

class complex{
	public:
		complex();
		complex(const double &, const double &);
		friend ostream & operator<<(ostream &, const complex &);
		friend istream & operator>>(istream &, complex &);
		bool operator==(complex &);
		bool operator!=(complex &);
		complex operator+(complex &);
		complex operator+(const double &);
		friend complex operator+(const double &, const complex &);
		complex operator-(complex &);
		complex operator-(const double &);
		friend complex operator-(const double &, const complex &);
		friend complex operator-(complex &);
		double operator[](const int &);
	private:
		double r, i;
};
complex::complex(){}
complex::complex(const double &x, const double &y){
	r = x;
	i = y;
}
ostream & operator<<(ostream & cout, const complex &c){
	cout << '(' << c.r << ',' << c.i << "i)";
	return cout;
}
istream & operator>>(istream & cin, complex &c){
	cin.ignore(100,'(');
	cin >> c.r;
	cin.ignore(100,',');
	cin >> c.i;
	return cin;
}
bool complex::operator==(complex &c){
	return r == c[0] && i == c[1];
}
bool complex::operator!=(complex &c){
	return r != c[0] || i != c[1];
}
complex complex::operator+(complex &c){
	return complex(r + c[0], i + c[1]);
}
complex complex::operator+(const double &d){
	return complex(r + d, i);
}
complex operator+(const double &d, const complex &c){
	return complex(d + c.r, c.i);
}
complex complex::operator-(complex &c){
	complex ans;
	return complex(r - c[0], i - c[1]);
}
complex complex::operator-(const double &d){
	return complex(r - d, i);
}
complex operator-(const double &d, const complex &c){
	return complex(d - c.r, -c.i);
}
complex operator-(complex &c){
	return complex(-c[0], -c[1]);
}
double complex::operator[](const int &n){
	if(!n)
		return r;
	if(n == 1)
		return i;
	cerr << "Error!\n";
	exit(1);
}
int main()
{
	complex x, y;
	cin >> x >> y;
	cout << "x + y = " << x + y << '\n';
	cout << "x - y = " << x - y << '\n';
	cout << "-x = " << -x << '\n';
	cout << "x == y ? " << (x == y) << '\n';
	cout << "x != y ? " << (x != y) << '\n';
	cout << "x[0] = " << x[0] << '\n';
	cout << "x[1] = " << x[1] << '\n';
	return 0;
}


#include <iostream>
#include <queue>
using namespace std;

class poly{
	public:
		poly();
		explicit poly(const string &);
		poly(const poly &);
		poly(const unsigned char &);
		poly & operator=(const poly &);
		poly operator+(const poly &);
		poly operator-(const poly &);
		poly operator*(const poly &);
		poly operator/(poly &);
		poly inv();
		friend ostream & operator<<(ostream &, const poly &);
		friend istream & operator>>(istream &, poly &);
		unsigned char value;
	private:
		//char value;
};
poly::poly(){
	value = 0;
}
poly::poly(const string &str) {
	if(str.size() > 8) {
		cerr << "too many characters for string\n";
		exit(1);
	}
	string temp(8-str.size(),'0');
	temp += str;
	value = 0;
	for(int i = 0; i < 8; i++){
		value <<= 1;
		value |= (temp[i] - '0');
	}
}
poly::poly(const poly &p){
	*this = p;
}
poly::poly(const unsigned char &ch){
	value = ch;
}
poly & poly::operator=(const poly &p){
	value = p.value;
	return *this;
}
poly poly::operator+(const poly &p){
	return poly(p.value ^ this->value);
}
poly poly::operator-(const poly &p){
	return poly(p.value ^ this->value);
}
poly poly::operator*(const poly &p){
	unsigned char ans = 0;
	unsigned char arr[8];
	arr[0] = p.value;
	for(int i = 1; i < 8; i++){
		if(arr[i-1] & 128)
			arr[i] = (((arr[i-1] & 127) << 1) ^ 27);
		else 
			arr[i] = (arr[i-1] << 1);
	}
	for(int i = 0; i < 8; i++)
		if(this->value & (1 << i))  
			ans ^= arr[i];
	return poly(ans);
}
poly poly::operator/(poly &p){
	return (*this) * p.inv();
}
poly poly::inv(){
	poly t;
	poly temp;
	for(t.value = 1; ; t.value++){
		temp = t * (*this);
		if(temp.value == 1)
			return t;
		if(t.value == 255){
			cerr << "Inverse not found!!!\n";
			exit(1);
		}
	}
}
ostream & operator<<(ostream & cout, const poly &p){
	/*string str(8,0);
	unsigned char temp = p.value;
	for(int i = 7; i >= 0; i--){
		str[i] = (temp & 1) + '0';
		temp >>= 1;
	}
	cout << str + '\n';*/
	queue<int>que;
	for(int i = 0; i < 8; i++)
		if(p.value & (128 >> i))
			que.push(7-i);
	while(que.size() != 1) {
		cout << "X^" << que.front() << " + ";
		que.pop();
	}
	if(!que.front ())
		cout << "1\n";
	else 
		cout << "X^" << que.front() << '\n';
	return cout;
}
istream & operator>>(istream & cin, poly &p){
	string str;
	cin >> str;
	p = poly(str);
	return cin;
}
int main() {
	cout << "PLease input two polynomial: \n";
	poly p, p2;
	while(cin >> p >> p2){
		cout << "p + p2 = " << p + p2 << '\n';
		cout << "p - p2 = " << p - p2 << '\n';
		cout << "p * p2 = " << p * p2 << '\n';
		cout << "p / p2 = " << p / p2 << '\n';
	}
	return 0;
}

#include <iostream>
#include <ctime>
#include <vector>
using namespace std;

vector<int>vec(13,0);
int main()
{
	srand((unsigned int) time(NULL));
	for(int i = 0; i < 3.6e4; i++) 
		vec[rand() % 6 + rand() % 6 + 2]++;
	cout << "Sum:\t";
	for(int i = 2; i <= 12; i++)
		cout << i << '\t';
	cout << "\ntimes:\t";
	for(int i = 2; i <= 12; i++)
		cout << vec[i] << '\t';
	return 0;
}


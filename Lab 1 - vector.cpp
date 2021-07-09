#include <bits/stdc++.h>
using namespace std;

int binarySearch(vector<int>&vec) //返回任意value=0的位置or第一個富庶的位置
{
	int L = 0, R = vec.size() - 1, m;
	while(L <= R)
	{
		m = L + (R - L) / 2;
		if(!vec[m])
			return m;
		if(vec[m] < 0)
			R = m - 1;
		else
			L = m + 1;
	}	
	return R;
} 
int main()
{
	vector<int>vec;
	int temp;
	while(cin >> temp)//ending by EOF (Ctrl + Z)
		vec.push_back(temp);
	sort(vec.begin(), vec.end());
	temp = binarySearch(vec);//尋找負數出現的位置 
	for(; temp >= 0; temp--)
		if(vec[temp] < 0)
			break;
	if(temp != -1)
		vec.erase(vec.begin(), vec.begin() + temp + 1);
	for(int i = 0; i < vec.size(); i++)
		cout << vec[i] << ' ';
	return 0;
}


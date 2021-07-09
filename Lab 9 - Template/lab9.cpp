// lab9.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include "array.h"
using namespace std;
int main()
{
	Array<int> intarr1(13);
	Array<int> intarr2;

	cout << "size of int arr1: " << intarr1.getSize() << endl << intarr1 << endl;
	cout << "size of int arr2: " << intarr2.getSize() << endl << intarr2;

	cout << "\nEnter 17 integers:" << endl;
	cin >> intarr1 >> intarr2;

	cout << "intarr1:" << endl << intarr1 << "\nintarr2:" << endl << intarr2 << endl<<endl;

	cout << "\nTest intarr1 != intarr2" << endl;

	if (intarr1 != intarr2)
		cout << "intarr1 != intarr2 ?!" << endl;

	cout << "\nTest intarr3(intarr1):\n";
	Array<int> intarr3(intarr1);
	cout << "intarr3的size: " << intarr3.getSize() << endl << intarr3;

	cout << "\n\nTest intarr1 = intarr2:\n\n";
	intarr1 = intarr2;
	cout << "intarr1:" << intarr1 << "\nintarr2:" << intarr2 << endl;

	cout << "\nTest intarr1 == intarr2" << endl;

	if (intarr1 == intarr2)
		cout << "intarr1 == intarr2 ?!\n" << endl;

	cout << "intarr1[0]: " << intarr1[0]<<endl;

	cout << "Test intarr1[0] = 100" << endl;
	intarr1[0] = 100;
	cout << "intarr1:\n" << intarr1<<endl;

	cout << "\nTest intarr1[99] = 100" << endl;
	intarr1[99] = 100;
}

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案

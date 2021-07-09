#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <iomanip>
using namespace std;

template<typename T>
class Array
{
	friend ostream& operator<< <T>(ostream&, const Array&);
	friend istream& operator>> <T>(istream&, Array&);

public:
	Array(int = 1);
	Array(const Array<T>&);
	~Array();
	int getSize() const;

	const Array<T>& operator=(const Array<T>&);
	bool operator==(const Array<T>&)const;
	bool operator!=(const Array<T>& right)const { return !(*this == right); }
	T& operator[](int);
	T operator[](int)const;

private:
	int size;
	T* ptr;
};

template<typename T>
Array<T>::Array(int arrsize)
{
	size = (arrsize > 0) ? arrsize : 10;
	ptr = new T[size];

	for (int i = 0; i < size; ++i)
		ptr[i] = 0;
}

template<typename T>
Array<T>::Array(const Array<T>& arr2):size(arr2.size)
{
	ptr = new T[size];

	for (int i = 0; i < size; ++i)
		ptr[i] = arr2.ptr[i];
}

template<typename T>
Array<T>::~Array()
{
	delete [] ptr;
}

template<typename T>
int Array<T>::getSize()const
{
	return size;
}


template<typename T>
const Array<T>& Array<T>::operator=(const Array<T>& right)
{
	if (&right != this)
	{
		if (size != right.size)
		{
			delete[] ptr;
			size = right.size;
			ptr = new T[size];
		}

		for (int i = 0; i < size; ++i)
			ptr[i] = right.ptr[i];
	}

	return *this;
}

template<typename T>
bool Array<T>::operator==(const Array<T>& right)const
{
	if ( size != right.size || sizeof(ptr[0])!=sizeof(right.ptr[0]) )
		return 0;

	for (int i = 0; i < size; ++i)
		if (ptr[i] != right[i])
			return 0;

	return 1;
}

template<typename T>
T& Array<T>::operator[](int index)
{
	if (index<0 || index>size)
	{
		cerr << "\nIndex¶W¥X½d³ò";
		exit(1);
	}

	return ptr[index];
}

template<typename T>
T Array<T>::operator[](int index)const
{
	if (index<0 || index>size)
	{
		cerr << "\nIndex¶W¥X½d³ò";
		exit(1);
	}

	return ptr[index];
}

template<typename T>
istream& operator>>(istream& in, Array<T>& arr)
{
	for (int i = 0; i < arr.size; ++i)
		in >> arr.ptr[i];

	return in;
}

template<typename T>
ostream& operator<<(ostream& out, const Array<T>& arr)
{
	int i=0;

	for (int i = 0; i < arr.size; ++i)
	{
		out << setw(12) << arr.ptr[i];

		if (!((i + 1) & 3))
			out << endl;
	}

	if (i & 3)
		out << endl;

	return out;
}
#endif // !ARRAY_H


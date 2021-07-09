#pragma once
#ifndef MAMMAL_H
#define MAMMAL_H
#include<iostream>
using namespace std;
class Mammal {
public:
	Mammal(void);
	~Mammal(void);
	virtual void Speak() const = 0;
protected:
	string name;
	int age;
};
Mammal::Mammal(void) : name("noname"), age(1) {
	cout << "Mammal constructor..." << endl;
}
Mammal::~Mammal(void) {
	cout << "Mammal destructor..." << endl;
}

#endif
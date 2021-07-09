#pragma once
#ifndef DOG_H
#define DOG_H
#include"Mammal.h"
class Dog :public Mammal {
public:
	void Speak()const {
		cout << "Dog" << name << "bark" << endl;
	}
};
#endif
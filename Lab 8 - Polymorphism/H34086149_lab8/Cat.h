#pragma once
#ifndef CAT_H
#define CAT_H
#include"Mammal.h"
class Cat :public Mammal {
public:
	void Speak()const {
		cout << "Cat" << name << "meow" << endl;
	}
};
#endif
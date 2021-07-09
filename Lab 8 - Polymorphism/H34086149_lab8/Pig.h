#pragma once
#ifndef PIG_H
#define PIG_H
#include"Mammal.h"
class Pig :public Mammal {
public:
	void Speak()const {
		cout << "Pig" << name << "co" << endl;
	}
};
#endif
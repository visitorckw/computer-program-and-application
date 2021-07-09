#pragma once
#ifndef HORSE_H
#define HORSE_H
#include"Mammal.h"
class Horse :public Mammal {
public:
	void Speak()const {
		cout << "Horse" << name << "neigh" << endl;
	}
};
#endif
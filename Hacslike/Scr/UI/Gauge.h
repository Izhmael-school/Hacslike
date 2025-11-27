#pragma once
#include <Dxlib.h>
#include "../Definition.h"

class Gauge{
private:
	float decreaseValue;
public:
	float maxValue;
	float currentValue;

	float posX, posY;
	float width;
	float height;
public:
	Gauge();
	~Gauge() = default;

	void Update(float _currentValue);
};


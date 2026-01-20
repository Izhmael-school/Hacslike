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
	Gauge(float _maxValue,float x,float y, float w, float h);
	~Gauge() = default;

	void Render(float _currentValue);
};


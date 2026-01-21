#pragma once
#include <Dxlib.h>
#include "../Definition.h"

class Gauge{
private:
	// 徐々に減る演出用
	float currentDecreaseValue;
public:
	// 数値
	int& maxValue;
	int& currentValue;

	// 座標等
	float posX, posY;
	float width;
	float height;

	// 色
	unsigned int topColor = green;
	unsigned int bottomColor = red;
	unsigned int frameColor = black;

	// 徐々に減るゲージの色
	unsigned int backColor = GetColor(50,175,50);
	// 徐々に減るスピード
	float speed;
public:
	Gauge(int& _currentValue, int& _maxValue, float x, float y, float w, float h);
	~Gauge() = default;

	void ChangeColor(unsigned int _top, unsigned int _bottom, unsigned int _frame,unsigned int _back);
	inline void SetSpeed(float _speed) { speed = _speed; }

	void Render();
};


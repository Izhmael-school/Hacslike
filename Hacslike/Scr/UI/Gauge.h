#pragma once
#include <Dxlib.h>
#include "../Definition.h"
#include "../Manager/TimeManager.h"

template <typename Value>

class Gauge {
private:
	// 徐々に減る演出用
	float currentDecreaseValue;
public:
	// 数値
	Value& maxValue;
	Value& currentValue;

	// 座標等
	float posX, posY;
	float width;
	float height;

	// 色
	unsigned int topColor = green;
	unsigned int bottomColor = red;
	unsigned int frameColor = black;

	// 徐々に減るゲージの色
	unsigned int backColor = GetColor(50, 175, 50);
	// 徐々に減るスピード
	float speed;
	// 徐々に減るか
	bool isDecrease;
public:
	Gauge(Value& _currentValue, Value& _maxValue, float x, float y, float w, float h, float _isDecrease = true)
		:maxValue(_maxValue)
		, currentValue(_currentValue)
		, currentDecreaseValue(-1)
		, posX(x)
		, posY(y)
		, width(w)
		, height(h)
		, speed(1)
		, isDecrease(_isDecrease) {
		currentDecreaseValue = width * ((float)currentValue / (float)maxValue);
	}

	~Gauge() = default;

	inline void ChangeColor(unsigned int _top, unsigned int _bottom, unsigned int _frame, unsigned int _back) {
		topColor = _top;
		bottomColor = _bottom;
		frameColor = _frame;
		backColor = _back;
	}

	inline void SetSpeed(float _speed) { speed = _speed; }

	void Render() {
		float value = static_cast<float>(currentValue) / static_cast<float>(maxValue);
		if (value > 1) value = 1;

		float barWidth = width * value;

		float diff = barWidth - currentDecreaseValue;

		currentDecreaseValue += diff * TimeManager::GetInstance().deltaTime * speed;

		// 微振動防止
		if (fabs(diff) < 0.1f) {
			currentDecreaseValue = barWidth;
		}

		// 背景
		DrawBox(posX, posY, posX + width, posY + height, bottomColor, true);
		// 徐々に減るか
		if (isDecrease) {
			// 前よりゲージが減っているか
			if (barWidth < currentDecreaseValue) {
				// 徐々に減る
				DrawBox(posX, posY, posX + currentDecreaseValue, posY + height, backColor, true);
				DrawBox(posX, posY, posX + barWidth, posY + height, topColor, true);
			}
			else {
				DrawBox(posX, posY, posX + barWidth, posY + height, backColor, true);
				// 徐々に増える
				DrawBox(posX, posY, posX + currentDecreaseValue, posY + height, topColor, true);
			}
		}
		else {
			DrawBox(posX, posY, posX + barWidth, posY + height, topColor, true);
		}

		// 枠
		DrawBox(posX, posY, posX + width, posY + height, frameColor, false);
	}
};


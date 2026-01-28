#pragma once
#include <DxLib.h>
#include "../Definition.h"
#include "../Manager/TimeManager.h"

template <typename Value>
class CircleGauge {
private:
	float currentDecreaseAngle;

public:
	Value& maxValue;
	Value& currentValue;

	// íÜêSç¿ïW
	float centerX, centerY;

	// îºåa
	float outerRadius;
	float innerRadius;

	// äpìx
	float startDeg;
	float endDeg;

	// êF
	unsigned int topColor = green;
	unsigned int bottomColor = red;
	unsigned int frameColor = black;
	unsigned int backColor = GetColor(50, 175, 50);

	// å∏è≠ââèo
	float speed;
	bool isDecrease;

	// êj
	bool isNeedle;
	float needleLength;
	unsigned int needleColor;
	int needleThickness;

	CircleGauge(
		Value& _current,
		Value& _max,
		float x,
		float y,
		float outerR,
		float thickness,
		float _startDeg = -90.0f,
		float _endDeg = 270.0f,
		bool _isDecrease = true
	)
		: currentValue(_current)
		, maxValue(_max)
		, centerX(x)
		, centerY(y)
		, outerRadius(outerR)
		, innerRadius(outerR - thickness)
		, startDeg(_startDeg)
		, endDeg(_endDeg)
		, speed(5.0f)
		, isDecrease(_isDecrease)
		, isNeedle(false)
		, needleLength(outerRadius)
		, needleColor(white)
		, needleThickness(2) {
		float rate = (float)currentValue / (float)maxValue;
		currentDecreaseAngle = startDeg + (endDeg - startDeg) * rate;
	}

	inline void SetSpeed(float s) { speed = s; }

	inline void ChangeColor(
		unsigned int top,
		unsigned int bottom,
		unsigned int frame,
		unsigned int back
	) {
		topColor = top;
		bottomColor = bottom;
		frameColor = frame;
		backColor = back;
	}

	void DrawRing(float drawEndDeg, unsigned int color, int div = 128) {
		for (int i = 0; i < div; i++) {
			float t0 = (float)i / div;
			float t1 = (float)(i + 1) / div;

			float deg0 = startDeg + (drawEndDeg - startDeg) * t0;
			float deg1 = startDeg + (drawEndDeg - startDeg) * t1;

			float r0 = Deg2Rad(deg0);
			float r1 = Deg2Rad(deg1);

			VECTOR v0 = VGet(centerX + cosf(r0) * outerRadius, centerY + sinf(r0) * outerRadius, 0);
			VECTOR v1 = VGet(centerX + cosf(r1) * outerRadius, centerY + sinf(r1) * outerRadius, 0);
			VECTOR v2 = VGet(centerX + cosf(r1) * innerRadius, centerY + sinf(r1) * innerRadius, 0);
			VECTOR v3 = VGet(centerX + cosf(r0) * innerRadius, centerY + sinf(r0) * innerRadius, 0);

			DrawTriangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, color, TRUE);
			DrawTriangle(v2.x, v2.y, v3.x, v3.y, v0.x, v0.y, color, TRUE);
		}
	}

	inline void EnableNeedle(bool enable) {
		isNeedle = enable;
	}

	inline void SetNeedle(float length, unsigned int color, int thickness = 2) {
		needleLength = length;
		needleColor = color;
		needleThickness = thickness;
	}

	void DrawNeedle(float angleDeg) {
		float rad = Deg2Rad(angleDeg);

		float x = centerX + cosf(rad) * needleLength;
		float y = centerY + sinf(rad) * needleLength;

		DrawLineAA(centerX, centerY, x, y, needleColor, needleThickness);
	}


	void Render() {
		float rate = (float)currentValue / (float)maxValue;
		float targetAngle = startDeg + (endDeg - startDeg) * rate;

		float diff = targetAngle - currentDecreaseAngle;
		currentDecreaseAngle += diff * TimeManager::GetInstance().deltaTime * speed;

		if (fabs(diff) < 0.1f)
			currentDecreaseAngle = targetAngle;

		// îwåi
		DrawRing(endDeg, bottomColor);

		if (isDecrease) {
			if (targetAngle < currentDecreaseAngle) {
				DrawRing(currentDecreaseAngle, backColor);
				DrawRing(targetAngle, topColor);
			}
			else {
				DrawRing(targetAngle, backColor);
				DrawRing(currentDecreaseAngle, topColor);
			}
		}
		else {
			DrawRing(targetAngle, topColor);
		}

		// êjÅiç≈å„Ç…ï`âÊÅj
		if (isNeedle) {
			DrawNeedle(currentDecreaseAngle);
		}
	}
};
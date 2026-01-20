#include "Gauge.h"
#include "../Manager/TimeManager.h"

Gauge::Gauge(float _maxValue, float x, float y, float w, float h)
	:maxValue(_maxValue)
	, currentValue(maxValue)
	, decreaseValue(maxValue)
	, posX(x)
	, posY(y)
	, width(w)
	, height(h) {}

void Gauge::Render(float _currentValue) {
	float value = _currentValue / maxValue;
	float barWidth = width * value;
	decreaseValue += (currentValue - decreaseValue) * TimeManager::GetInstance().deltaTime * 1;
	// îwåi
	DrawBox(posX, posY, posX + width, posY + height, red, true);

	// HP
	DrawBox(posX, posY, posX + barWidth, posY + height, green, true);

	// òg
	DrawBox(posX, posY, posX + width, posY + height, black, false);
}


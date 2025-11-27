#include "Gauge.h"
#include "../Manager/TimeManager.h"

Gauge::Gauge() {}

void Gauge::Update(float _currentValue) {
	float value = _currentValue / maxValue;
	float d = (decreaseValue - currentValue) * TimeManager::GetInstance().deltaTime;
	DrawBox(posX, posY, posX + width, posY + height, red, true);
	DrawBox(posX, posY, posX + width - value, posY + height, green, true);
	DrawBox(posX, posY, posX + width, posY + height, black, false);
}


#include "Gauge.h"
#include "../Manager/TimeManager.h"

Gauge::Gauge(int& _currentValue, int& _maxValue, float x, float y, float w, float h)
	:maxValue(_maxValue)
	, currentValue(_currentValue)
	, currentDecreaseValue(-1)
	, posX(x)
	, posY(y)
	, width(w)
	, height(h)
	, speed(1) {
	currentDecreaseValue = width * ((float)currentValue / (float)maxValue);
}

void Gauge::ChangeColor(unsigned int _top, unsigned int _bottom, unsigned int _frame, unsigned int _back) {
	topColor = _top;
	bottomColor = _bottom;
	frameColor = _frame;
	backColor = _back;
}

void Gauge::Render() {
	float value = (float)currentValue / (float)maxValue;
	float barWidth = width * value;

	float diff = barWidth - currentDecreaseValue;

	currentDecreaseValue += diff * TimeManager::GetInstance().deltaTime * speed;

	// î˜êUìÆñhé~
	if (fabs(diff) < 0.1f) {
		currentDecreaseValue = barWidth;
	}

	// îwåi
	DrawBox(posX, posY, posX + width, posY + height, bottomColor, true);
	// ëOÇÊÇËÉQÅ[ÉWÇ™å∏Ç¡ÇƒÇ¢ÇÈÇ©
	if (barWidth < currentDecreaseValue) {
		// èôÅXÇ…å∏ÇÈ
		DrawBox(posX, posY, posX + currentDecreaseValue, posY + height, backColor, true);
		// HP
		DrawBox(posX, posY, posX + barWidth, posY + height, topColor, true);
	}
	else {
		// HP
		DrawBox(posX, posY, posX + barWidth, posY + height, backColor, true);
		// èôÅXÇ…ëùÇ¶ÇÈ
		DrawBox(posX, posY, posX + currentDecreaseValue, posY + height, topColor, true);
	}

	// òg
	DrawBox(posX, posY, posX + width, posY + height, frameColor, false);
}


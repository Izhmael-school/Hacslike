#include "InputManager.h"
#include "../Definition.h"

InputManager::InputManager()
	: keyState("")
	, prevkeyState("")
	, padState()
	, prevPadState()
	, mouseInput(-1)
	, prevMouseInput(-1)
{
}

/*
 * @function	Update
 * @brief		更新処理
 */
void InputManager::Update() {
	// キーボード
	memcpy_s(prevkeyState, 256, keyState, 256);
	GetHitKeyStateAll(keyState);

	// XInputコントローラー
	prevPadState = padState;
	GetJoypadXInputState(DX_INPUT_PAD1, &padState);

	// マウス
	prevMouseInput = mouseInput;
	mouseInput = GetMouseInput();
}

#pragma once
#include <windows.h>
#define DX_NON_USING_NAMESPACE_DXLIB
#define DX_NON_NAMESPACE
#include <XInput.h>
#include <Dxlib.h>
#include "../Component/Singleton.h"

class InputManager : public Singleton<InputManager> {
private:
	char keyState[256];
	char prevkeyState[256];

	::XINPUT_STATE padState;
	::XINPUT_STATE prevPadState;

	int prevMouseInput;
	int mouseInput;
public:	// メンバ変数	

	InputManager();
	~InputManager() = default;
	// 更新処理
	void Update();

public:	// キーボード入力用
	// キーが押されたか
	inline bool IsKeyDown(int _key) const { return !prevkeyState[_key] && keyState[_key]; }

	// キーが押されているか
	inline bool IsKey(int _key) const { return keyState[_key]; }

	// キーが離されたか
	inline bool IsKeyUp(int _key) const {return prevkeyState[_key] && !keyState[_key];
	}

#pragma region パッド用
	// キーが押されたか
	inline bool IsButtonDown(int _key) { return !(prevPadState.Gamepad.wButtons & _key) && (padState.Gamepad.wButtons & _key); }

	// キーが押されているか
	inline bool IsButton(int _key) { return padState.Gamepad.wButtons & _key; }

	// キーが離されたか
	inline bool IsButtonUp(int _key) { return (prevPadState.Gamepad.wButtons & _key) && !(padState.Gamepad.wButtons & _key); }
#pragma endregion

#pragma region マウス用
	// クリックされたか
	inline bool IsMouseDown(int _mouse) const { return !(prevMouseInput & _mouse) && (mouseInput & _mouse); }

	// クリックされているか
	inline bool IsMouse(int _mouse) const { return (mouseInput & _mouse); }

	// クリックが離されたか
	inline bool IsMouseUp(int _mouse) const { return (prevMouseInput & _mouse) && !(mouseInput & _mouse); }
#pragma endregion

};


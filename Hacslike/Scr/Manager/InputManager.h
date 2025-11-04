#pragma once
#include <Windows.h>
#include <XInput.h>
#include <Dxlib.h>
#include <cmath>
#include <string>
#include "../Component/Singleton.h"

#pragma comment(lib, "xinput.lib")

class InputManager : public Singleton<InputManager> {
public:	// コンストラクタとデストラクタ
	/*
	 * @brief	コンストラクタ
	 * @tip		外部で生成されないようにアクセス指定子をprivateにする
	 */
	InputManager();

	/*
	 * @brief	デストラクタ
	 */
	~InputManager() = default;

private:
	char keyState[256];
	char prevkeyState[256];

	XINPUT_STATE padState;		  //現在
	XINPUT_STATE prevPadState;    //1フレーム前のキーの状態
	XINPUT_STATE sthickState;     //スティックの状態
	float rangeOfMotion;          //スティック可動範囲

	int prevMouseInput;
	int mouseInput;

	bool isPadActive;
public:	// メンバ変数
	// 更新処理
	void Update();

public:	// キーボード入力用
	// キーが押されたか
	inline bool IsKeyDown(int _key) const { return !prevkeyState[_key] && keyState[_key]; }

	// キーが押されているか
	inline bool IsKey(int _key) const { return keyState[_key]; }

	// キーが離されたか
	inline bool IsKeyUp(int _key) const { return prevkeyState[_key] && !keyState[_key]; }

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

public: //左右のスティック入力用
	//左
	inline float IsJoypadSthick(std::string _SthickName) {
		//入力値
		float tmp = 0.0f;
		if (_SthickName == "L_Vertical") {
			tmp += padState.Gamepad.sThumbLY / rangeOfMotion;
			if (padState.Gamepad.sThumbLY > rangeOfMotion) {
				padState.Gamepad.sThumbLY = rangeOfMotion;
			}
		}
		else if (_SthickName == "L_Horizontal") {
			if (padState.Gamepad.sThumbLX > rangeOfMotion) {
				padState.Gamepad.sThumbLX = rangeOfMotion;
			}
			tmp += padState.Gamepad.sThumbLX / rangeOfMotion;

		}
		else if (_SthickName == "R_Vertical") {
			if (padState.Gamepad.sThumbRY > rangeOfMotion) {
				padState.Gamepad.sThumbRY = rangeOfMotion;
			}
			tmp += padState.Gamepad.sThumbRY / rangeOfMotion;

		}
		else if (_SthickName == "R_Horizontal") {
			if (padState.Gamepad.sThumbRX > rangeOfMotion) {
				padState.Gamepad.sThumbRX = rangeOfMotion;
			}
			tmp += padState.Gamepad.sThumbRX / rangeOfMotion;

		}

		if (abs(tmp) <= 0.05f)
			tmp = 0.0f;

		return tmp;
	}

};


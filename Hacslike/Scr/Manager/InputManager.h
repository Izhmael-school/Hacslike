#pragma once
#include <Dxlib.h>
#include <cmath>
#include <string>


class InputManager {
#pragma region シングルトンのデータ構造
private:	// 静的メンバ変数
	static InputManager* pInstance;	// 自身のインスタンスのアドレスを格納


private:	// コンストラクタとデストラクタ
	/*
	 * @brief	コンストラクタ
	 * @tip		外部で生成されないようにアクセス指定子をprivateにする
	 */
	InputManager();

	/*
	 * @brief	デストラクタ
	 */
	~InputManager() = default;

public:	//コピーと譲渡禁止
	InputManager(const InputManager&) = delete;
	InputManager& operator = (const InputManager&) = delete;
	InputManager(InputManager&&) = delete;
	InputManager& operator = (InputManager&&) = delete;

private:	// 静的メンバ関数
	/*
	 * @function	CreateInstance
	 * @brief		自信のインスタンスを生成する
	 */
	static void CreateInstance();

public:	// 静的メンバ関数
	/*
	 * @function	GetInstance
	 * @brief		自信のインスタンスを取得する唯一の手段
	 * @return		InputManager*	自身のインスタンスのアドレス
	 */
	static InputManager* GetInstance();

	/*
	 * @function	DestroyInstance
	 * @brief		自信のインスタンスを破棄する唯一の手段
	 */
	static void DestroyInstance();
#pragma endregion

private:
	char keyState[256];
	char prevkeyState[256];

	XINPUT_STATE padState;		  //現在
	XINPUT_STATE padPrevState;    //1フレーム前のキーの状態
	XINPUT_STATE sthickState;     //スティックの状態
	float rangeOfMotion;          //スティック可動範囲

	int prevMouse;
	int mouse = GetMouseInput();

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
	inline bool IsKeyUp(int _key) const {return prevkeyState[_key] && !keyState[_key];
	}

#pragma region パッド用
	// キーが押されたか
	inline bool IsButtonDown(int _key) { return !padPrevState.Buttons[_key] && padState.Buttons[_key]; }

	// キーが押されているか
	inline bool IsButton(int _key) { return padState.Buttons[_key]; }

	// キーが離されたか
	inline bool IsButtonUp(int _key) { return padPrevState.Buttons[_key] && !padState.Buttons[_key]; }
#pragma endregion

#pragma region マウス用
	// クリックされたか
	inline bool IsMouseDown(int _mouse) const { return !(prevMouse & _mouse) && (mouse & _mouse); }

	// クリックされているか
	inline bool IsMouse(int _mouse) const { return (mouse & _mouse); }

	// クリックが離されたか
	inline bool IsMouseUp(int _mouse) const { return (prevMouse & _mouse) && !(mouse & _mouse); }
#pragma endregion

public: //左右のスティック入力用
	//左
	inline float IsJoypadSthick(std::string _SthickName) {
		//入力値
		float tmp = 0.0f;
		if (_SthickName == "L_Vertical") {
			tmp += sthickState.ThumbLY / rangeOfMotion;
			if (sthickState.ThumbLY > rangeOfMotion) {
				sthickState.ThumbLY = rangeOfMotion;
			}
		}
		else if (_SthickName == "L_Horizontal") {
			if (sthickState.ThumbLX > rangeOfMotion) {
				sthickState.ThumbLX = rangeOfMotion;
			}
			tmp += sthickState.ThumbLX / rangeOfMotion;

		}
		else if (_SthickName == "R_Vertical") {
			if (sthickState.ThumbRY > rangeOfMotion) {
				sthickState.ThumbRY = rangeOfMotion;
			}
			tmp += sthickState.ThumbRY / rangeOfMotion;

		}
		else if (_SthickName == "R_Horizontal") {
			if (sthickState.ThumbRX > rangeOfMotion) {
				sthickState.ThumbRX = rangeOfMotion;
			}
			tmp += sthickState.ThumbRX / rangeOfMotion;

		}

		if (abs(tmp) <= 0.05f)
			tmp = 0.0f;

		return tmp;
	}

};


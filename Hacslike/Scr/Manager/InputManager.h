#pragma once
#include <Dxlib.h>

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

	XINPUT_STATE padState;
	XINPUT_STATE padPrevState;

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

};


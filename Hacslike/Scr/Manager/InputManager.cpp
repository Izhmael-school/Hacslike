#include "InputManager.h"
#include <DxLib.h>

// 静的メンバ変数の初期化
InputManager* InputManager::pInstance = nullptr;

InputManager::InputManager()
	:keyState()
	,prevkeyState() 
{}

/*
 * @function	CreateInstance
 * @brief		自信のインスタンスを生成する
 */
void InputManager::CreateInstance() {
	pInstance = new InputManager();
}

/*
 * @function	GetInstance
 * @brief		自信のインスタンスを取得する唯一の手段
 * @return		InputManager*	自身のインスタンスのアドレス
 *  @tip		生成は１度だけ行う
 */
InputManager* InputManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

/*
 * @function	DestroyInstance
 * @brief		自信のインスタンスを破棄する唯一の手段
 */
void InputManager::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

/*
 * @function	Update
 * @brief		更新処理
 */
void InputManager::Update() {
	// １フレーム前のキーの状態を保存
	memcpy_s(prevkeyState, 256, keyState, 256);
	// 今のフレームの状態を保存
	GetHitKeyStateAll(keyState);

	// １フレーム前のキーの状態を保存
	padPrevState = padState;
	// 今のフレームの状態を保存
	GetJoypadXInputState(DX_INPUT_PAD1, &padState);

	prevMouse = mouse;
	mouse = GetMouseInput();

}

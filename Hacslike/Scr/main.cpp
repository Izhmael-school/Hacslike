#include <iostream>
#include <Dxlib.h>
#include <random>
#include "Definition.h"
#include "Manager/TimeManager.h"
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "Manager/StageManager.h"
#include "Manager/CollisionManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

#pragma region // DxLibの初期化処理　触るべからず
	// ウィンドウのサイズを変更する
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, FPS);
	// 起動時のウィンドウのモードの設定
	ChangeWindowMode(TRUE);	// TRUE : ウィンドウモード FALSE : フルスクリーン
	// 背景色の設定
	SetBackgroundColor(196, 196, 196);

	// Dxlibの初期化
	if (DxLib_Init() == -1)
		return 0;

	// Effekseerの初期化
	/*if (Effekseer_Init(8000) == -1) {
		DxLib_End();
		return 0;
	}*/

	// 描画する先を設定する 裏画面に変更する
	SetDrawScreen(DX_SCREEN_BACK);

	// 図形描画のZバッファの有効化
	{
		// Zバッファを使用するかどうか
		SetUseZBuffer3D(TRUE);	// default : FALSE
		// Zバッファに書き込みを行うか
		SetWriteZBuffer3D(TRUE); // default : FALSE
	}

	// ライティング
	{
		// ライトの計算をどうするか
		SetUseLighting(TRUE); // default : TRUE
		// 標準ライトを使用するかどうか
		SetLightEnable(TRUE);	// default : TRUE
		// グローバル環境光の設定
		SetGlobalAmbientLight(GetColorF(1, 0, 0, 0));
		//// 反射光の設定  Diffuse
		//SetLightDifColor(GetColorF(1, 0, 0, 0));
		//// 鏡面反射光の設定　Specular
		//SetLightSpcColor(GetColorF(1, 0, 0.25f, 1));
		//// 環境光の設定　Ambient
		//SetLightAmbColor(GetColorF(1, 1, 1, 1));
	}
#pragma endregion

	// 乱数調節(ガチ)
	std::random_device rd;
	std::mt19937_64 mt(rd());
	SRand(mt());
	SRand(mt());

	

	// ゲームのメインループ
	while (true) {





		// 更新処理
		SceneManager::GetInstance()->Update();
		TimeManager::GetInstance()->Update();
		InputManager::GetInstance()->Update();
		StageManager::GetInstance()->Update();
		CollisionManager::GetInstance()->Update();

		// 画面をクリアする
		ClearDrawScreen();

		// 描画処理
		SceneManager::GetInstance()->Render();
		StageManager::GetInstance()->Render();
		CollisionManager::GetInstance()->Render();

		// エスケープキーでウィンドウを閉じる
		if (InputManager::GetInstance()->IsKeyDown(KEY_INPUT_ESCAPE))
			break;

		// 裏画面と表画面を切り替える
		ScreenFlip();

		// 処理が速すぎたら待つ
		while (1) {
			if (GetNowCount() - TimeManager::GetInstance()->GetCurrent() >= 1000 / FPS)
				break;
		}


	}

	SceneManager::DestroyInstance();
	TimeManager::DestroyInstance();
	InputManager::DestroyInstance();
	StageManager::DestroyInstance();

	// DxLibの終了
	DxLib_End();

	return 0;
}
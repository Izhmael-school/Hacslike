#include "FadeManager.h"
#include "TimeManager.h"

#pragma region シングルトンのデータ構造

//	静的メンバ変数の初期化
FadeManager* FadeManager::pInstance = nullptr;

/*
*	@brief	コンストラクタ
*/
FadeManager::FadeManager()
	: blend(0.0f)
	, time(0.0f)
	, fadeState(FadeState::FadeEnd) {}

/*
*	@brief	デストラクタ
*/
//FadeManager::~FadeManager() {}

/*
*	@function	CreateInstance
*	@brief		自身のインスタンスを生成する
*/
void FadeManager::CreateInstance() {
	pInstance = new FadeManager();
}

/*
*	@function	GetInstance
*	@brief		自身のインスタンスを取得する唯一の手段
*	@return		InputManager*	自身のインスタンスのアドレス
*/
FadeManager* FadeManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();
	return pInstance;
}

/*
*	@function	DestroyInstance
*	@breif		自身のインスタンスを破棄する唯一の手段
*/
void FadeManager::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}
#pragma endregion

/*
*	@function	Update
*	@breif		更新処理
*/
void FadeManager::Update() {
	// フェード処理が完了していたら更新しない
	if (fadeState == FadeState::FadeEnd) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		return;
	}

	//	だんだんとフェード処理を行うための計算
	blend += 255 * (int)fadeState * TimeManager::GetInstance()->deltaTime / time;

	if (blend <= 0) {
		blend = 0;
		fadeState = FadeState::FadeEnd;
	}
	if (blend >= 255) {
		blend = 255;
		fadeState = FadeState::FadeEnd;
	}
}

/*
*	@function	Render
*	@breif		描画処理
*/
void FadeManager::Render() {
	//	フェード処理が完了していたら描画しない
	if (fadeState == FadeState::FadeEnd)
		return;

	//	透明度を変化させてフェード処理とする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)blend);
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, black, TRUE);
	//	ブレンドの状態を戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

/*
*	@function		FadeIn
*	@brief			フェードイン
*	@param[in]	float _t = 1.0f		全体時間
*/
void FadeManager::FadeIn(float _t) {
	//	各種の変数を初期化
	fadeState = FadeState::FadeIn;
	time = _t;
	blend = 255.0f;
}

/*
*	@function		FadeOut
*	@brief			フェードアウト
*	@param[in]	float _t = 1.0f		全体時間
*/
void FadeManager::FadeOut(float _t) {
	//	各種の変数を初期化
	fadeState = FadeState::FadeOut;
	time = _t;
	blend = 0.0f;
}
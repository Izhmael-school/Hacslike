#pragma once
#include "../../Definition.h"

class FadeManager {
#pragma region シングルトンのデータ構造
private:	// 静的メンバ変数
	static FadeManager* pInstance;	// 自身のインスタンスのアドレスを格納


private:	// コンストラクタとデストラクタ
	/*
	 * @brief	コンストラクタ
	 * @tip		外部で生成されないようにアクセス指定子をprivateにする
	 */
	FadeManager();

	/*
	 * @brief	デストラクタ
	 */
	~FadeManager() = default;

public:	//コピーと譲渡禁止
	FadeManager(const FadeManager&) = delete;
	FadeManager& operator = (const FadeManager&) = delete;
	FadeManager(FadeManager&&) = delete;
	FadeManager& operator = (FadeManager&&) = delete;

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
	static FadeManager* GetInstance();

	/*
	 * @function	DestroyInstance
	 * @brief		自信のインスタンスを破棄する唯一の手段
	 */
	static void DestroyInstance();
#pragma endregion

private:
	float blend;
	float time;

	FadeState fadeState;
public:
	void Update();

	void Render();

	void FadeIn(float _t = 1.0f);

	void FadeOut(float _t = 1.0f);
public:
	inline FadeState GetFadeState() const { return fadeState; }
};


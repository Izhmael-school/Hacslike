#pragma once
#include "../Definition.h"
class SceneManager {
#pragma region シングルトンのデータ構造
private:	// 静的メンバ変数
	static SceneManager* pInstance;	// 自身のインスタンスのアドレスを格納


private:	// コンストラクタとデストラクタ
	/*
	 * @brief	コンストラクタ
	 * @tip		外部で生成されないようにアクセス指定子をprivateにする
	 */
	SceneManager();

	/*
	 * @brief	デストラクタ
	 */
	~SceneManager() = default;

public:	//コピーと譲渡禁止
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator = (const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator = (SceneManager&&) = delete;

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
	static SceneManager* GetInstance();

	/*
	 * @function	DestroyInstance
	 * @brief		自信のインスタンスを破棄する唯一の手段
	 */
	static void DestroyInstance();
#pragma endregion

private:
	class BaseScene* pCurrentScene;	// 今のシーン
	SceneType Current;
	SceneType next;
	bool changed;

public:

	void Update();

	void Render();

	void LoadScene();

public:
	void SetNext(SceneType _next);

};


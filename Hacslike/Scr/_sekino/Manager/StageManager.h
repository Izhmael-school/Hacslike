#pragma once
#include "../GameObject/Stage/StageCell.h"
#include "../GameObject/Stage/StageGenerator.h"
#include "../../Definition.h"

class StageManager {
#pragma region シングルトンのデータ構造
private:	// 静的メンバ変数
	static StageManager* pInstance;	// 自身のインスタンスのアドレスを格納

private:	// コンストラクタとデストラクタ
	/*
	 * @brief	コンストラクタ
	 * @tip		外部で生成されないようにアクセス指定子をprivateにする
	 */
	StageManager();

	/*
	 * @brief	デストラクタ
	 */
	~StageManager();

public:	//コピーと譲渡禁止
	StageManager(const StageManager&) = delete;
	StageManager& operator = (const StageManager&) = delete;
	StageManager(StageManager&&) = delete;
	StageManager& operator = (StageManager&&) = delete;

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
	static StageManager* GetInstance();

	/*
	 * @function	DestroyInstance
	 * @brief		自信のインスタンスを破棄する唯一の手段
	 */
	static void DestroyInstance();
#pragma endregion

public:
	class StageGenerator* generator;

	void Update();
	void Render();

	int GetMapData(int x,int y);
};


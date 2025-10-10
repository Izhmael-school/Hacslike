#pragma once
#include <vector>

/*
 * @brief 当たり判定の管理
 * @tips  シングルトン
 */
class CollisionManager {
#pragma region シングルトンのデータ構造
private:	// 静的メンバ変数
	static CollisionManager* pInstance;	// 自身のインスタンスのアドレスを格納


private:	// コンストラクタとデストラクタ
	/*
	 * @brief	コンストラクタ
	 * @tip		外部で生成されないようにアクセス指定子をprivateにする
	 */
	CollisionManager();

	/*
	 * @brief	デストラクタ
	 */
	~CollisionManager();

public:	//コピーと譲渡禁止
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator = (const CollisionManager&) = delete;
	CollisionManager(CollisionManager&&) = delete;
	CollisionManager& operator = (CollisionManager&&) = delete;

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
	static CollisionManager* GetInstance();

	/*
	 * @function	DestroyInstance
	 * @brief		自信のインスタンスを破棄する唯一の手段
	 */
	static void DestroyInstance();
#pragma endregion

private:	// メンバ変数
	std::vector<class Collider*> pColliderArray;
	std::vector<std::vector<bool>> prevs;
	std::vector<std::vector<bool>> currents;

	int index = 0;
public:		// メンバ関数

	void Update();

	void Render();

	void Register(class Collider* _pCol);

	void UnRegister(class Collider* _pCol);



	/*
	 * @function	CheckHit
	 * @brief		当たり判定の衝突検知
	 * @param[in]	Collider* _pCol1	当たり判定1
	 * @param[in]	Collider* _pCol2	当たり判定2
	 * @return		bool
	 */
	bool CheckHit(class Collider* _pCol1, class Collider* _Col2);
};


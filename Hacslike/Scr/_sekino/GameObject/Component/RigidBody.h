#pragma once

class RigidBody {
#pragma region シングルトンのデータ構造
private:	// 静的メンバ変数
	static RigidBody* pInstance;	// 自身のインスタンスのアドレスを格納


private:	// コンストラクタとデストラクタ
	/*
	 * @brief	コンストラクタ
	 * @tip		外部で生成されないようにアクセス指定子をprivateにする
	 */
	RigidBody();

	/*
	 * @brief	デストラクタ
	 */
	~RigidBody();

public:	//コピーと譲渡禁止
	RigidBody(const RigidBody&) = delete;
	RigidBody& operator = (const RigidBody&) = delete;
	RigidBody(RigidBody&&) = delete;
	RigidBody& operator = (RigidBody&&) = delete;

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
	static RigidBody* GetInstance();

	/*
	 * @function	DestroyInstance
	 * @brief		自信のインスタンスを破棄する唯一の手段
	 */
	static void DestroyInstance();
#pragma endregion
private:


public:
	void Update(class GameObject* pObj);
};


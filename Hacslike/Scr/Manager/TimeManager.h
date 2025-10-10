#pragma once
class TimeManager {
#pragma region シングルトンのデータ構造
private:	// 静的メンバ変数
	static TimeManager* pInstance;	// 自身のインスタンスのアドレスを格納


private:	// コンストラクタとデストラクタ
	/*
	 * @brief	コンストラクタ
	 * @tip		外部で生成されないようにアクセス指定子をprivateにする
	 */
	TimeManager();

	/*
	 * @brief	デストラクタ
	 */
	~TimeManager() = default;

public:	//コピーと譲渡禁止
	TimeManager(const TimeManager&) = delete;
	TimeManager& operator = (const TimeManager&) = delete;
	TimeManager(TimeManager&&) = delete;
	TimeManager& operator = (TimeManager&&) = delete;

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
	static TimeManager* GetInstance();

	/*
	 * @function	DestroyInstance
	 * @brief		自信のインスタンスを破棄する唯一の手段
	 */
	static void DestroyInstance();
#pragma endregion

private:
	int prev;
	int current;
	int m = 0, s = 0, ms = 0;

public:
	float deltaTime;

public:
	void Start();
	void Update();
	void Render();

	inline int GetCurrent() const { return current; }
};


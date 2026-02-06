#pragma once
#include "../GameObject.h"

class Camera : public GameObject {
private:
	GameObject* pTarget;	// 追従する対象とのポインタ
	float armLenght;		// 追従する対象との距離
	VECTOR offSet;		 //位置調整
	int rotation_X;

public:
	static Camera* main;

public:
	// コンストラクタとデストラクタ
	/*
	 * @brief		コンストラクタ
	 * @param[in]	float _length = 500.0f	追従する対象との距離
	 */
	Camera(VECTOR _pos = VZero, float _length = 500.0f);

	/*
	 * @brief		デストラクタ
	 */
	~Camera();

public:// オーバーライドしたメンバ関数
	/*
	 * @function	Start
	 * @brief		初期化処理
	 */
	virtual void Start() override;

	/*
	 * @function	Update
	 * @brief		更新処理
	 */
	virtual void Update() override;

	/*
	 * @function	Render
	 * @brief		描画処理
	 */
	virtual void Render() override;

public:	// メンバ関数

public:	// ゲッターとセッター
	/*
	 * @function	GetTarget
	 * @brief		追従する対象の取得
	 * @return		GameObject
	 */
	inline GameObject* GetTarget() const { return pTarget; }

	/*
	 * @function	SetTarget
	 * @brief		追従する対象の変更
	 * @return		GameObject* _v 初期化する追従する対象
	 */
	inline void SetTarget(GameObject* _v) { pTarget = _v; }
};



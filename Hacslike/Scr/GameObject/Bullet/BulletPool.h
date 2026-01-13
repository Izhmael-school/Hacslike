#pragma once
#include <vector>
#include "../../Component/Collider/SphereHitBox.h"

/// <summary>
/// SphereColliderのプール
/// </summary>
class BulletPool {
public:
	static BulletPool& GetInstance() {
		static BulletPool instance(30); // 初期プール数
		return instance;
	}

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="owner"></param>
	/// <param name="pos"></param>
	/// <param name="vel"></param>
	/// <param name="radius"></param>
	/// <param name="life"></param>
	/// <returns></returns>
	SphereHitBox* Spawn(Character* owner, const VECTOR& pos, const VECTOR& vel, float radius, float life);

	/// <summary>
	///	弾用の生成
	/// </summary>
	/// <param name="owner"></param>
	/// <param name="pos"></param>
	/// <param name="vel"></param>
	/// <param name="radius"></param>
	/// <param name="life"></param>
	/// <param name="chain"></param>
	/// <returns></returns>
	SphereHitBox* BulletSpawn(Character* owner, const VECTOR& pos, const VECTOR& vel, float radius, float life, int chain);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render();

private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="poolSize"></param>
	BulletPool(int poolSize);

	/// <summary>
	///	デストラクタ
	/// </summary>
	~BulletPool();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	BulletPool(const BulletPool&) = delete;

	BulletPool& operator=(const BulletPool&) = delete;

	//	オブジェクトプール
	std::vector<SphereHitBox*> pool;
};
#include "BulletPool.h"
#include "../Hacslike/Scr/Manager/CollisionManager.h" // CollisionManager を使うので include

BulletPool::BulletPool(int poolSize) {
	pool.reserve(poolSize);
	for (int i = 0; i < poolSize; i++) {
		SphereHitBox* b = new SphereHitBox();
		b->SetActive(false);
		pool.push_back(b);
		CollisionManager::GetInstance().UnRegister(b->GetCollider());
	}
}

BulletPool::~BulletPool() {
	// プール破棄時に登録解除しておく（安全策）
	for (auto b : pool) {
		if (b) {
			auto col = b->GetCollider();
			if (col) {
				// CollisionManager に登録されているか分からないので常に UnRegister を試みる（安全な実装なら無害）
				CollisionManager::GetInstance().UnRegister(col);
			}
			delete b;
		}
	}
}

static void CleanupColliderForBullet(SphereHitBox* b) {
	if (!b) return;
	auto col = b->GetCollider();
	if (col) {
		col->SetEnable(false);
		// CollisionManager 側が未登録の場合は無視する実装であることが望ましい
		CollisionManager::GetInstance().UnRegister(col);
		// もし SphereHitBox 側で登録フラグを持っていればここでクリアする
		// b->SetIsRegistered(false);
	}
}

SphereHitBox* BulletPool::Spawn(Character* owner, const VECTOR& pos, const VECTOR& vel, float radius, float life) {
	for (auto b : pool) {
		if (b->GetActive()) continue;
			// --- 再利用前に古いコライダーの残滓がないか安全にクリーンアップ ---
			CleanupColliderForBullet(b);

			// Reset で内部状態を初期化（位置・速度・有効化フラグは Reset でやる設計）
			b->Reset(owner, pos, vel, radius, life);

			// Spawn 後は必ず Collider を登録して有効化する（Reset が登録しない設計の場合）
			auto newCol = b->GetCollider();
			if (newCol) {
				// Register（CollisionManager が二重登録を防ぐ実装でないなら、Spawn 前に必ず UnRegister しておくのが重要）
				CollisionManager::GetInstance().Register(newCol);
				// b->SetIsRegistered(true); // ※SphereHitBox にそのメソッドがある場合のみ
				newCol->SetEnable(true);
			}

			// プール上でアクティブにする
			b->SetActive(true);

			// デバッグログ（必要なら有効化）
			// printfDx("BulletPool::Spawn -> ptr=%p owner=%s pos=(%f,%f,%f) radius=%f life=%f\n", b, owner ? owner->GetTag().c_str() : "NULL", pos.x, pos.y, pos.z, radius, life);
			b->SetOwner(owner);

			return b;
	}


	// プール枯渇 → 必要なら拡張可能
	return nullptr;
}

SphereHitBox* BulletPool::BulletSpawn(Character* owner, const VECTOR& pos, const VECTOR& vel, float radius, float life, int chain) {
	for (auto b : pool) {
		if (!b->GetActive()) {
			// --- 再利用前に古いコライダーの残滓がないか安全にクリーンアップ ---
			CleanupColliderForBullet(b);

			// Reset で内部状態を初期化（位置・速度・有効化フラグは Reset でやる設計）
			b->BulletReset(owner, pos, vel, radius, life, chain);

			// Spawn 後は必ず Collider を登録して有効化する（Reset が登録しない設計の場合）
			auto newCol = b->GetCollider();
			if (newCol) {
				// Register（CollisionManager が二重登録を防ぐ実装でないなら、Spawn 前に必ず UnRegister しておくのが重要）
				CollisionManager::GetInstance().Register(newCol);
				// b->SetIsRegistered(true); // ※SphereHitBox にそのメソッドがある場合のみ
				newCol->SetEnable(true);
			}

			// プール上でアクティブにする
			b->SetActive(true);

			// デバッグログ（必要なら有効化）
			// printfDx("BulletPool::Spawn -> ptr=%p owner=%s pos=(%f,%f,%f) radius=%f life=%f\n", b, owner ? owner->GetTag().c_str() : "NULL", pos.x, pos.y, pos.z, radius, life);

			return b;
		}
	}
	return nullptr;
}

void BulletPool::Update() {
	// 各弾を更新。Update の中で寿命切れなどで SetActive(false) になった場合は即座にコライダーを解除する
	for (auto b : pool) {
		if (!b) continue;
		if (b->GetActive()) {
			b->Update();
			// Update の結果として非アクティブになった場合（寿命切れ等）、コライダーを解除しておく
			if (!b->GetActive()) {
				CleanupColliderForBullet(b);
			}
		}
	}
}

void BulletPool::Render() {
	for (auto b : pool) if (b->GetActive()) b->Render();
}
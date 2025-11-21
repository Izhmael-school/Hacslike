#include "BulletPool.h"

BulletPool::BulletPool(int poolSize) {
	pool.reserve(poolSize);
	for (int i = 0; i < poolSize; i++) {
		SphereHitBox* b = new SphereHitBox();
		b->SetActive(false);
		pool.push_back(b);
	}
}

BulletPool::~BulletPool() {
	for (auto b : pool) delete b;
}

SphereHitBox* BulletPool::Spawn(GameObject* owner, const VECTOR& pos, const VECTOR& vel, float radius, float life) {
	for (auto b : pool) {
		if (!b->GetActive()) {
			b->Reset(owner, pos, vel, radius, life);
			return b;
			printfDx("SphereHitBox Constructor Called!\n");
		}
	}
	// プール枯渇 → 必要なら拡張可能
	return nullptr;
}

void BulletPool::Update() {
	for (auto b : pool) if (b->GetActive()) b->Update();
}

void BulletPool::Render() {
	for (auto b : pool) if (b->GetActive()) b->Render();
}
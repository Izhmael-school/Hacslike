#pragma once
#include <vector>
#include "../../Component/Collider/SphereHitBox.h"

class BulletPool {
public:
	static BulletPool& GetInstance() {
		static BulletPool instance(30); // ‰Šúƒv[ƒ‹”
		return instance;
	}

	SphereHitBox* Spawn(Character* owner, const VECTOR& pos, const VECTOR& vel, float radius, float life);
	void Update();
	void Render();

private:
	BulletPool(int poolSize);
	~BulletPool();
	BulletPool(const BulletPool&) = delete;
	BulletPool& operator=(const BulletPool&) = delete;

	std::vector<SphereHitBox*> pool;
};
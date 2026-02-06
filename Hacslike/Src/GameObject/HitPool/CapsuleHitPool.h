#pragma once
#include <vector>
#include "../../Component/Collider/CapsuleHitBox.h"
class CapsuleHitPool {
public:
	static CapsuleHitPool& GetInstance() {
		static CapsuleHitPool instance(128);	//	‰Šúƒv[ƒ‹”
		return instance;
	}

	CapsuleHitBox* Spawn(GameObject* _owner, const VECTOR& p1, const VECTOR& p2, const VECTOR& vel,float _radius, float _lifeTime);
	void Update();
	void Render();

private:
	CapsuleHitPool(int poolSize);
	~CapsuleHitPool();
	CapsuleHitPool(const CapsuleHitPool&) = delete;
	CapsuleHitPool& operator = (const CapsuleHitPool&) = delete;

	std::vector<CapsuleHitBox*> pool;
};


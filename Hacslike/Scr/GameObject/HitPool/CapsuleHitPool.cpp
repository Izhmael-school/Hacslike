#include "CapsuleHitPool.h"

CapsuleHitPool::CapsuleHitPool(int poolSize) {
	pool.reserve(poolSize);
	for (int i = 0; i < poolSize; i++) {
		CapsuleHitBox* c = new CapsuleHitBox();
		c->SetActive(false);
		pool.push_back(c);
	}
}

CapsuleHitPool::~CapsuleHitPool() {
	for (auto c : pool) delete c;
}

CapsuleHitBox* CapsuleHitPool::Spawn(GameObject* _owner, const VECTOR& p1, const VECTOR& p2, const VECTOR& vel, float _radius, float _lifeTime) {
	for (auto c : pool) {
		if (!c->GetActive()) {
			c->Reset(_owner, p1, p2, vel, _radius, _lifeTime);
			return c;
		}
	}
	//	ƒv[ƒ‹‚ªŒÍŠ‰‚µ‚½ê‡‚ÌŠg’£
	return nullptr;
}

void CapsuleHitPool::Update() {
	for (auto c : pool) if (c->GetActive()) c->Update();
}

void CapsuleHitPool::Render() {
	for (auto c : pool) if (c->GetActive()) c->Render();
}
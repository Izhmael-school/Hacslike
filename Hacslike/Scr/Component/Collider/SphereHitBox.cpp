#include "SphereHitBox.h"

SphereHitBox::SphereHitBox(GameObject* ownwr, VECTOR _offset, float _radius, float _lifeTime)
	: pCollider(nullptr)
	, timer(0.0f)
	, lifeTime(_lifeTime){
	pCollider = new SphereCollider(ownwr, _offset, _radius);
	pCollider->SetEnable(true);
}

SphereHitBox::~SphereHitBox() {
	if (pCollider) {
		delete pCollider;
		pCollider = nullptr;
	}
}

void SphereHitBox::Update() {
	timer += 1.0f / 60.0f;
	if (pCollider && pCollider->IsEnable()) {
		pCollider->Update();
	}
}

void SphereHitBox::Render() {
	if (pCollider) {
		pCollider->Render();
	}
}

bool SphereHitBox::IsDead() const {
	return timer >= lifeTime;
}

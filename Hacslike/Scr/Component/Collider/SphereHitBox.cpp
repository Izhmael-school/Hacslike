#include "SphereHitBox.h"
#include "../Hacslike/Scr/Manager/TimeManager.h"
<<<<<<< Updated upstream
=======
#include "../Hacslike/Scr/Manager/CollisionManager.h"
#include "../../GameObject/Character/Character.h"
>>>>>>> Stashed changes

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
	timer += TimeManager::GetInstance().deltaTime;
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
<<<<<<< Updated upstream
=======

void SphereHitBox::CreateCollider() {
	if (!pCollider) {
		SetPosition(owner->GetPosition());
		pCollider = new SphereCollider(this, offset, radius);
		pCollider->SetEnable(true);
		CollisionManager::GetInstance()->Register(pCollider);
	}
}

void SphereHitBox::OnTriggerEnter(Collider* _pCol) {
	if (_pCol->GetGameObject()->GetTag() == "Enemy") {
		Character* c = dynamic_cast<Character*>(_pCol->GetGameObject());
		c->SubHp(1000);
	}
}

void SphereHitBox::OnTriggerStay(Collider* _pCol) {
}

void SphereHitBox::OnTriggerExit(Collider* _pCol) {
}
>>>>>>> Stashed changes

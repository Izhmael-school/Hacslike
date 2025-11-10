#include "SphereHitBox.h"
#include "../Hacslike/Scr/Manager/TimeManager.h"
#include "../Hacslike/Scr/Manager/CollisionManager.h"

SphereHitBox::SphereHitBox(GameObject* _owner, VECTOR _offset, float _radius, float _lifeTime)
	: owner(_owner)
	, offset(_offset)
	, radius(_radius)
	, pCollider(nullptr)
	, timer(0.0f)
	, lifeTime(_lifeTime){
	//pCollider = new SphereCollider(ownwr, _offset, _radius);
	//pCollider->SetEnable(true);
}

SphereHitBox::~SphereHitBox() {
	if (pCollider) {
		CollisionManager::GetInstance().UnRegister(pCollider);
		delete pCollider;
		pCollider = nullptr;
	}
}

void SphereHitBox::Start() {}

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

void SphereHitBox::CreateCollider() {
	if (!pCollider) {
		SetPosition(owner->GetPosition());
		pCollider = new SphereCollider(this, offset, radius);
		pCollider->SetEnable(true);
		CollisionManager::GetInstance().Register(pCollider);
	}
}

void SphereHitBox::OnTriggerEnter(Collider* _pCol) {
	//	“–‚½‚Á‚½‘ŠŽè‚Ìƒ^ƒO‚ª "Goblin" ‚¾‚Á‚½‚ç
	if (_pCol->GetGameObject()->GetTag() == "Enemy") {
		//	“–‚½‚Á‚½‘ŠŽè‚ð”ñ•\Ž¦‚É‚·‚é
		_pCol->GetGameObject()->SetVisible(false);
	}
}

void SphereHitBox::OnTriggerStay(Collider* _pCol) {
}

void SphereHitBox::OnTriggerExit(Collider* _pCol) {
}

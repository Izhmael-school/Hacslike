#include "SphereHitBox.h"
#include "../Hacslike/Scr/Manager/TimeManager.h"
#include "../Hacslike/Scr/Manager/CollisionManager.h"
#include "../../GameObject/Character/Character.h"
#include "../../Manager/AudioManager.h"

SphereHitBox::SphereHitBox(GameObject* _owner, VECTOR _offset, float _radius, float _lifeTime)
	: owner(_owner)
	, offset(_offset)
	, radius(_radius)
	, pCollider(nullptr)
	, timer(0.0f)
	, lifeTime(_lifeTime) {
	Start();
}

SphereHitBox::~SphereHitBox() {
	if (pCollider) {
		CollisionManager::GetInstance().UnRegister(pCollider);
		delete pCollider;
		pCollider = nullptr;
	}
}

void SphereHitBox::Start() {
	CreateCollider();
	character = static_cast<Character*>(owner);

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

void SphereHitBox::CreateCollider() {
	if (pCollider == nullptr) {
		SetPosition(VAdd(owner->GetPosition(),offset));
		pCollider = new SphereCollider(this, offset, radius);
		pCollider->SetEnable(true);
	}
}

void SphereHitBox::OnTriggerEnter(Collider* _pCol) {
	Character* pTarget = _pCol->GetCharacter();

	//	“–‚½‚Á‚½‘ŠŽè‚Ìƒ^ƒO‚ª "Enemy" ‚© "Player" ‚©‚Â“–‚½‚Á‚½‘ÎÛ‚Æ“–‚½‚è”»’è‚ÌŽ‚¿Žå‚ªˆá‚¤ê‡
	if ((pTarget->CompareTag("Enemy") || pTarget->CompareTag("Player")) && owner->GetTag() != pTarget->GetTag()) {
		_pCol->GetCharacter()->Damage(pTarget->GetAtk());
		AudioManager::GetInstance().PlayOneShot("damage");
	}
}

void SphereHitBox::OnTriggerStay(Collider* _pCol) {}

void SphereHitBox::OnTriggerExit(Collider* _pCol) {}

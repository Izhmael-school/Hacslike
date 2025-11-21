#include "CapsuleHitBox.h"
#include "../Hacslike/Scr/Manager/TimeManager.h"
#include "../Hacslike/Scr/Manager/CollisionManager.h"
#include "../../Manager/AudioManager.h"

CapsuleHitBox::CapsuleHitBox(GameObject* _owner, const VECTOR& p1, const VECTOR& p2, float _radius, float _life)
	: owner(_owner)
	, startPos(p1)
	, endPos(p2)
	, radius(_radius)
	, pCollider(nullptr)
	, timer(0.0f)
	, lifeTime(_life){
	Start();
}

CapsuleHitBox::~CapsuleHitBox() {
	if (pCollider) {
		CollisionManager::GetInstance().UnRegister(pCollider);
		delete pCollider;
		pCollider = nullptr;
	}
}

void CapsuleHitBox::CreateCollider() {
	if (!pCollider) {
		SetPosition(owner->GetPosition());
		pCollider = new CapsuleCollider(this, startPos, endPos, radius);
		pCollider->SetEnable(true);
		//CollisionManager::GetInstance().Register(pCollider);
	}
}



void CapsuleHitBox::Start() {
	AudioManager::GetInstance().Load("Res/Audio/SE/Player/Damage.mp3", "damage", false);
}

void CapsuleHitBox::Update() {
	timer += TimeManager::GetInstance().deltaTime;
	if (pCollider && pCollider->IsEnable()) {
		pCollider->Update();
	}
}

void CapsuleHitBox::Render() {
	if (pCollider) {
		pCollider->Render();
	}
}

bool CapsuleHitBox::IsDead() const {
	return timer >= lifeTime;
}

void CapsuleHitBox::OnTriggerEnter(Collider* _pCol) {
	Character* pTarget = _pCol->GetCharacter();

	//	“–‚½‚Á‚½‘ŠŽè‚Ìƒ^ƒO‚ª "Enemy" ‚© "Player" ‚©‚Â“–‚½‚Á‚½‘ÎÛ‚Æ“–‚½‚è”»’è‚ÌŽ‚¿Žå‚ªˆá‚¤ê‡
	if ((pTarget->CompareTag("Enemy") || pTarget->CompareTag("Player")) && owner->GetTag() != pTarget->GetTag()) {
		_pCol->GetCharacter()->Damage(pTarget->GetAtk());
		AudioManager::GetInstance().PlayOneShot("damage");
	}
}

void CapsuleHitBox::OnTriggerStay(Collider* _pCol) {
}

void CapsuleHitBox::OnTriggerExit(Collider* _pCol) {
}
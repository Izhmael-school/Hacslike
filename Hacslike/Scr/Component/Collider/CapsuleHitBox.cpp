#include "CapsuleHitBox.h"
#include "../Hacslike/Scr/Manager/TimeManager.h"
#include "../Hacslike/Scr/Manager/CollisionManager.h"

CapsuleHitBox::CapsuleHitBox(GameObject* _owner, const VECTOR& p1, const VECTOR& p2, float _radius, float _life)
	: owner(_owner)
	, startPos(p1)
	, endPos(p2)
	, radius(_radius)
	, pCollider(nullptr)
	, timer(0.0f)
	, lifeTime(_life) {
}

CapsuleHitBox::~CapsuleHitBox() {
	if (pCollider) {
		CollisionManager::GetInstance()->UnRegister(pCollider);
		delete pCollider;
		pCollider = nullptr;
	}
}

void CapsuleHitBox::CreateCollider() {
	if (!pCollider) {
		pCollider = new CapsuleCollider(owner, startPos, endPos, radius);
		pCollider->SetEnable(true);
		CollisionManager::GetInstance()->Register(pCollider);
	}
}



void CapsuleHitBox::Update() {
	timer += TimeManager::GetInstance()->deltaTime;
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
	//	“–‚½‚Á‚½‘ŠŽè‚Ìƒ^ƒO‚ª "Goblin" ‚¾‚Á‚½‚ç
	if (_pCol->GetGameObject()->GetTag() == "Goblin") {
		//	“–‚½‚Á‚½‘ŠŽè‚ð”ñ•\Ž¦‚É‚·‚é
		_pCol->GetGameObject()->SetVisible(false);
	}
}

void CapsuleHitBox::OnTriggerStay(Collider* _pCol) {
}

void CapsuleHitBox::OnTriggerExit(Collider* _pCol) {
}
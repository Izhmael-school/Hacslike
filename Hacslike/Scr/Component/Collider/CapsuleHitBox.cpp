#include "CapsuleHitBox.h"
#include "../Hacslike/Scr/Manager/TimeManager.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "../Hacslike/Scr/Manager/CollisionManager.h"
=======
<<<<<<< Updated upstream
=======
#include "../Hacslike/Scr/Manager/CollisionManager.h"
#include "../../GameObject/Character/Character.h"
>>>>>>> Stashed changes
>>>>>>> Sekino
=======
>>>>>>> parent of 536f9b8 (Add)

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
		SetPosition(owner->GetPosition());
		pCollider = new CapsuleCollider(this, startPos, endPos, radius);
		pCollider->SetEnable(true);
		CollisionManager::GetInstance()->Register(pCollider);
	}
}



void CapsuleHitBox::Start() {}

void CapsuleHitBox::Update() {
<<<<<<< HEAD
<<<<<<< HEAD
	timer += TimeManager::GetInstance()->deltaTime;
	if (pCollider && pCollider->IsEnable()) {
		pCollider->Update();
	}
=======
<<<<<<< Updated upstream
=======
>>>>>>> parent of 536f9b8 (Add)
    timer += TimeManager::GetInstance()->deltaTime;
    if (pCollider && pCollider->IsEnable()) {
        pCollider->Update();
    }
<<<<<<< HEAD
=======
	timer += TimeManager::GetInstance().deltaTime;
	if (pCollider && pCollider->IsEnable()) {
		pCollider->Update();
	}
>>>>>>> Stashed changes
>>>>>>> Sekino
=======
>>>>>>> parent of 536f9b8 (Add)
}

void CapsuleHitBox::Render() {
	if (pCollider) {
		pCollider->Render();
	}
}

bool CapsuleHitBox::IsDead() const {
<<<<<<< HEAD
<<<<<<< HEAD
=======
<<<<<<< Updated upstream
    return timer >= lifeTime;
=======
>>>>>>> Sekino
	return timer >= lifeTime;
}

void CapsuleHitBox::OnTriggerEnter(Collider* _pCol) {
	//	“–‚½‚Á‚½‘ŠŽè‚Ìƒ^ƒO‚ª "Goblin" ‚¾‚Á‚½‚ç
	if (_pCol->GetGameObject()->GetTag() == "Enemy") {
<<<<<<< HEAD
		//	“–‚½‚Á‚½‘ŠŽè‚ð”ñ•\Ž¦‚É‚·‚é
		_pCol->GetGameObject()->SetVisible(false);
=======
		Character* c = dynamic_cast<Character*>(_pCol->GetGameObject());
		c->SubHp(1000);
>>>>>>> Sekino
	}
}

void CapsuleHitBox::OnTriggerStay(Collider* _pCol) {
}

void CapsuleHitBox::OnTriggerExit(Collider* _pCol) {
<<<<<<< HEAD
=======
>>>>>>> Stashed changes
>>>>>>> Sekino
=======
    return timer >= lifeTime;
>>>>>>> parent of 536f9b8 (Add)
}
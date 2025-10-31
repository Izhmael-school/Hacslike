#include "CapsuleHitBox.h"
#include "../Hacslike/Scr/Manager/TimeManager.h"
<<<<<<< Updated upstream
=======
#include "../Hacslike/Scr/Manager/CollisionManager.h"
#include "../../GameObject/Character/Character.h"
>>>>>>> Stashed changes

CapsuleHitBox::CapsuleHitBox(GameObject* owner, const VECTOR& p1, const VECTOR& p2, float radius, float life)
    : pCollider(nullptr)
    , timer(0.0f)
    , lifeTime(life) {
    pCollider = new CapsuleCollider(owner, p1, p2, radius);
    pCollider->SetEnable(true);
}

CapsuleHitBox::~CapsuleHitBox() {
    if (pCollider) {
        delete pCollider;
        pCollider = nullptr;
    }
}

void CapsuleHitBox::Update() {
<<<<<<< Updated upstream
    timer += TimeManager::GetInstance()->deltaTime;
    if (pCollider && pCollider->IsEnable()) {
        pCollider->Update();
    }
=======
	timer += TimeManager::GetInstance().deltaTime;
	if (pCollider && pCollider->IsEnable()) {
		pCollider->Update();
	}
>>>>>>> Stashed changes
}

void CapsuleHitBox::Render() {
    if (pCollider) {
        pCollider->Render();
    }
}

bool CapsuleHitBox::IsDead() const {
<<<<<<< Updated upstream
    return timer >= lifeTime;
=======
	return timer >= lifeTime;
}

void CapsuleHitBox::OnTriggerEnter(Collider* _pCol) {
	//	“–‚½‚Á‚½‘ŠŽè‚Ìƒ^ƒO‚ª "Goblin" ‚¾‚Á‚½‚ç
	if (_pCol->GetGameObject()->GetTag() == "Enemy") {
		Character* c = dynamic_cast<Character*>(_pCol->GetGameObject());
		c->SubHp(1000);
	}
}

void CapsuleHitBox::OnTriggerStay(Collider* _pCol) {
}

void CapsuleHitBox::OnTriggerExit(Collider* _pCol) {
>>>>>>> Stashed changes
}
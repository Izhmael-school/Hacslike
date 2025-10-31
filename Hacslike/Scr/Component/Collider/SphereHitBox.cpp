#include "SphereHitBox.h"
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
		CollisionManager::GetInstance()->UnRegister(pCollider);
		delete pCollider;
		pCollider = nullptr;
	}
}

void SphereHitBox::Start() {}

void SphereHitBox::Update() {
	timer += TimeManager::GetInstance()->deltaTime;
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
<<<<<<< HEAD
<<<<<<< HEAD
=======
<<<<<<< Updated upstream
=======
>>>>>>> Sekino

void SphereHitBox::CreateCollider() {
	if (!pCollider) {
		SetPosition(owner->GetPosition());
		pCollider = new SphereCollider(this, offset, radius);
		pCollider->SetEnable(true);
		CollisionManager::GetInstance()->Register(pCollider);
	}
}

void SphereHitBox::OnTriggerEnter(Collider* _pCol) {
<<<<<<< HEAD
	//	“–‚½‚Á‚½‘ŠŽè‚Ìƒ^ƒO‚ª "Goblin" ‚¾‚Á‚½‚ç
	if (_pCol->GetGameObject()->GetTag() == "Enemy") {
		//	“–‚½‚Á‚½‘ŠŽè‚ð”ñ•\Ž¦‚É‚·‚é
		_pCol->GetGameObject()->SetVisible(false);
=======
	if (_pCol->GetGameObject()->GetTag() == "Enemy") {
		Character* c = dynamic_cast<Character*>(_pCol->GetGameObject());
		c->SubHp(1000);
>>>>>>> Sekino
	}
}

void SphereHitBox::OnTriggerStay(Collider* _pCol) {
}

void SphereHitBox::OnTriggerExit(Collider* _pCol) {
}
<<<<<<< HEAD
=======
>>>>>>> Stashed changes
>>>>>>> Sekino
=======
>>>>>>> parent of 536f9b8 (Add)

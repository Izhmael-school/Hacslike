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
	, lifeTime(_lifeTime)
	, velocity()
	, active(false) {
	
	position = VAdd(owner ? owner->GetPosition() : VGet(0, 0, 0), offset); // ←ここで初期位置を決定
	Start();
}


SphereHitBox::SphereHitBox(){
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
	if (!active) return;

	timer += TimeManager::GetInstance().deltaTime;
	position = VAdd(position, velocity); // SphereHitBox の座標更新

	if (pCollider && pCollider->IsEnable()) {
		pCollider->GetGameObject()->SetPosition(position); // ←追加
		pCollider->Update();
	}

	if (timer >= lifeTime) {
		active = false;
		if (pCollider) pCollider->SetEnable(false);
		return;
	}
}

void SphereHitBox::Render() {
	if (active && pCollider) pCollider->Render();
}

bool SphereHitBox::IsDead() const {
	return timer >= lifeTime;
}

void SphereHitBox::CreateCollider() {
<<<<<<< HEAD
	if (pCollider == nullptr) {
		SetPosition(VAdd(owner->GetPosition(),offset));
=======
	if (!pCollider) {
>>>>>>> TT
		pCollider = new SphereCollider(this, offset, radius);
		pCollider->SetEnable(true);
		CollisionManager::GetInstance().Register(pCollider);
	}
}

void SphereHitBox::Move(const VECTOR& vel) {
	position = VAdd(position, vel);
}

void SphereHitBox::Reset(GameObject* _owner, const VECTOR& startPos,
	const VECTOR& _velocity, float _radius, float _life) {
	owner = _owner;
	position = startPos;
	velocity = _velocity;
	radius = _radius;   // ←必ずここで更新
	lifeTime = _life;
	timer = 0.0f;
	active = true;

	if (!pCollider) CreateCollider();
	pCollider->SetEnable(true);
	pCollider->SetRadius(radius); 
}

void SphereHitBox::OnTriggerEnter(Collider* _pCol) {
	Character* pTarget = _pCol->GetCharacter();

	//	当たった相手のタグが "Enemy" か "Player" かつ当たった対象と当たり判定の持ち主が違う場合
	if ((pTarget->CompareTag("Enemy") || pTarget->CompareTag("Player")) && owner->GetTag() != pTarget->GetTag()) {
		_pCol->GetCharacter()->Damage(pTarget->GetAtk());
		AudioManager::GetInstance().PlayOneShot("damage");
	}
}



void SphereHitBox::OnTriggerStay(Collider* _pCol) {}

void SphereHitBox::OnTriggerExit(Collider* _pCol) {}

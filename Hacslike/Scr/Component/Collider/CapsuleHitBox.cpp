#include "CapsuleHitBox.h"
#include "../Hacslike/Scr/Manager/TimeManager.h"
#include "../Hacslike/Scr/Manager/CollisionManager.h"
#include "../../Manager/AudioManager.h"

CapsuleHitBox::CapsuleHitBox(GameObject* _owner, const VECTOR& p1, const VECTOR& p2,
	float _radius, float _life)
	: owner(_owner)
	, startPos(p1)  // ← 相対座標を保存
	, endPos(p2)    // ← 相対座標を保存
	, radius(_radius)
	, pCollider(nullptr)
	, timer(0.0f)
	, lifeTime(_life)
	, velocity(VGet(0, 0, 0))
	, active(false) {
	Start();
}

CapsuleHitBox::CapsuleHitBox()
	: owner(nullptr)
	, startPos()
	, endPos()
	, radius(0)
	, pCollider(nullptr)
	, timer(0)
	, lifeTime(0)
	, velocity()
	, active(false) {
}
CapsuleHitBox::~CapsuleHitBox() {
	if (pCollider) {
		CollisionManager::GetInstance().UnRegister(pCollider);
		delete pCollider;
		pCollider = nullptr;
	}
}

void CapsuleHitBox::Move(const VECTOR& vel) {
	position = VAdd(position, vel);
}



void CapsuleHitBox::Start() {
	AudioManager::GetInstance().Load("Res/Audio/SE/Player/Damage.mp3", "damage", false);

	CreateCollider();
	character = static_cast<Character*>(owner);
}

void CapsuleHitBox::Update() {
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


void CapsuleHitBox::Render() {
	if (pCollider) {
		pCollider->Render();
	}
}

bool CapsuleHitBox::IsDead() const {
	return timer >= lifeTime;
}

void CapsuleHitBox::CreateCollider() {
	if (pCollider == nullptr) {
		SetPosition(owner->GetPosition());
	}
	if (!pCollider) {
		pCollider = new CapsuleCollider(this, startPos, endPos, radius);
		pCollider->SetEnable(true);
		CollisionManager::GetInstance().Register(pCollider);
	}
}

void CapsuleHitBox::Reset(GameObject* _owner, const VECTOR& p1, const VECTOR& p2,
	const VECTOR& _velocity, float _radius, float _life) {

	owner = _owner;
	startPos = p1;  // ローカル
	endPos = p2;  // ローカル
	velocity = _velocity;
	radius = _radius;
	lifeTime = _life;
	timer = 0.0f;
	active = true;

	if (!pCollider) CreateCollider();

	pCollider->SetEnable(true);
	pCollider->SetRadius(radius);
}

void CapsuleHitBox::OnTriggerEnter(Collider* _pCol) {
	Character* pTarget = _pCol->GetCharacter();

	//	当たった相手のタグが "Enemy" か "Player" かつ当たった対象と当たり判定の持ち主が違う場合
	if ((pTarget->CompareTag("Enemy") || pTarget->CompareTag("Player")) && owner->GetTag() != pTarget->GetTag()) {
		_pCol->GetCharacter()->Damage(pTarget->GetAtk());
		AudioManager::GetInstance().PlayOneShot("damage");
	}
}

void CapsuleHitBox::OnTriggerStay(Collider* _pCol) {
}

void CapsuleHitBox::OnTriggerExit(Collider* _pCol) {
}
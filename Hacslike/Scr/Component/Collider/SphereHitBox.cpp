#include "SphereHitBox.h"
#include "../Hacslike/Scr/Manager/TimeManager.h"
#include "../Hacslike/Scr/Manager/CollisionManager.h"
#include "../../GameObject/Character/Character.h"
#include "../../Manager/AudioManager.h"
#include "../../GameObject/Character/Player/PlayerAttack.h"

SphereHitBox::SphereHitBox(Character* _owner, VECTOR _offset, float _radius, float _lifeTime)
	: owner(_owner)
	, offset(_offset)
	, radius(_radius)
	, pCollider(nullptr)
	, timer(0.0f)
	, lifeTime(_lifeTime)
	, velocity()
	, active(false)
	, chain(){

	position = VAdd(owner ? owner->GetPosition() : VGet(0, 0, 0), offset); // ←ここで初期位置を決定
	Start();
}


SphereHitBox::SphereHitBox()
	: owner()
	, offset()
	, radius()
	, pCollider(nullptr)
	, timer(0.0f)
	, lifeTime()
	, velocity()
	, active(false)
	, character(NULL)
	, position() {
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

	// 生存時間が来たら消す
	if (timer >= lifeTime) {

		if (pCollider) {
			pCollider->SetEnable(false);
			CollisionManager::GetInstance().UnRegister(pCollider);

		}
		active = false;
		return;
	}

	// まだ生きてる → 位置更新
	position = VAdd(position, velocity);

	if (pCollider && pCollider->IsEnable()) {
		pCollider->GetGameObject()->SetPosition(position);
		pCollider->Update();
	}
}

void SphereHitBox::Render() {
	if (active && pCollider) pCollider->Render();
}

bool SphereHitBox::IsDead() const {
	return timer >= lifeTime;
}

void SphereHitBox::CreateCollider() {
	//if (pCollider == nullptr) {
	//	SetPosition(VAdd(owner->GetPosition(), offset));
	//}
	if (!pCollider) {
		pCollider = new SphereCollider(this, offset, radius);
		pCollider->SetEnable(true);
		CollisionManager::GetInstance().Register(pCollider);
	}
}

void SphereHitBox::Move(const VECTOR& vel) {
	position = VAdd(position, vel);
}

void SphereHitBox::Reset(Character* _owner, const VECTOR& startPos,
	const VECTOR& _velocity, float _radius, float _life) {
	owner = _owner;
	position = startPos;
	velocity = _velocity;
	radius = _radius;
	lifeTime = _life;
	timer = 0.0f;
	active = true;

	if (pCollider) {
		CollisionManager::GetInstance().UnRegister(pCollider);
		pCollider->SetEnable(true);
		pCollider->SetRadius(radius);

		pCollider->GetGameObject()->SetPosition(position);
	}
	else {
		CreateCollider();
		pCollider->GetGameObject()->SetPosition(position);
	}

	CollisionManager::GetInstance().Register(pCollider);

}

void SphereHitBox::BulletReset(Character* _owner, const VECTOR& startPos, const VECTOR& _velocity, float _radius, float _life, int _chain) {
	owner = _owner;
	position = startPos;
	velocity = _velocity;
	radius = _radius;
	lifeTime = _life;
	timer = 0.0f;
	active = true;
	chain = _chain;

	if (pCollider) {
		CollisionManager::GetInstance().UnRegister(pCollider);
		pCollider->SetEnable(true);
		pCollider->SetRadius(radius);

		pCollider->GetGameObject()->SetPosition(position);
	}
	else {
		CreateCollider();
		pCollider->GetGameObject()->SetPosition(position);
	}

	CollisionManager::GetInstance().Register(pCollider);
}

void SphereHitBox::OnTriggerEnter(Collider* _pCol) {
	Character* pTarget = _pCol->GetCharacter();
	if (!pTarget) {
		//printfDx("Hit: Character = NULL (owner = %s)\n", owner->GetTag().c_str());
		return;
	}

	// デバッグ出力
#if _DEBUG
	printfDx("Hit: owner=%s  ->  target=%s\n",
		owner->GetTag().c_str(),
		pTarget->GetTag().c_str());
#endif
	// 当たり判定処理
	if ((pTarget->CompareTag("Enemy") || pTarget->CompareTag("Player")) &&
		owner->GetTag() != pTarget->GetTag()) {
#if _DEBUG
			printfDx("Damage: owner=%s  target=%s  atk=%d\n",
				owner->GetTag().c_str(),
				pTarget->GetTag().c_str());
#endif

			_pCol->GetCharacter()->Damage(owner->GetAtk());
			//AudioManager::GetInstance().PlayOneShot("damage");
	}
	//Character* pTarget = _pCol->GetCharacter();
	//if (!pTarget) {
	//	printfDx("%sが当たった", !pTarget->GetTag().c_str());
	//}; // キャラクターでないなら無視

	////	当たった相手のタグが "Enemy" か "Player" かつ当たった対象と当たり判定の持ち主が違う場合
	//if ((pTarget->CompareTag("Enemy") || pTarget->CompareTag("Player")) && owner->GetTag() != pTarget->GetTag()) {
	//	

	//	printfDx("Damage: owner=%s  target=%s \n",
	//		owner->GetTag().c_str(),
	//		_pCol->GetGameObject()->GetTag().c_str());
	//	
	//	_pCol->GetCharacter()->Damage(pTarget->GetAtk());
	//	AudioManager::GetInstance().PlayOneShot("damage");
	//	
	//}

}

void SphereHitBox::OnTriggerStay(Collider* _pCol) {}

void SphereHitBox::OnTriggerExit(Collider* _pCol) {}

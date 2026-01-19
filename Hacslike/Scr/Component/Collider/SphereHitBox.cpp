#include "SphereHitBox.h"
#include "../../GameObject/Character/Character.h"
#include "../../GameObject/Character/Player/PlayerAttack.h"
#include "../../Manager/TimeManager.h"
#include "../../Manager/AudioManager.h"
#include "../../Manager/CollisionManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="_owner"></param>
/// <param name="_offset"></param>
/// <param name="_radius"></param>
/// <param name="_lifeTime"></param>
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

/// <summary>
/// コンストラクタ
/// </summary>
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

/// <summary>
/// デストラクタ
/// </summary>
SphereHitBox::~SphereHitBox() {
	if (pCollider) {
		CollisionManager::GetInstance().UnRegister(pCollider);
		delete pCollider;
		pCollider = nullptr;
	}
}

/// <summary>
/// 初期化処理
/// </summary>
void SphereHitBox::Start() {
	CreateCollider();
	character = static_cast<Character*>(owner);
}

/// <summary>
/// 更新処理
/// </summary>
void SphereHitBox::Update() {
	if (!active) return;

	timer += TimeManager::GetInstance().deltaTime;

	// 生存時間が来たら消す
	if (timer >= lifeTime) {
		active = false;
		if (pCollider) {
			pCollider->SetEnable(false);
			CollisionManager::GetInstance().UnRegister(pCollider);
		}
		return;
	}

	// --- 跳ね返り巡回ロジック ---
	if (!targetPoints.empty() && currentTargetIndex < targetPoints.size()) {
		VECTOR targetPos = targetPoints[currentTargetIndex];

		// ターゲットへのベクトル
		VECTOR toTarget = VSub(targetPos, position);

		// VSizeでベクトルの長さを取得
		float dist = VSize(toTarget);

		// 目的地に十分近づいたか？ (20.0fは調整してください)
		if (dist < 60.0f) {
			currentTargetIndex++;

			if (currentTargetIndex < targetPoints.size()) {
				// 次の地点への方向ベクトルを作成
				VECTOR nextDir = VSub(targetPoints[currentTargetIndex], position);
				// 正規化して速度を掛ける
				velocity = VScale(VNorm(nextDir), moveSpeed);
			}
			else {
				// 5箇所全て回りきったら消滅
				timer = lifeTime;
			}
		}
	}

	// 位置更新
	position = VAdd(position, velocity);

	if (pCollider && pCollider->IsEnable()) {
		pCollider->GetGameObject()->SetPosition(position);
		pCollider->Update();
	}
}

/// <summary>
/// 描画処理
/// </summary>
void SphereHitBox::Render() {
	if (active && pCollider) pCollider->Render();
}

/// <summary>
/// 消す処理
/// </summary>
/// <returns></returns>
bool SphereHitBox::IsDead() const {
	return timer >= lifeTime;
}

/// <summary>
/// コライダーの作成
/// </summary>
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

/// <summary>
/// コライダーの移動
/// </summary>
/// <param name="vel"></param>
void SphereHitBox::Move(const VECTOR& vel) {
	position = VAdd(position, vel);
}

/// <summary>
/// リセット
/// </summary>
/// <param name="_owner"></param>
/// <param name="startPos"></param>
/// <param name="_velocity"></param>
/// <param name="_radius"></param>
/// <param name="_life"></param>
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

/// <summary>
/// 弾用のリセット
/// </summary>
/// <param name="_owner"></param>
/// <param name="startPos"></param>
/// <param name="_velocity"></param>
/// <param name="_radius"></param>
/// <param name="_life"></param>
/// <param name="_chain"></param>
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

/// <summary>
/// 当たった瞬間
/// </summary>
/// <param name="_pCol"></param>
void SphereHitBox::OnTriggerEnter(Collider* _pCol) {
	if (owner == nullptr) return;

	Character* pTarget = _pCol->GetCharacter();

	// 当たり判定処理
	if ((pTarget->CompareTag("Enemy") || pTarget->CompareTag("Player")) &&
		owner->GetTag() != pTarget->GetTag()) {
		_pCol->GetCharacter()->Damage(owner->GetAtk());
		AudioManager::GetInstance().PlayOneShot("damage");
	}
}

/// <summary>
/// 当たってる間
/// </summary>
/// <param name="_pCol"></param>
void SphereHitBox::OnTriggerStay(Collider* _pCol) {}

/// <summary>
/// 離れた瞬間
/// </summary>
/// <param name="_pCol"></param>
void SphereHitBox::OnTriggerExit(Collider* _pCol) {}

#include "EnemyGoblin.h"
#include "../../../../Component/Collider/Collider.h"


EnemyGoblin::EnemyGoblin(int mHandle) {
	SetModelHandle(mHandle);
	// ステータスの設定
	SetStatusData(0);
	Start();
}

EnemyGoblin::~EnemyGoblin() {}

void EnemyGoblin::Start() {
	Enemy::Start();
	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 150, 0), 30);

	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01",14,2,150,150);
	SetAnimEventForAttackCollider("attack02",12,2,150,150);
	SetAnimEventForAttackCollider("attack02",24,2,120,150);
	SetAnimEventForAttackCollider("attack03",33,2,200,150);
	// 攻撃の効果音
	pAnimator->GetAnimation("attack01")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 14);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 12);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 24);
	pAnimator->GetAnimation("attack03")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("Impact"); }, 32);
	pAnimator->GetAnimation("dead")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, 30);
}

void EnemyGoblin::Update() {
	Enemy::Update();
}

void EnemyGoblin::Render() {
	Enemy::Render();
}

void EnemyGoblin::OnTriggerEnter(Collider* _pOther) {
	if (IsDead()) return;
	Enemy::OnTriggerEnter(_pOther);
}

#include "EnemyWolf.h"

EnemyWolf::EnemyWolf(int mHandle) {
	SetModelHandle(mHandle);
	// ステータスの設定
	SetStatusData(3);
	Start();
}

EnemyWolf::~EnemyWolf() {}

void EnemyWolf::Start() {

	Enemy::Start();
	// 当たり判定の設定
	pCollider = new SphereCollider(this, position, 100);
	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", 15, 2, 100, 150);
	SetAnimEventForAttackCollider("attack02", 12, 2, 100, 100);
	SetAnimEventForAttackCollider("attack02", 18, 2, 75, 100);
	SetAnimEventForAttackCollider("attack02", 22, 2, 50, 100);
	// 効果音
	pAnimator->GetAnimation("attack01")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 15);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 12);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 18);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 22);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 28);
	pAnimator->GetAnimation("dead")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, 23);

}

void EnemyWolf::Update() {
	Enemy::Update();
}

void EnemyWolf::Render() {
	Enemy::Render();
}

void EnemyWolf::OnTriggerEnter(Collider* _pOther) {
	if (IsDead()) return;
	Enemy::OnTriggerEnter(_pOther);
}

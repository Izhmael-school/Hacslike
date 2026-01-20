#include "EnemyHellHound.h"

EnemyHellHound::EnemyHellHound(int mHandle) {
	SetModelHandle(mHandle);
	// ステータスの設定
	SetStatusData(6);
	Start();
}

EnemyHellHound::~EnemyHellHound() {}

void EnemyHellHound::Start() {
	Enemy::Start();
	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 0, 0), VGet(0, 350, 0), 120);
	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", 16, 2, 150, 250);
	SetAnimEventForAttackCollider("attack02", 12, 2, 125, 140);
	SetAnimEventForAttackCollider("attack02", 22, 2, 100, 140);
	SetAnimEventForAttackCollider("attack02", 32, 2, 75, 140);
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("Bite2"); }, 16);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Bite2"); }, 12);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Bite2"); }, 22);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Bite2"); }, 32);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, 19);
}

void EnemyHellHound::Update() {
	Enemy::Update();
}

void EnemyHellHound::Render() {
	Enemy::Render();
}

void EnemyHellHound::OnTriggerEnter(Collider* _pOther) {
	if (IsDead()) return;
	Enemy::OnTriggerEnter(_pOther);
}

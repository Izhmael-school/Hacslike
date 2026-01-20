#include "EnemyZombie.h"

EnemyZombie::EnemyZombie(int mHandle) {
	SetModelHandle(mHandle);
	// ステータスの設定
	SetStatusData(5);
	Start();
}

EnemyZombie::~EnemyZombie() {}

void EnemyZombie::Start() {
	Enemy::Start();
	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 0, 0), VGet(0, 600, 0), 60);
	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", 16, 2, 100, 100);
	SetAnimEventForAttackCollider("attack02", 14, 2, 100, 100);
	SetAnimEventForAttackCollider("attack02", 36, 2, 70, 100);
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("Bite1"); }, 16);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Bite2"); }, 14);
	SetAnimEvent("attack03", [this]() {AudioManager::GetInstance().PlayOneShot("Bite2"); }, 36);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, 41);
}

void EnemyZombie::Update() {
	Enemy::Update();
}

void EnemyZombie::Render() {
	Enemy::Render();

}

void EnemyZombie::OnTriggerEnter(Collider* _pOther) {
	if (IsDead()) return;
	Enemy::OnTriggerEnter(_pOther);
}

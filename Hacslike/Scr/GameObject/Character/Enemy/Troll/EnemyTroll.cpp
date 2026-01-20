#include "EnemyTroll.h"

EnemyTroll::EnemyTroll(int modelHandle) {
	SetModelHandle(modelHandle);
	// ステータスの設定
	SetStatusData(4);
	Start();
}

EnemyTroll::~EnemyTroll() {}

void EnemyTroll::Start() {
	Enemy::Start();
	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 00, 0), VGet(0, 250, 0), 70);

	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", 19, 2, 200, 300);
	SetAnimEventForAttackCollider("attack02", 15, 2, 200, 150);
	SetAnimEventForAttackCollider("attack03", 15, 2, 200, 150);
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("Impact"); }, 19);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Impact"); }, 15);
	SetAnimEvent("attack03", [this]() {AudioManager::GetInstance().PlayOneShot("Impact"); }, 15);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, 38);
}

void EnemyTroll::Update() {
	Enemy::Update();
}

void EnemyTroll::Render() {
	Enemy::Render();
}

void EnemyTroll::OnTriggerEnter(Collider* _pOther) {
	if (IsDead()) return;
	Enemy::OnTriggerEnter(_pOther);
}

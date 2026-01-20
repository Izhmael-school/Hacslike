#include "EnemyDurahan.h"

EnemyDurahan::EnemyDurahan(int mHandle) {
	SetModelHandle(mHandle);
	// ステータスの設定
	SetStatusData(9);
	Start();
}

void EnemyDurahan::Start() {
	Enemy::Start();
	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 0, 0), VGet(0, 500, 0), 100);

	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", 43, 2, 200, 250);
	SetAnimEventForAttackCollider("attack02", 19, 2, 75, 350);
	SetAnimEventForAttackCollider("attack02", 19, 2, 50, 250);
	SetAnimEventForAttackCollider("attack02", 19, 2, 50, 175);
	SetAnimEventForAttackCollider("attack02", 19, 2, 50, 100);
	SetAnimEventForAttackCollider("attack03", 31, 2, 150, 400);
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("Axe"); }, 43);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Axe"); }, 19);
	SetAnimEvent("attack03", [this]() {AudioManager::GetInstance().PlayOneShot("Axe"); }, 19);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); },33 );
}

void EnemyDurahan::Update() {
	Enemy::Update();
}

void EnemyDurahan::Render() {
	Enemy::Render();
}

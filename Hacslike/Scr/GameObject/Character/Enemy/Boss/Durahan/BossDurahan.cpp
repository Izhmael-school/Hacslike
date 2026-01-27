#include "BossDurahan.h"

BossDurahan::BossDurahan() {
	Start();
}

BossDurahan::~BossDurahan() {}

void BossDurahan::Start() {
	// ステータスの設定
	SetStatusData(13);
	BossBase::Start();

	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 800, 0), 50);

	// 攻撃関連変数の代入
	attack01ColliderRadius = 200;
	attack02ColliderRadius = 75;
	float attack02ColliderRadius02 = 50;
	attack03ColliderRadius = 150;
	attack01ColliderSpawnTime = 43;
	attack02ColliderSpawnTime = 19;
	attack03ColliderSpawnTime = 31;

	deadAnimationTime = 33;

	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime, colliderLifeTime, attack01ColliderRadius, 250, 2.0f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius, 350);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius02, 250, 0.6f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius02, 175, 0.6f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius02, 100, 0.6f);
	SetAnimEventForAttackCollider("attack03", attack03ColliderSpawnTime, colliderLifeTime, attack03ColliderRadius, 400, 1.5f);
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("Axe"); }, attack01ColliderSpawnTime);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Axe"); }, attack02ColliderSpawnTime);
	SetAnimEvent("attack03", [this]() {AudioManager::GetInstance().PlayOneShot("Axe"); }, attack03ColliderSpawnTime);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, deadAnimationTime);

}

void BossDurahan::Update() {
	BossBase::Update();
}

void BossDurahan::Render() {
	BossBase::Render();
}

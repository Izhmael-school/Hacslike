#include "BossHellHound.h"

BossHellHound::BossHellHound() {
	Start();
}

BossHellHound::~BossHellHound() {}

void BossHellHound::Start() {
	// ステータスの設定
	SetStatusData(11);
	BossBase::Start();

	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 800, 0), 50);

	// 攻撃関連変数の代入
	attack01ColliderRadius = 150;
	attack02ColliderRadius = 125;
	float attack02ColliderRadius02 = 100;
	float attack02ColliderRadius03 = 75;
	attack01ColliderSpawnTime = 16;
	attack02ColliderSpawnTime = 12;
	float attack02ColliderSpawnTime02 = 22;
	float attack02ColliderSpawnTime03 = 32;

	deadAnimationTime = 19;

	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime, colliderLifeTime, attack01ColliderRadius, 250);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius, 140, 0.7f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime02, colliderLifeTime, attack02ColliderRadius02, 140, 0.3f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime03, colliderLifeTime, attack02ColliderRadius03, 140, 0.3f);
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("Bite2"); }, attack01ColliderSpawnTime);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Bite2"); }, attack02ColliderSpawnTime);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Bite2"); }, attack02ColliderSpawnTime02);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Bite2"); }, attack02ColliderSpawnTime03);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, deadAnimationTime);
}

void BossHellHound::Update() {
	BossBase::Update();
}

void BossHellHound::Render() {
	BossBase::Render();
}

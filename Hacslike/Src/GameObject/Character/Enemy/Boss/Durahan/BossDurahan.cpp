#include "BossDurahan.h"

BossDurahan::BossDurahan(VECTOR _appearPos)
	:BossBase(_appearPos)
{
	Start();
}

BossDurahan::~BossDurahan() {}

void BossDurahan::Start() {
	// ステータスの設定
	SetStatusData(14);
	BossBase::Start();

	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 800, 0), 200);

	// 攻撃関連変数の代入
	attack01ColliderRadius = 200;
	float attack01ColliderRadius02 = 400;
	attack02ColliderRadius = 200;
	float attack02ColliderRadius02 = 100;
	attack03ColliderRadius = 300;
	attack01ColliderSpawnTime = 43;
	float attack01ColliderSpawnTime02 = 18;
	float attack01ColliderSpawnTime03 = 25;
	float attack01ColliderSpawnTime04 = 35;
	attack02ColliderSpawnTime = 19;
	attack03ColliderSpawnTime = 31;

	deadAnimationTime = 33;

	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime, colliderLifeTime, attack01ColliderRadius, 450, 2.0f);
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime02, colliderLifeTime, attack01ColliderRadius02, 0, 0.5f);
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime03, colliderLifeTime, attack01ColliderRadius02, 0, 0.5f);
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime04, colliderLifeTime, attack01ColliderRadius02, 0, 0.5f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius, 700);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius02, 500, 0.6f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius02, 350, 0.6f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius02, 200, 0.6f);
	SetAnimEventForAttackCollider("attack03", attack03ColliderSpawnTime, colliderLifeTime, attack03ColliderRadius, 600, 1.5f);
	SetAnimEventForAttackCollider("attack03", attack03ColliderSpawnTime, colliderLifeTime, attack03ColliderRadius / 2, 100);
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

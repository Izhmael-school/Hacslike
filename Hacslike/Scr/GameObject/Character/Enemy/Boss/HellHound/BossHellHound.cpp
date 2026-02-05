#include "BossHellHound.h"

BossHellHound::BossHellHound(VECTOR _appearPos) 
	:BossBase(_appearPos)
{
	Start();
}

BossHellHound::~BossHellHound() {}

void BossHellHound::Start() {
	// ステータスの設定
	SetStatusData(11);
	BossBase::Start();

	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 800, 0), 230);

	// 攻撃関連変数の代入
	attack01ColliderRadius = 125;
	attack02ColliderRadius = 125;
	float attack02ColliderRadius02 = 100;
	float attack02ColliderRadius03 = 75;
	float attack02ColliderRadius04 = 50;
	float attack02ColliderRadius05 = 50;
	attack01ColliderSpawnTime = 13;
	float attack01ColliderSpawnTime02 = 15;
	float attack01ColliderSpawnTime03 = 17;
	float attack01ColliderSpawnTime04 = 19;
	attack02ColliderSpawnTime = 12;
	float attack02ColliderSpawnTime02 = 22;
	float attack02ColliderSpawnTime03 = 32;
	float attack02ColliderSpawnTime04 = 38;
	float attack02ColliderSpawnTime05 = 42;

	deadAnimationTime = 19;

	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime, colliderLifeTime, attack01ColliderRadius, 200,0.2f);
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime02, colliderLifeTime, attack01ColliderRadius, 250,0.4f);
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime03, colliderLifeTime, attack01ColliderRadius, 300,0.6f);
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime04, colliderLifeTime, attack01ColliderRadius, 350);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius, 280, 0.7f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime02, colliderLifeTime, attack02ColliderRadius02, 280, 0.3f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime03, colliderLifeTime, attack02ColliderRadius03, 280, 0.3f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime04, colliderLifeTime, attack02ColliderRadius04, 280, 0.2f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime05, colliderLifeTime, attack02ColliderRadius05, 280, 0.2f);
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

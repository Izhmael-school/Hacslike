#include "EnemyHobGoblin.h"

EnemyHobGoblin::EnemyHobGoblin(int mHandle) {
	SetModelHandle(mHandle);
	// ステータスの設定
	SetStatusData(10);
	Start();
}

EnemyHobGoblin::~EnemyHobGoblin() {}

void EnemyHobGoblin::Start() {
	Enemy::Start();
	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 0, 0), VGet(0, 250, 0), 50);

	// 攻撃関連変数の代入
	attack01ColliderRadius = 150;
	attack02ColliderRadius = 120;
	float attack02ColliderRadius02 = 90;
	attack03ColliderRadius = 100;
	attack01ColliderSpawnTime = 13;
	attack02ColliderSpawnTime = 12;
	float attack02ColliderSpawnTime02 = 31;
	attack03ColliderSpawnTime = 15;

	deadAnimationTime = 29;

	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime, colliderLifeTime, attack01ColliderRadius, 100);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius, 150,0.5f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime02, colliderLifeTime, attack02ColliderRadius02, 150,0.5f);
	SetAnimEventForAttackCollider("attack03", attack03ColliderSpawnTime, colliderLifeTime, attack03ColliderRadius, 150,1.5f);
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("HeadBang"); }, attack01ColliderSpawnTime);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("HeadBang"); }, attack02ColliderSpawnTime);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("HeadBang"); }, attack02ColliderSpawnTime02);
	SetAnimEvent("attack03", [this]() {AudioManager::GetInstance().PlayOneShot("HeadBang"); }, attack03ColliderSpawnTime);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, deadAnimationTime);
}

void EnemyHobGoblin::Update() { Enemy::Update(); }

void EnemyHobGoblin::Render() { Enemy::Render(); }

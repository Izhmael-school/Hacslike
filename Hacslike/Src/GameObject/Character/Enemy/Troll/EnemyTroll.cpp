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

	// 攻撃関連変数の代入
	attack01ColliderRadius = 200;
	attack02ColliderRadius = 200;
	attack03ColliderRadius = 200;
	attack01ColliderSpawnTime = 19;
	attack02ColliderSpawnTime = 15;
	attack03ColliderSpawnTime = 15;

	deadAnimationTime = 38;

	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime, colliderLifeTime, attack01ColliderRadius, 300,1.5f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius, 150);
	SetAnimEventForAttackCollider("attack03", attack03ColliderSpawnTime, colliderLifeTime, attack03ColliderRadius, 150);
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("Impact"); }, attack01ColliderSpawnTime);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Impact"); }, attack02ColliderSpawnTime);
	SetAnimEvent("attack03", [this]() {AudioManager::GetInstance().PlayOneShot("Impact"); }, attack03ColliderSpawnTime);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, deadAnimationTime);
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

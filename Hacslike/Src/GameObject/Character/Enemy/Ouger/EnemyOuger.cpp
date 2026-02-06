#include "EnemyOuger.h"

EnemyOuger::EnemyOuger(int mHandle) {
	SetModelHandle(mHandle);
	// ステータスの設定
	SetStatusData(7);
	Start();
}

EnemyOuger::~EnemyOuger() {}

void EnemyOuger::Start() {
	Enemy::Start();
	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 0, 0), VGet(0,200, 0), 120);

	// 攻撃関連変数の代入
	attack01ColliderRadius = 150;
	attack02ColliderRadius = 175;
	attack03ColliderRadius = 250;
	attack01ColliderSpawnTime = 17;
	attack02ColliderSpawnTime = 16;
	attack03ColliderSpawnTime = 48;

	deadAnimationTime = 47;

	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime, colliderLifeTime, attack01ColliderRadius, 200);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius, 200);
	SetAnimEventForAttackCollider("attack03", attack03ColliderSpawnTime, colliderLifeTime, attack03ColliderRadius, 250,2.0f);
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("Punch1"); }, attack01ColliderSpawnTime);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Punch1"); }, attack02ColliderSpawnTime);
	SetAnimEvent("attack03", [this]() {AudioManager::GetInstance().PlayOneShot("Punch2"); }, attack03ColliderSpawnTime);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, deadAnimationTime);
}

void EnemyOuger::Update() {
	Enemy::Update();
}

void EnemyOuger::Render() {
	Enemy::Render();
}

void EnemyOuger::OnTriggerEnter(Collider* _pOther) {
	if (IsDead()) return;
	Enemy::OnTriggerEnter(_pOther);
}

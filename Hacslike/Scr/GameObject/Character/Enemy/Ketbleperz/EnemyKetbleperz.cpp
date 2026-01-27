#include "EnemyKetbleperz.h"

EnemyKetbleperz::EnemyKetbleperz(int mHandle) {
	SetModelHandle(mHandle);
	// ステータスの設定
	SetStatusData(8);
	Start();
}

EnemyKetbleperz::~EnemyKetbleperz() {}

void EnemyKetbleperz::Start() {
	Enemy::Start();
	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 150, 0), 250);

	// 攻撃関連変数の代入
	attack01ColliderRadius = 200;
	attack02ColliderRadius = 200;
	attack03ColliderRadius = 200;
	attack01ColliderSpawnTime = 23;
	attack02ColliderSpawnTime = 13;
	attack03ColliderSpawnTime = 18;

	deadAnimationTime = 40;

	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime, colliderLifeTime, attack01ColliderRadius, 200);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius, 200);
	SetAnimEventForAttackCollider("attack03", attack03ColliderSpawnTime, colliderLifeTime, attack03ColliderRadius, 200);
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("HeadBang"); }, attack01ColliderSpawnTime);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("HeadBang"); }, attack02ColliderSpawnTime);
	SetAnimEvent("attack03", [this]() {AudioManager::GetInstance().PlayOneShot("HeadBang"); }, attack03ColliderSpawnTime);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, deadAnimationTime);
}

void EnemyKetbleperz::Update() { 
	Enemy::Update();
}

void EnemyKetbleperz::Render() { 
	Enemy::Render();
}

void EnemyKetbleperz::OnTriggerEnter(Collider* _pOther) {
	if (IsDead()) return;
	Enemy::OnTriggerEnter(_pOther);
}

#include "EnemyZombie.h"

EnemyZombie::EnemyZombie(int mHandle) {
	SetModelHandle(mHandle);
	// ステータスの設定
	SetStatusData(5);
	Start();
}

EnemyZombie::~EnemyZombie() {}

void EnemyZombie::Start() {
	Enemy::Start();
	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 0, 0), VGet(0, 600, 0), 60);

	// 攻撃関連変数の代入
	attack01ColliderRadius = 100;
	attack02ColliderRadius = 100;
	float attack02ColliderRadius02 = 70;
	attack01ColliderSpawnTime = 16;
	attack02ColliderSpawnTime = 14;
	float attack02ColliderSpawnTime02 = 36;

	deadAnimationTime = 41;

	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime, colliderLifeTime, attack01ColliderRadius, 100);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius, 100,0.5f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime02, colliderLifeTime, attack02ColliderRadius02, 100,0.5f);
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("Bite1"); }, attack01ColliderSpawnTime);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Bite2"); }, attack02ColliderSpawnTime);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Bite2"); }, attack02ColliderSpawnTime02);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, deadAnimationTime);
}

void EnemyZombie::Update() {
	Enemy::Update();
}

void EnemyZombie::Render() {
	Enemy::Render();

}

void EnemyZombie::OnTriggerEnter(Collider* _pOther) {
	if (IsDead()) return;
	Enemy::OnTriggerEnter(_pOther);
}

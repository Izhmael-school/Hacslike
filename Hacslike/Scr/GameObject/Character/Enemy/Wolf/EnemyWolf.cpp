#include "EnemyWolf.h"

EnemyWolf::EnemyWolf(int mHandle) {
	SetModelHandle(mHandle);
	// ステータスの設定
	SetStatusData(3);
	Start();
}

EnemyWolf::~EnemyWolf() {}

void EnemyWolf::Start() {

	Enemy::Start();
	// 当たり判定の設定
	pCollider = new SphereCollider(this, position, 100);

	// 攻撃関連変数の代入
	attack01ColliderRadius = 100;
	attack02ColliderRadius = 100;
	float attack02ColliderRadius02 = 75;
	float attack02ColliderRadius03 = 50;
	attack01ColliderSpawnTime = 15;
	attack02ColliderSpawnTime = 12;
	float attack02ColliderSpawnTime02 = 18;
	float attack02ColliderSpawnTime03 = 22;

	deadAnimationTime = 23;

	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime, attack03ColliderSpawnTime, attack01ColliderRadius, 150);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, attack03ColliderSpawnTime, attack02ColliderRadius, 100,0.6f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime02, attack03ColliderSpawnTime, attack02ColliderRadius02, 100,0.2f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime03, attack03ColliderSpawnTime, attack02ColliderRadius03, 100, 0.2f);
	// 効果音
	pAnimator->GetAnimation("attack01")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, attack01ColliderSpawnTime);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, attack02ColliderSpawnTime);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, attack02ColliderSpawnTime02);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, attack02ColliderSpawnTime03);
	pAnimator->GetAnimation("dead")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, deadAnimationTime);

}

void EnemyWolf::Update() {
	Enemy::Update();
}

void EnemyWolf::Render() {
	Enemy::Render();
}

void EnemyWolf::OnTriggerEnter(Collider* _pOther) {
	if (IsDead()) return;
	Enemy::OnTriggerEnter(_pOther);
}

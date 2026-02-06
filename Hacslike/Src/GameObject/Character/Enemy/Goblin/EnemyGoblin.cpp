#include "EnemyGoblin.h"
#include "../../../../Component/Collider/Collider.h"


EnemyGoblin::EnemyGoblin(int mHandle) {
	SetModelHandle(mHandle);
	// ステータスの設定
	SetStatusData(0);
	Start();
}

EnemyGoblin::~EnemyGoblin() {}

void EnemyGoblin::Start() {
	Enemy::Start();
	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 250, 0), 50);

	// 攻撃関連変数の代入
	attack01ColliderRadius = 150;
	attack02ColliderRadius = 150;
	float attack02ColliderRadius02 = 120;
	attack03ColliderRadius = 200;
	attack01ColliderSpawnTime = 14;
	attack02ColliderSpawnTime = 12;
	float attack02ColliderSpawnTime02 = 24;
	attack03ColliderSpawnTime = 33;




	deadAnimationTime = 30;
	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime, colliderLifeTime, attack01ColliderRadius,150);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius,150,0.5f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime02, colliderLifeTime, attack02ColliderRadius02,150,0.5f);
	SetAnimEventForAttackCollider("attack03", attack03ColliderSpawnTime, colliderLifeTime, attack03ColliderRadius,150,1.5f);
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, attack01ColliderSpawnTime);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, attack02ColliderSpawnTime);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, attack02ColliderSpawnTime02);
	SetAnimEvent("attack03", [this]() {AudioManager::GetInstance().PlayOneShot("Impact"); }, attack03ColliderSpawnTime);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, deadAnimationTime);

}

void EnemyGoblin::Update() {
	Enemy::Update();
}

void EnemyGoblin::Render() {
	Enemy::Render();
}

void EnemyGoblin::OnTriggerEnter(Collider* _pOther) {
	if (IsDead()) return;
	Enemy::OnTriggerEnter(_pOther);
}

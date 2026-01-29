#include "BossOuger.h"

BossOuger::BossOuger(VECTOR _appearPos) 
	:BossBase(_appearPos)
{
	Start();
}

BossOuger::~BossOuger() {}

void BossOuger::Start() {
	// ステータスの設定
	SetStatusData(12);
	BossBase::Start();

	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 800, 0), 100);

	// 攻撃関連変数の代入
	attack01ColliderRadius = 200;
	attack02ColliderRadius = 175;
	attack03ColliderRadius = 150;
	float attack03ColliderRadius02 = 300;
	float attack03ColliderRadius03 = 600;
	attack01ColliderSpawnTime = 17;
	attack02ColliderSpawnTime = 16;
	attack03ColliderSpawnTime = 48;

	deadAnimationTime = 47;

	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime, colliderLifeTime, attack01ColliderRadius, 200);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime - 2, colliderLifeTime, attack02ColliderRadius,VGet(-200,0,0), 200);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime - 1, colliderLifeTime, attack02ColliderRadius,VGet(-200,0,200), 200);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius, 200);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime + 1, colliderLifeTime, attack02ColliderRadius, VGet(200,0,200), 200);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime + 2, colliderLifeTime, attack02ColliderRadius, VGet(200,0,0), 200);
	SetAnimEventForAttackCollider("attack03", attack03ColliderSpawnTime, colliderLifeTime, attack03ColliderRadius, 250, 2.5f);
	SetAnimEventForAttackCollider("attack03", attack03ColliderSpawnTime, colliderLifeTime, attack03ColliderRadius02, 250, 1.0f);
	SetAnimEventForAttackCollider("attack03", attack03ColliderSpawnTime, colliderLifeTime, attack03ColliderRadius03, 250, 0.5f);
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("Punch1"); }, attack01ColliderSpawnTime);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Punch1"); }, attack02ColliderSpawnTime);
	SetAnimEvent("attack03", [this]() {AudioManager::GetInstance().PlayOneShot("Punch2"); }, attack03ColliderSpawnTime);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, deadAnimationTime);

	VECTOR pos = appearPos;
	SetAppearPos(VAdd(pos, VLeft));
	SetCirclePos(VAdd(pos, VRight));
}

void BossOuger::Update() {
	BossBase::Update();
}

void BossOuger::Render() {
	BossBase::Render();
}

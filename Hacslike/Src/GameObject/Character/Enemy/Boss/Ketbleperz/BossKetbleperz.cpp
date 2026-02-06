#include "BossKetbleperz.h"

BossKetbleperz::BossKetbleperz(VECTOR _appearPos) 
	:BossBase(_appearPos)
{
	Start();
}

BossKetbleperz::~BossKetbleperz() {}

void BossKetbleperz::Start() {

	// ステータスの設定
	SetStatusData(13);
	BossBase::Start();

	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 800, 0), 450);

	// 攻撃関連変数の代入
	attack01ColliderRadius = 200;
	float attack01ColliderRadius02 = 400;
	attack02ColliderRadius = 200;
	attack03ColliderRadius = 200;
	attack01ColliderSpawnTime = 23;
	attack02ColliderSpawnTime = 13;
	attack03ColliderSpawnTime = 18;

	deadAnimationTime = 40;

	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime, colliderLifeTime, attack01ColliderRadius, 300);
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime, colliderLifeTime, attack01ColliderRadius02, 300,0.5f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius, 300);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius, 450);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius, 600);
	SetAnimEventForAttackCollider("attack03", attack03ColliderSpawnTime, colliderLifeTime, attack03ColliderRadius, 400,2);
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("HeadBang"); }, attack01ColliderSpawnTime);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("HeadBang"); }, attack02ColliderSpawnTime);
	SetAnimEvent("attack03", [this]() {AudioManager::GetInstance().PlayOneShot("HeadBang"); }, attack03ColliderSpawnTime);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, deadAnimationTime);

}

void BossKetbleperz::Update() {
	BossBase::Update();
}

void BossKetbleperz::Render() {
	BossBase::Render();
}

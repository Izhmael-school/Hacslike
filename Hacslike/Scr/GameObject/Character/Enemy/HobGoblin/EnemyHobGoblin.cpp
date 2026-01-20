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

	// アニメーションイベントの設定
	pAnimator->GetAnimation("dead")->SetEvent([this]() { EnemyManager::GetInstance().UnuseEnemy(this); }, pAnimator->GetTotalTime("dead"));
	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", 13, 2, 150, 100);
	SetAnimEventForAttackCollider("attack02", 12, 2, 120, 150);
	SetAnimEventForAttackCollider("attack02", 31, 2, 90, 150);
	SetAnimEventForAttackCollider("attack03", 15, 2, 100, 150);
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("HeadBang"); }, 13);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("HeadBang"); }, 12);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("HeadBang"); }, 31);
	SetAnimEvent("attack03", [this]() {AudioManager::GetInstance().PlayOneShot("HeadBang"); }, 15);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, 29);
}

void EnemyHobGoblin::Update() { Enemy::Update(); }

void EnemyHobGoblin::Render() { Enemy::Render(); }

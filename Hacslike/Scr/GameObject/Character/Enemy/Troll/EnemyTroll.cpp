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
	SetScale(VGet(0.1f, 0.1f, 0.1f));
	type = Troll;

	// アニメーションの設定
	pAnimator->SetModelHandle(modelHandle);
	LoadAnimation();

	// アニメーションイベントの設定
	pAnimator->GetAnimation("dead")->SetEvent([this]() { EnemyManager::GetInstance().UnuseEnemy(this); }, pAnimator->GetTotalTime("dead"));
	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", 19,2, 200, 300);
	SetAnimEventForAttackCollider("attack02", 15,2, 200, 150);
	SetAnimEventForAttackCollider("attack03", 15,2, 200, 150);
	// 攻撃中の移動制御
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(true); }, 0);
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack01"));
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(true); }, 0);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack02"));
	pAnimator->GetAnimation("attack03")->SetEvent([this]() { SetAttacking(true); }, 0);
	pAnimator->GetAnimation("attack03")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack03"));
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("Impact"); }, 19);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Impact"); }, 15);
	SetAnimEvent("attack03", [this]() {AudioManager::GetInstance().PlayOneShot("Impact"); }, 15);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, 38);
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

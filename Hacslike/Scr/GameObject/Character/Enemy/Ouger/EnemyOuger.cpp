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
	SetScale(VGet(0.1f, 0.1f, 0.1f));

	// アニメーションの設定
	pAnimator->SetModelHandle(modelHandle);
	// アニメーションのロード
	LoadAnimation();
	// アニメーションイベントの設定
	pAnimator->GetAnimation("dead")->SetEvent([this]() { EnemyManager::GetInstance().UnuseEnemy(this); }, pAnimator->GetTotalTime("dead"));
	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", 17, 2, 150, 200);
	SetAnimEventForAttackCollider("attack02", 16, 2, 175, 200);
	SetAnimEventForAttackCollider("attack03", 48, 2, 250, 250);
	// 攻撃中の移動制御
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(true); }, 0);
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack01"));
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(true); }, 0);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack02"));
	pAnimator->GetAnimation("attack03")->SetEvent([this]() { SetAttacking(true); }, 0);
	pAnimator->GetAnimation("attack03")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack03"));
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("Punch1"); }, 17);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Punch1"); }, 16);
	SetAnimEvent("attack03", [this]() {AudioManager::GetInstance().PlayOneShot("Punch2"); }, 48);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, 47);
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

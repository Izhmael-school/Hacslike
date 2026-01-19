#include "EnemyDurahan.h"

EnemyDurahan::EnemyDurahan(int mHandle) {
	SetModelHandle(mHandle);
	// ステータスの設定
	SetStatusData(9);
	Start();
}

void EnemyDurahan::Start() {
	Enemy::Start();
	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 0, 0), VGet(0, 500, 0), 100);
	SetScale(VGet(0.1f, 0.1f, 0.1f));
	type = Durahan;

	// アニメーションの設定
	pAnimator->SetModelHandle(modelHandle);
	// アニメーションのロード
	LoadAnimation();
	// アニメーションイベントの設定
	pAnimator->GetAnimation("dead")->SetEvent([this]() { EnemyManager::GetInstance().UnuseEnemy(this); }, pAnimator->GetTotalTime("dead"));
	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", 43, 2, 200, 250);
	SetAnimEventForAttackCollider("attack02", 19, 2, 75, 350);
	SetAnimEventForAttackCollider("attack02", 19, 2, 50, 250);
	SetAnimEventForAttackCollider("attack02", 19, 2, 50, 175);
	SetAnimEventForAttackCollider("attack02", 19, 2, 50, 100);
	SetAnimEventForAttackCollider("attack03", 31, 2, 150, 400);
	// 攻撃中の移動制御
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(true); }, 0);
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack01"));
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(true); }, 0);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack02"));
	pAnimator->GetAnimation("attack03")->SetEvent([this]() { SetAttacking(true); }, 0);
	pAnimator->GetAnimation("attack03")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack03"));
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("Axe"); }, 43);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Axe"); }, 19);
	SetAnimEvent("attack03", [this]() {AudioManager::GetInstance().PlayOneShot("Axe"); }, 19);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); },33 );
}

void EnemyDurahan::Update() {
	Enemy::Update();
}

void EnemyDurahan::Render() {
	Enemy::Render();
}

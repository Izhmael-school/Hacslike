#include "EnemyHellHound.h"

EnemyHellHound::EnemyHellHound(int mHandle) {
	SetModelHandle(mHandle);
	// ステータスの設定
	SetStatusData(6);
	Start();
}

EnemyHellHound::~EnemyHellHound() {}

void EnemyHellHound::Start() {
	Enemy::Start();
	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 0, 0), VGet(0, 350, 0), 120);
	SetScale(VGet(0.1f, 0.1f, 0.1f));
	type = HellHound;

	// アニメーションの設定
	pAnimator->SetModelHandle(modelHandle);
	// アニメーションのロード
	LoadAnimation();
	// アニメーションイベントの設定
	pAnimator->GetAnimation("dead")->SetEvent([this]() { EnemyManager::GetInstance().UnuseEnemy(this); }, pAnimator->GetTotalTime("dead"));
	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", 16, 2, 150, 250);
	SetAnimEventForAttackCollider("attack02", 12, 2, 125, 140);
	SetAnimEventForAttackCollider("attack02", 22, 2, 100, 140);
	SetAnimEventForAttackCollider("attack02", 32, 2, 75, 140);
	// 攻撃中の移動制御
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(true); }, 0);
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack01"));
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(true); }, 0);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack02"));
}

void EnemyHellHound::Update() {
	Enemy::Update();
}

void EnemyHellHound::Render() {
	Enemy::Render();
}

void EnemyHellHound::OnTriggerEnter(Collider* _pOther) {
	if (IsDead()) return;
	Enemy::OnTriggerEnter(_pOther);
}

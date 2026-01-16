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
	SetScale(VGet(0.1f, 0.1f, 0.1f));
	type = Zombie;

	// アニメーションの設定
	pAnimator->SetModelHandle(modelHandle);
	// アニメーションのロード
	LoadAnimation();
	// アニメーションイベントの設定
	SetAnimEvent("dead",[this]() { EnemyManager::GetInstance().UnuseEnemy(this); }, pAnimator->GetTotalTime("dead"));
	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", 16, 2, 100, 100);
	SetAnimEventForAttackCollider("attack02", 14, 2, 100, 100);
	SetAnimEventForAttackCollider("attack02", 36, 2, 70, 100);
	// 攻撃中の移動制御
	SetAnimEvent("attack01", [this]() { SetAttacking(true); });
	SetAnimEvent("attack01", [this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack01"));
	SetAnimEvent("attack02", [this]() { SetAttacking(true); });
	SetAnimEvent("attack02", [this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack02"));

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

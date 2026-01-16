#include "EnemyKetbleperz.h"

EnemyKetbleperz::EnemyKetbleperz(int mHandle) {
	SetModelHandle(mHandle);
	// ステータスの設定
	SetStatusData(8);
	Start();
}

EnemyKetbleperz::~EnemyKetbleperz() {}

void EnemyKetbleperz::Start() {
	Enemy::Start();
	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 150, 0), 250);
	SetScale(VGet(0.1f, 0.1f, 0.1f));
	type = Ketbleperz;

	// アニメーションの設定
	pAnimator->SetModelHandle(modelHandle);
	// アニメーションのロード
	LoadAnimation();
	// アニメーションイベントの設定
	pAnimator->GetAnimation("dead")->SetEvent([this]() { EnemyManager::GetInstance().UnuseEnemy(this); }, pAnimator->GetTotalTime("dead"));
	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", 23, 2, 200, 200);
	SetAnimEventForAttackCollider("attack02", 13, 2, 200, 200);
	SetAnimEventForAttackCollider("attack03", 18, 2, 200, 200);
	// 攻撃中の移動制御
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(true); }, 7);
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack01"));
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(true); }, 0);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack02"));
	pAnimator->GetAnimation("attack03")->SetEvent([this]() { SetAttacking(true); }, 0);
	pAnimator->GetAnimation("attack03")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack03"));

}

void EnemyKetbleperz::Update() { 
	Enemy::Update();
}

void EnemyKetbleperz::Render() { 
	Enemy::Render();
}

void EnemyKetbleperz::OnTriggerEnter(Collider* _pOther) {
	if (IsDead()) return;
	Enemy::OnTriggerEnter(_pOther);
}

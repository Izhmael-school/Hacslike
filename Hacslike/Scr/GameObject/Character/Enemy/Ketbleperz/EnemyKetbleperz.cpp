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
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 150, 0), 30);
	SetScale(VGet(0.1f, 0.1f, 0.1f));
	type = Ketbleperz;

	// アニメーションの設定
	pAnimator->SetModelHandle(modelHandle);
	// アニメーションのロード
	LoadAnimation();
	// アニメーションイベントの設定
	pAnimator->GetAnimation("dead")->SetEvent([this]() { EnemyManager::GetInstance().UnuseEnemy(this); }, pAnimator->GetTotalTime("dead"));
	// 攻撃の当たり判定
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 2 / GetFPS())); }, 14);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 3 / GetFPS())); }, 12);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 2 / GetFPS())); }, 24);
	pAnimator->GetAnimation("attack03")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 2 / GetFPS())); }, 33);
	// 攻撃中の移動制御
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(true); }, 8);
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack01"));
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(true); }, 8);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack02"));
	pAnimator->GetAnimation("attack03")->SetEvent([this]() { SetAttacking(true); }, 8);
	pAnimator->GetAnimation("attack03")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack03"));

}

void EnemyKetbleperz::Update() {}

void EnemyKetbleperz::Render() {}

void EnemyKetbleperz::OnTriggerEnter(Collider* _pOther) {}

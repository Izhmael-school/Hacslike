#include "EnemyWolf.h"

EnemyWolf::EnemyWolf(int mHandle) {
	SetModelHandle(mHandle);
	// ステータスの設定
	SetStatusData(3);
	Start();
}

EnemyWolf::~EnemyWolf() {}

void EnemyWolf::Start() {

	Enemy::Start();
	// 当たり判定の設定
	pCollider = new SphereCollider(this, position, 100);
	SetScale(VGet(0.1f, 0.1f, 0.1f));
	type = EnemyType::Wolf;

	// アニメーションの設定
	pAnimator->SetModelHandle(modelHandle);
	// アニメーションのロード
	LoadAnimation();
	// アニメーションイベントの設定
	pAnimator->GetAnimation("dead")->SetEvent([this]() { EnemyManager::GetInstance().UnuseEnemy(this); }, pAnimator->GetTotalTime("dead"));
	// 攻撃の当たり判定
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 2 / GetFPS())); }, 15);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 2 / GetFPS())); }, 12);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 2 / GetFPS())); }, 18);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 2 / GetFPS())); }, 22);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 2 / GetFPS())); }, 28);
	// 攻撃中の移動制御
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(true); }, 8);
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack01"));
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(true); }, 8);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack02"));
	// 効果音
	pAnimator->GetAnimation("attack01")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 15);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 12);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 18);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 22);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 28);
	pAnimator->GetAnimation("dead")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, 23);

}

void EnemyWolf::Update() {
	Enemy::Update();
}

void EnemyWolf::Render() {
	Enemy::Render();
}

void EnemyWolf::OnTriggerEnter(Collider* _pOther) {
	if (IsDead()) return;
	Enemy::OnTriggerEnter(_pOther);
}

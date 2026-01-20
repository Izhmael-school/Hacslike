#include "BossHellHound.h"

BossHellHound::BossHellHound() {
	Start();
}

BossHellHound::~BossHellHound() {}

void BossHellHound::Start() {
	// ステータスの設定
	SetStatusData(11);
	BossBase::Start();

	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 800, 0), 50);

	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", 16, 2, 300, 250);
	SetAnimEventForAttackCollider("attack02", 12, 2, 125, 140);
	SetAnimEventForAttackCollider("attack02", 22, 2, 100, 140);
	SetAnimEventForAttackCollider("attack02", 32, 2, 75, 140);
	// 攻撃中の移動制御
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(true); }, 0);
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack01"));
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(true); }, 0);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack02"));
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("Bite2"); }, 16);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Bite2"); }, 12);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Bite2"); }, 22);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("Bite2"); }, 32);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, 19);
}

void BossHellHound::Update() {
	BossBase::Update();
}

void BossHellHound::Render() {
	BossBase::Render();
}

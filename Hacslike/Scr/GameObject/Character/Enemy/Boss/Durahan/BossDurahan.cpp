#include "BossDurahan.h"

BossDurahan::BossDurahan() {
	Start();
}

BossDurahan::~BossDurahan() {}

void BossDurahan::Start() {
	// ステータスの設定
	SetStatusData(13);
	BossBase::Start();

	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 800, 0), 50);

	type = Durahan;

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
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, 33);

}

void BossDurahan::Update() {
	BossBase::Update();
}

void BossDurahan::Render() {
	BossBase::Render();
}

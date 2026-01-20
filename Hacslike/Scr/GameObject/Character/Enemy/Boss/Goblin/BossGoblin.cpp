#include "BossGoblin.h"
#include "../../../../../Component/Collider/Collider.h"

BossGoblin::BossGoblin() {
	Start();
}

BossGoblin::~BossGoblin() {
}

void BossGoblin::Start() {

	// ƒXƒe[ƒ^ƒX‚ÌÝ’è
	SetStatusData(1);
	BossBase::Start();

	// “–‚½‚è”»’è‚ÌÝ’è
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 800, 0), 50);

	// UŒ‚‚Ì“–‚½‚è”»’è
	SetAnimEventForAttackCollider("attack01", 14, 2, 150, 150);
	SetAnimEventForAttackCollider("attack02", 12, 2, 150, 150);
	SetAnimEventForAttackCollider("attack02", 24, 2, 120, 150);
	SetAnimEventForAttackCollider("attack03", 33, 2, 200, 150);
	// UŒ‚’†‚ÌˆÚ“®§Œä
	SetAnimEvent("attack01", [this]() { SetAttacking(true); }, 0);
	SetAnimEvent("attack01", [this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack01"));
	SetAnimEvent("attack02", [this]() { SetAttacking(true); }, 0);
	SetAnimEvent("attack02", [this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack02"));
	SetAnimEvent("attack03", [this]() { SetAttacking(true); }, 0);
	SetAnimEvent("attack03", [this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack03"));
	// UŒ‚‚ÌŒø‰Ê‰¹
	pAnimator->GetAnimation("attack01")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 14);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 12);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 24);
	pAnimator->GetAnimation("attack03")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("Impact"); }, 32);
	pAnimator->GetAnimation("dead")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, 30);
}

void BossGoblin::Update() {
	BossBase::Update();
}

void BossGoblin::Render() {
	BossBase::Render();
}


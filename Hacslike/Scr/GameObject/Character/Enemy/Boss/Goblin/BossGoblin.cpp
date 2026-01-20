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
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 2 / GetFPS())); }, 14);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 3 / GetFPS())); }, 12);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 2 / GetFPS())); }, 24);
	pAnimator->GetAnimation("attack03")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 2 / GetFPS())); }, 32);
	// UŒ‚’†‚ÌˆÚ“®§Œä
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(true); }, 8);
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack01"));
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(true); }, 8);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack02"));
	pAnimator->GetAnimation("attack03")->SetEvent([this]() { SetAttacking(true); }, 8);
	pAnimator->GetAnimation("attack03")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack03"));
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


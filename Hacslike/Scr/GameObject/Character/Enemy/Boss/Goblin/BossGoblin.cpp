#include "BossGoblin.h"
#include "../../../../../Component/Collider/Collider.h"

BossGoblin::BossGoblin() {
	Start();
}

BossGoblin::~BossGoblin() {
}

void BossGoblin::Start() {
	BossBase::Start();

	SetStatusData(1);
	SetModelHandleDup(MV1LoadModel((mPath + "boss.mv1").c_str()));
	int i = modelHandle;

	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 150, 0), 30);
	SetScale(VGet(0.3f, 0.3f, 0.3f));
	type = Goblin;

	// アニメーションの設定
	pAnimator->SetModelHandle(modelHandle);
	// ステータスの設定
	SetStatusData(1);
	// アニメーションのロード
	LoadAnimation();
	// アニメーションイベントの設定
	pAnimator->GetAnimation("dead")->SetEvent([this]() {EnemyManager::GetInstance().DeleteEnemy(this); }, pAnimator->GetTotalTime("dead"));
	// 攻撃の当たり判定
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 2 / GetFPS())); }, 14);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 3 / GetFPS())); }, 12);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 2 / GetFPS())); }, 24);
	pAnimator->GetAnimation("attack03")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 2 / GetFPS())); }, 32);
	// 攻撃中の移動制御
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(true); }, 8);
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack01"));
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(true); }, 8);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack02"));
	pAnimator->GetAnimation("attack03")->SetEvent([this]() { SetAttacking(true); }, 8);
	pAnimator->GetAnimation("attack03")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack03"));
	// 攻撃の効果音
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


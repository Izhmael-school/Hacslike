#include "EnemyGoblin.h"
#include "../../../../Component/Collider/Collider.h"


EnemyGoblin::EnemyGoblin(int mHandle) {
	SetModelHandle(mHandle);
	// ステータスの設定
	SetStatusData(0);
	Start();
}

EnemyGoblin::~EnemyGoblin() {}

void EnemyGoblin::Start() {
	Enemy::Start();
	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 150, 0), 30);
	SetScale(VGet(0.1f, 0.1f, 0.1f));

	// アニメーションの設定
	pAnimator->SetModelHandle(modelHandle);
	// アニメーションのロード
	LoadAnimation();
	// アニメーションイベントの設定
	pAnimator->GetAnimation("dead")->SetEvent([this]() { EnemyManager::GetInstance().UnuseEnemy(this); }, pAnimator->GetTotalTime("dead"));
	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01",14,2,150,150);
	SetAnimEventForAttackCollider("attack02",12,2,150,150);
	SetAnimEventForAttackCollider("attack02",24,2,120,150);
	SetAnimEventForAttackCollider("attack03",33,2,200,150);
	// 攻撃中の移動制御
	SetAnimEvent("attack01", [this]() { SetAttacking(true); }, 0);
	SetAnimEvent("attack01", [this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack01"));
	SetAnimEvent("attack02", [this]() { SetAttacking(true); }, 0);
	SetAnimEvent("attack02", [this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack02"));
	SetAnimEvent("attack03", [this]() { SetAttacking(true); }, 0);
	SetAnimEvent("attack03", [this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack03"));
	// 攻撃の効果音
	pAnimator->GetAnimation("attack01")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 14);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 12);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, 24);
	pAnimator->GetAnimation("attack03")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("Impact"); }, 32);
	pAnimator->GetAnimation("dead")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, 30);
	// 攻撃のエフェクト
	//pAnimator->GetAnimation("attack01")->SetEvent([this]() { EffectManager::GetInstance().Instantiate("Claw_Vertical", position); }, 14);

}

void EnemyGoblin::Update() {
	Enemy::Update();
}

void EnemyGoblin::Render() {
	Enemy::Render();
}

void EnemyGoblin::OnTriggerEnter(Collider* _pOther) {
	if (IsDead()) return;
	Enemy::OnTriggerEnter(_pOther);
}

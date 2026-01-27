#include "EnemyGoblin.h"
#include "../../../../Component/Collider/Collider.h"


EnemyGoblin::EnemyGoblin(int mHandle) {
	SetModelHandle(mHandle);
	// ƒXƒe[ƒ^ƒX‚ÌÝ’è
	SetStatusData(0);
	Start();
}

EnemyGoblin::~EnemyGoblin() {}

void EnemyGoblin::Start() {
	Enemy::Start();
	// “–‚½‚è”»’è‚ÌÝ’è
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 150, 0), 30);

	// UŒ‚ŠÖ˜A•Ï”‚Ì‘ã“ü
	attack01ColliderRadius = 150;
	attack02ColliderRadius = 150;
	float attack02ColliderRadius02 = 120;
	attack03ColliderRadius = 200;
	attack01ColliderSpawnTime = 14;
	attack02ColliderSpawnTime = 12;
	float attack02ColliderSpawnTime02 = 24;
	attack03ColliderSpawnTime = 33;

	deadAnimationTime = 30;

	// UŒ‚‚Ì“–‚½‚è”»’è
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime, colliderLifeTime, attack01ColliderRadius,150);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius,150,0.5f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime02, colliderLifeTime, attack02ColliderRadius02,150,0.5f);
	SetAnimEventForAttackCollider("attack03", attack03ColliderSpawnTime, colliderLifeTime, attack03ColliderRadius,150,1.5f);
	// UŒ‚‚ÌŒø‰Ê‰¹
	pAnimator->GetAnimation("attack01")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, attack01ColliderSpawnTime);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, attack02ColliderSpawnTime);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, attack02ColliderSpawnTime02);
	pAnimator->GetAnimation("attack03")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("Impact"); }, attack03ColliderSpawnTime);
	pAnimator->GetAnimation("dead")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, deadAnimationTime);
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

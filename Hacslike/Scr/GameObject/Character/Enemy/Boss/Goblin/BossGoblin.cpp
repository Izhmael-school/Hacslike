#include "BossGoblin.h"
#include "../../../../../Component/Collider/Collider.h"

BossGoblin::BossGoblin(VECTOR _appearPos) 
	:BossBase(_appearPos)
{
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

	// UŒ‚ŠÖ˜A•Ï”‚Ì‘ã“ü
	attack01ColliderRadius = 150;
	attack02ColliderRadius = 150;
	float attack02ColliderRadius02 = 120;
	attack03ColliderRadius = 200;
	float attack03ColliderRadius02 = 100;
	attack01ColliderSpawnTime = 13;
	attack02ColliderSpawnTime = 12;
	float attack02ColliderSpawnTime02 = 24;
	attack03ColliderSpawnTime = 33;

	deadAnimationTime = 30;

	// UŒ‚‚Ì“–‚½‚è”»’è
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime, colliderLifeTime, attack01ColliderRadius, 150);
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime - 2, colliderLifeTime, attack01ColliderRadius, VGet(150,0,0),150, 0.3f);
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime + 2, colliderLifeTime, attack01ColliderRadius, VGet(-150,0,0),150,0.7f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime, colliderLifeTime, attack02ColliderRadius, 150, 0.5f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime - 2, colliderLifeTime, attack02ColliderRadius,  VGet(150, 0, 0) ,150 , 0.2f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime + 2, colliderLifeTime, attack02ColliderRadius, VGet(-150, 0, 0) ,150 , 0.3f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime02, colliderLifeTime, attack02ColliderRadius02, 150, 0.5f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime02 - 2, colliderLifeTime, attack02ColliderRadius02, VGet(150, 0, 0), 150, 0.2f);
	SetAnimEventForAttackCollider("attack02", attack02ColliderSpawnTime02 + 2, colliderLifeTime, attack02ColliderRadius02, VGet(-150, 0, 0), 150, 0.3f);
	SetAnimEventForAttackCollider("attack03", attack03ColliderSpawnTime, colliderLifeTime, attack03ColliderRadius, 250, 1.5f);
	SetAnimEventForAttackCollider("attack03", attack03ColliderSpawnTime, colliderLifeTime, attack03ColliderRadius02, 0);
	// UŒ‚‚ÌŒø‰Ê‰¹
	pAnimator->GetAnimation("attack01")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, attack01ColliderSpawnTime);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, attack02ColliderSpawnTime);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SwordSwing"); }, attack02ColliderSpawnTime02);
	pAnimator->GetAnimation("attack03")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("Impact"); }, attack03ColliderSpawnTime);
	pAnimator->GetAnimation("dead")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, deadAnimationTime);

	VECTOR pos = appearPos;
	SetAppearPos(VAdd(pos, VLeft));
	SetCirclePos(VAdd(pos, VRight));
}

void BossGoblin::Update() {
	BossBase::Update();
}

void BossGoblin::Render() {
	BossBase::Render();
}


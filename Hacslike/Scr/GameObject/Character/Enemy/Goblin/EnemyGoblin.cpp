#include "EnemyGoblin.h"

EnemyGoblin::EnemyGoblin() {
	Start();
}

EnemyGoblin::~EnemyGoblin() {}

void EnemyGoblin::Start() {
	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 150, 0), 30);
	SetScale(VGet(0.1f, 0.1f, 0.1f));
	MV1SetRotationXYZ(modelHandle, VGet(0, 180.0f * DX_PI_F / 180.0f, 0));
	type = Goblin;

	CollisionManager::GetInstance()->Register(pCollider);
	// アニメーションの設定
	pAnimator->SetModelHandle(modelHandle);
	pAnimator->Load("Res/Model/Enemy/Goblin/idle.mv1","idle01", true);
	pAnimator->Load("Res/Model/Enemy/Goblin/dead.mv1","dead");
	pAnimator->Load("Res/Model/Enemy/Goblin/walk.mv1","walk");
	pAnimator->GetAnimation("dead")->SetEvent([this]() { SetVisible(false); }, pAnimator->GetTotalTime("dead"));
	Enemy::Start();
}

void EnemyGoblin::Update() {
	Enemy::Update();
	if (!isVisible) return;
	if (pAnimator->GetCurrentAnimation() != 1)
		pAnimator->Play(0);

	Tracking();
}

void EnemyGoblin::Render() {
	Enemy::Render();
}

void EnemyGoblin::OnTriggerEnter(Collider* _pOther) {
	
}

#include "EnemyGoblin.h"

EnemyGoblin::EnemyGoblin() {
	Start();
}

EnemyGoblin::~EnemyGoblin() {}

void EnemyGoblin::Start() {
	// モデルの複製
	modelHandle = MV1DuplicateModel(EnemyManager::GetInstance().originGoblinMHandle);
	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 150, 0), 30);
	CollisionManager::GetInstance()->Register(pCollider);
	// アニメーションの設定
	pAnimator->SetModelHandle(modelHandle);
	pAnimator->Load("Res/Model/Enemy/Goblin/idle.mv1","idle01", true);
	pAnimator->Load("Res/Model/Enemy/Goblin/dead.mv1","dead");
	pAnimator->GetAnimation("dead")->SetEvent([this]() { SetVisible(false); }, pAnimator->GetTotalTime("dead"));
	Enemy::Start();
}

void EnemyGoblin::Update() {
	Enemy::Update();
	if (!isVisible) return;
	if (pAnimator->GetCurrentAnimation() != 1)
		pAnimator->Play(0);

	/*if (Vision_Fan()) {
		pAnimator->Play(1);
	}
	else {
		if (pAnimator->GetCurrentAnimation() == 1) return;
		pAnimator->Play(0);
	}*/
}

void EnemyGoblin::Render() {
	Enemy::Render();
}

void EnemyGoblin::OnTriggerEnter(Collider* _pOther) {
	if (_pOther->GetGameObject()->CompareTag("Player")) {
		hp = 0;
		IsDead();
	}
}

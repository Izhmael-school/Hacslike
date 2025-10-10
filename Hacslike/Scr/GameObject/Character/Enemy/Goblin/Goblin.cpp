#include "Goblin.h"

Goblin::Goblin() {
	Start();
}

Goblin::~Goblin() {}

void Goblin::Start() {
	modelHandle = MV1LoadModel("Res/Model/Goblin/goblin.mv1");
	SetModelHandle(modelHandle);
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 150, 0), 30);
	pAnimator->SetModelHandle(modelHandle);
	pAnimator->Load("Res/Model/Goblin/idle.mv1", true);
	pAnimator->Load("Res/Model/Goblin/attack01.mv1");
}

void Goblin::Update() {
	Enemy::Update();
	if (!isVisible) return;
	if (pAnimator->GetCurrentAnimation() != 1)
		pAnimator->Play(0);

	if (Vision_Fan()) {
		pAnimator->Play(1);
	}
	else {
		if (pAnimator->GetCurrentAnimation() == 1) return;
		pAnimator->Play(0);
	}
}

void Goblin::Render() {
	Enemy::Render();
}

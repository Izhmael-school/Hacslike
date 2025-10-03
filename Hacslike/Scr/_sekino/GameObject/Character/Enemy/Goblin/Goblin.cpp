#include "Goblin.h"
#include "../../../../Component/Collider.h"

Goblin::Goblin()
{
	Start();
}

Goblin::~Goblin() {}

void Goblin::Start() 
{
	modelHandle = MV1LoadModel("Res/Model/Goblin/goblin.mv1");
	SetModelHandle(modelHandle);
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 150, 0), 30);
	pAnimator->SetModelHandle(modelHandle);
	pAnimator->Load("Res/Model/Goblin/idle.mv1",true);
}

void Goblin::Update() {
	Enemy::Update();
	if (!isVisible) return;

	pAnimator->Play(0);

	Vision();
}

void Goblin::Render() {
	Enemy::Render();
}

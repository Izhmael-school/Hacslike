#include "BossBase.h"
#include "../../../../CommonModule.h"

BossBase::BossBase() 
	:appearPos()
{
	Start();
}

BossBase::~BossBase() {
}

void BossBase::AppearStair() {
	auto cells = StageManager::GetInstance().generator->cells;
	for (auto c : cells) {
		if (c->GetDataPos().x != appearPos.x || c->GetDataPos().z != appearPos.z) continue;

		StageManager::GetInstance().UnuseObject(c);
		StageCell* stair = StageManager::GetInstance().UseObject(Stair);
		StageManager::GetInstance().generator->useStair = stair;
		stair->SetPosition(VGet(appearPos.x * CellSize,0,appearPos.z * CellSize));
		stair->SetDataPos(VGet(appearPos.x,0,appearPos.z));
		StageManager::GetInstance().SetMapData(appearPos.x, appearPos.z, (int)Stair);
		break;
	}
}

void BossBase::Start() {
	Enemy::Start();
}

void BossBase::Update() {
	Enemy::Update();
}

void BossBase::Render() {
	Enemy::Render();
}

void BossBase::IsDead() {
	if (hp > 0) return;
	Enemy::IsDead();

	AppearStair();
}


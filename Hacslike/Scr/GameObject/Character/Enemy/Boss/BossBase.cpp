#include "BossBase.h"
#include "../../../../CommonModule.h"
#include"../../../../Manager/ArtifactManager.h"
#include "../../../../GameSystem/GameSystem.h"
#include"../../Player/Player.h"
#include "../../../../UI/BossSlainUI.h"

BossBase::BossBase()
	:appearPos() {
	vision.rayAngle = 360;
	vision.rayCount = 120;
}

BossBase::~BossBase() {
	delete hpBar;
	delete attackSpanBar;
}

void BossBase::AppearStair() {
	auto cells = StageManager::GetInstance().generator->cells;
	for (auto c : cells) {
		if (c->GetDataPos().x != appearPos.x || c->GetDataPos().z != appearPos.z) continue;

		StageManager::GetInstance().UnuseObject(c);
		StageCell* stair = StageManager::GetInstance().UseObject(Stair);
		StageManager::GetInstance().generator->useStair = stair;
		stair->SetPosition(VGet(appearPos.x * CellSize, 0, appearPos.z * CellSize));
		stair->SetDataPos(VGet(appearPos.x, 0, appearPos.z));
		StageManager::GetInstance().SetMapData(appearPos.x, appearPos.z, (int)Stair);
		break;
	}
}

void BossBase::Start() {
	Enemy::Start();
	SetModelHandleDup(MV1LoadModel(MergeString(mPath, "boss.mv1").c_str()));
	SetScale(VGet(0.2f, 0.2f, 0.2f));
	// アニメーションの設定
	pAnimator->SetModelHandle(modelHandle);
	// アニメーションのロード
	LoadAnimation();
	
	pAnimator->GetAnimation("dead")->SetEvent([this]() {EnemyManager::GetInstance().DeleteEnemy(this); }, pAnimator->GetTotalTime("dead"));
	hpBar = new Gauge(hp,maxHp, WINDOW_WIDTH / 4, 700.0f, WINDOW_WIDTH / 2, 15.0f);
	attackSpanBar = new Gauge(atkTime, atkSpan, WINDOW_WIDTH / 4, 715.0f, WINDOW_WIDTH / 2, 5.0f);

}

void BossBase::Update() {
	Enemy::Update();
	
	if(isDead) BossSlainUI::GetInstance()->Update();
	
}

void BossBase::Render() {
	Enemy::Render();
	
	if (!isDead) {
		hpBar->Render();
		DrawStringToHandle(WINDOW_WIDTH / 4, 700.0f - 20, name.c_str(), white,MainFont);
		
	}
	if(isDead) BossSlainUI::GetInstance()->Draw();
}

void BossBase::DeadExecute() {
	if (hp > 0) return;
	Enemy::DeadExecute();

	BossSlainUI::GetInstance()->Start();

	AppearStair();
	
	ArtifactManager::GetInstance().SetBossDesiegen(true);
}


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
	Start();
}

BossBase::~BossBase() {}

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
	// アニメーションイベントの設定
	pAnimator->GetAnimation("dead")->SetEvent([this]() {EnemyManager::GetInstance().DeleteEnemy(this); }, pAnimator->GetTotalTime("dead"));
}

void BossBase::Update() {
	Enemy::Update();
	
	if(isDead) BossSlainUI::GetInstance()->Update();

}

void BossBase::Render() {
	Enemy::Render();

	if(isDead) BossSlainUI::GetInstance()->Draw();
}

void BossBase::DeadExecute() {
	if (hp > 0) return;
	Enemy::DeadExecute();

	BossSlainUI::GetInstance()->Start();

	AppearStair();
	Player* pPlayer = Player::GetInstance();
	if (!isSelectArtifact) {
		
		artifactChioces = ArtifactManager::GetInstance().GenerateArtifactChoices();
		artifactSelectUI.StartSelection();
		isSelectArtifact = true;
		//GameSystem::GetInstance()->SetGameStatus(GameStatus::Stop);
		
	}
	else {
		int Selected = artifactSelectUI.UpdateSelection(artifactChioces);
		if (Selected != -1) {

			if (player && Selected >= 0 && Selected < (int)artifactChioces.size()) {
				ArtifactManager::GetInstance().ApplySelectedArtifact(pPlayer, artifactChioces[Selected]);

				
			}

			//GameSystem::GetInstance()->SetGameStatus(GameStatus::Playing);
		}
	}
	
}


#include "StageManager.h"
#include "FadeManager.h"

StageManager::StageManager() {
	generator = new StageGenerator();
}

StageManager::~StageManager() {
	delete generator;
	generator = nullptr;
	for (auto t : floorDifTexture) {
		DeleteGraph(t);
	}

	for (auto t : floorNormalTexture) {
		DeleteGraph(t);
	}
}

void StageManager::Update() {
	generator->Update();

#if _DEBUG
	if (InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN))
		GenerateStage();
#endif
}

void StageManager::Render() {
	generator->Render();
	DrawFormatString(100, 100, red, "階層 %d 階", floorCount - 1);
}

int StageManager::GetMapData(int x, int y) {
	return generator->map[x][y];
}

void StageManager::GenerateStage() {
	// 初期化
	generator->ClearStage();
	// 階層の加算
	floorCount++;
	//ChangeTexture(floorDifTexture[floor(floorCount / textureChangeFloor)], Room);
	generator->GenerateStageData();
	generator->GenerateStageObject();
	SetGameObjectRandomPos(Character::player);

	for (int i = 0; i < 10; i++) {
		EnemyManager::GetInstance().SpawnEnemy(Goblin, GetRandomRoomRandomPos());
	}
}

void StageManager::GenerateStage(int stageID) {
	// ステージの初期化
	generator->ClearStage();
	// フロアの加算
	floorCount++;
	// フロアデータの読み込み
	generator->LoadStageData(stageID);
	// フロアの生成
	generator->GenerateStageObject();
	// プレイヤーの配置
	generator->SetGameObject(Character::player, generator->GetStageData().playerSpawnPos);
	// ボスの配置
	VECTOR pos = generator->GetStageData().bossSpawnPos;
	EnemyManager::GetInstance().SpawnEnemy(Goblin, VGet(pos.x * CellSize, 0, pos.z * CellSize));
}

void StageManager::Generate() {
	FadeManager::GetInstance().FadeOut(0.5f);

	if (floorCount % BossFloorNum == 0) {
		GenerateStage((int)(floorCount / BossFloorNum));
	}
	else {
		GenerateStage();
	}

	FadeManager::GetInstance().FadeIn(0.5f);
}

void StageManager::SetGameObjectRandomPos(GameObject* obj) {
	generator->SetGameObjectRandomPos(obj);
}

void StageManager::ChangeTexture(int textureHandle, ObjectType changeObject) {
	generator->ChangeObjectTexture(textureHandle, changeObject);
}

int StageManager::GetNowRoomNum(VECTOR pos) {
	return generator->GetNowRoomNum(pos);
}

VECTOR StageManager::GetRandomRoomRandomPos() {
	return generator->GetRandomRoomRandomPos();
}




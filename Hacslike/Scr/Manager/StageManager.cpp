#include "StageManager.h"

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

	if (InputManager::GetInstance()->IsButtonDown(XINPUT_BUTTON_DPAD_DOWN))
		GenerateStage();
}

void StageManager::Render() {
	generator->Render();
	DrawFormatString(100, 100, red, "階層 %d 階", floorCount);
}

int StageManager::GetMapData(int x, int y) {
	return generator->map[x][y];
}

void StageManager::GenerateStage() {
	generator->ClearStage();

	floorCount++;
<<<<<<< HEAD

	//ChangeTexture(floorDifTexture[floor(floorCount / textureChangeFloor)], Room);
	generator->GenerateStageData();
	generator->GenerateStageObject();
	SetGameObjectRandomPos(Character::player);
=======
	
	//ChangeTexture(floorDifTexture[floor(floorCount / textureChangeFloor)], Room);

	generator->StageGenerate();
	SetObject(Character::player);
<<<<<<< HEAD
=======
	generator->GenerateStageData();
	generator->GenerateStageObject();
	SetGameObjectRandomPos(Character::player);
>>>>>>> Stashed changes
>>>>>>> Sekino
=======
>>>>>>> parent of 536f9b8 (Add)

	for (int i = 0; i < 10; i++) {

		EnemyManager::GetInstance().SpawnEnemy(Goblin, GetRandomRoomRandomPos());
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
<<<<<<< Updated upstream
void StageManager::SetObject(GameObject* obj) {
=======
>>>>>>> Sekino
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
<<<<<<< HEAD
	EnemyManager::GetInstance().SpawnEnemy(Goblin, VGet(pos.x * CellSize, 0, pos.z * CellSize));
}

void StageManager::SetGameObjectRandomPos(GameObject* obj) {
=======
	EnemyManager::GetInstance().SpawnEnemy(Goblin, VGet(pos.x * CellSize, 0, pos.z * CellSize),true);
}

void StageManager::SetGameObjectRandomPos(GameObject* obj) {
>>>>>>> Stashed changes
>>>>>>> Sekino
=======
void StageManager::SetObject(GameObject* obj) {
>>>>>>> parent of 536f9b8 (Add)
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



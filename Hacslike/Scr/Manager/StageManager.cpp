#include "StageManager.h"

StageManager* StageManager::pInstance = nullptr;

StageManager::StageManager() {
	generator = new StageGenerator();
}

StageManager::~StageManager() 
{
	delete generator;
	generator = nullptr;
	for (auto t : floorDifTexture) {
		DeleteGraph(t);
	}

	for (auto t : floorNormalTexture) {
		DeleteGraph(t);
	}
}

void StageManager::CreateInstance() {
	pInstance = new StageManager();
}

StageManager* StageManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

void StageManager::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

void StageManager::Update() {
	generator->Update();
}

void StageManager::Render() {
	generator->Render();
}

int StageManager::GetMapData(int x, int y) {
	return generator->map[x][y];
}

void StageManager::GenerateStage() {
	generator->ClearStage();

	floorCount++;
	
	//ChangeTexture(floorDifTexture[floor(floorCount / textureChangeFloor)], Room);
<<<<<<< Updated upstream

	generator->StageGenerate();
	SetObject(Character::player);
=======
	generator->GenerateStageData();
	generator->GenerateStageObject();
	SetGameObjectRandomPos(Character::player);
>>>>>>> Stashed changes

	for (int i = 0; i < 10; i++) {

	EnemyManager::GetInstance().SpawnEnemy(Goblin, GetRandomRoomRandomPos());
	}
}

<<<<<<< Updated upstream
void StageManager::SetObject(GameObject* obj) {
=======
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
	EnemyManager::GetInstance().SpawnEnemy(Goblin, VGet(pos.x * CellSize, 0, pos.z * CellSize),true);
}

void StageManager::SetGameObjectRandomPos(GameObject* obj) {
>>>>>>> Stashed changes
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

void StageManager::AppearHiddenStair() {
	generator->AppearHiddenStair();
}



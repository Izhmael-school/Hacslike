#include "StageManager.h"
#include "FadeManager.h"
#include "../GameObject/Character/Character.h"
#include "../CommonModule.h"
#include "AudioManager.h"
#include"../Save/SaveIO.h"
#include "../GameObject/SaveObject/SaveObject.h"
#include"../GameObject/TreasureChest/StartTreasureChest.h"
#include "../GameObject/Enhancement/EnhancementStone.h"

StageManager::StageManager() {
	generator = new StageGenerator();
	AudioManager::GetInstance().Load("Res/Audio/SE/Stage/FloorDawn.mp3","FloorDawn",false);

	LoadFloorTexture();
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

	if(pSaveObject){
		pSaveObject->Update();
	}
	if (pChest) {
		pChest->Update();
	}
	if(pStone){
		pStone->Update();
	}
#if _DEBUG
	if (InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN) || InputManager::GetInstance().IsKeyDown(KEY_INPUT_DOWN)) {
		LoadFloorData();

		if (floorCount % BossFloorNum == 0) {
			GenerateStage((int)(floorCount / BossFloorNum));
		}
		else {
			GenerateStage();
		}
	}
#endif
}

void StageManager::Render() {
	generator->Render();

	if(pSaveObject){
		pSaveObject->Render();
	}
	if (pChest) {
		pChest->Render();
	}
	if (pStone) {
		pStone->Render();
	}
	DrawFormatString(100, 100, red, "階層 %d 階", floorCount - 1);
}

void StageManager::LoadFloorData() {
	auto data = LoadJsonFile("Scr/Data/FloorData.json");

	for (auto d : data) {
		if (d["startFloor"] - 1 != floorCount) continue;

		floorData.startFloor = d["startFloor"];
		floorData.endFloor = d["endFloor"];

		// ベクターの初期化
		floorData.spawnEnemyID.clear();
		floorData.spawnEnemyID.shrink_to_fit();

		for (int id : d["spawnEnemyID"]) {
			floorData.spawnEnemyID.push_back(id);
		}
		break;
	}
}

void StageManager::LoadFloorTexture() {
	auto data = LoadJsonFile("Scr/data/FloorData.json");

	std::string floorTag = "f";
	std::string wallTag = "w";
	std::string normalTag = "n";
	std::string pngTag = ".png";

	for (auto d : data) {
		std::string floorDiv = MergeString(TEXTURE_FILEPATH, d["floorTextureName"], floorTag, pngTag);
		std::string floorNml = MergeString(TEXTURE_FILEPATH, d["floorTextureName"], floorTag,normalTag, pngTag);
		std::string wallDiv = MergeString(TEXTURE_FILEPATH, d["floorTextureName"], wallTag, pngTag);
		std::string wallNml = MergeString(TEXTURE_FILEPATH, d["floorTextureName"], wallTag,normalTag, pngTag);
	
		floorDifTexture.push_back(LoadGraph(floorDiv.c_str()));
		floorNormalTexture.push_back(LoadGraph(floorNml.c_str()));
		wallDifTexture.push_back(LoadGraph(wallDiv.c_str()));
		wallNormalTexture.push_back(LoadGraph(wallNml.c_str()));
	}
}

int StageManager::GetMapData(int x, int y) {

	if (x < 0 || y < 0 || x > mapWidth_Large || y > mapHeight_Large) return -1;

	return generator->map[x][y];
}

int StageManager::SetMapData(int x, int y, int setValue) {
	return generator->map[x][y] = setValue;
}

int StageManager::GetRoomStatus(int roomNum, RoomStatus status) {
	return generator->roomStatus[status][roomNum];
}

void StageManager::GenerateStage() {
	// 初期化
	generator->ClearStage();
	// エネミーの削除
	EnemyManager::GetInstance().UnuseAllEnemy();
	// 階層の加算
	floorCount++;
	// ステージのデータを作る
	generator->GenerateStageData();
	// ステージのオブジェクトを置く
	generator->GenerateStageObject();
	int texNum = std::floor((floorCount - 1 + 10) / textureChangeFloor);
	// テクスチャの張替え
	ChangeTexture(texNum, Room);
	// プレイヤーの設置
	SetGameObjectRandomPos(Character::player);
	int canSpawnNum = 0;
	for (int i = 0; i < RoomMax_Large; i++) {
		int w = generator->roomStatus[rw][i];
		int h = generator->roomStatus[rh][i];

		canSpawnNum += w * h;
	}
	// 敵の数を決める
	int spawnNum = Random(std::floor(canSpawnNum / 10), std::floor(canSpawnNum / 3));

	if (spawnNum > EnemyMax) spawnNum = EnemyMax;

	// 敵データを読み込む
	auto data = LoadJsonFile("Scr/Data/EnemyData.json");

	int max = floorData.spawnEnemyID.size();
	std::vector<EnemyData> spawnEnemyDataList;

	// スポーンする可能性のある敵のデータを取得しておく
	for (int i = 0; i < max; i++) {
		for (auto d : data) {
			if (d["id"] != floorData.spawnEnemyID[i]) continue;

			EnemyData eData;
			eData.id = d["id"];
			eData.typeID = d["typeID"];

			spawnEnemyDataList.push_back(eData);
			break;
		}
	}

	for (int i = 0; i < spawnNum; i++) {
		int spawnEnemyID = spawnEnemyDataList[Random(0, spawnEnemyDataList.size() - 1)].id;

		for (auto e : spawnEnemyDataList) {
			if (e.id != spawnEnemyID) continue;

			EnemyManager::GetInstance().SpawnEnemy((EnemyType)e.typeID, GetRandomRoomRandomPos());
			break;
		}

	}
}

void StageManager::GenerateStage(int stageID) {
	// ステージの初期化
	generator->ClearStage();
	// エネミーの削除
	EnemyManager::GetInstance().UnuseAllEnemy();
	// フロアの加算
	floorCount++;
	// フロアデータの読み込み
	generator->LoadStageData(stageID);
	// フロアの生成
	generator->GenerateStageObject();
	// テクスチャの張替え
	ChangeTexture(std::floor((floorCount - 1) / textureChangeFloor), Room);
	// プレイヤーの配置
	generator->SetGameObject(Character::player, generator->GetStageData().playerSpawnPos);
	// JSON に書かれた saveObjectPos があれば SaveObject を生成して配置する
	{
		StageData sd = generator->GetStageData();
		// SaveObject のシングルトンを取得（未生成なら生成される）
		SaveObject* so = SaveObject::GetInstance();
		if (so) {
			// 位置は StageGenerator 側のセル座標（generator->SetGameObject がスケール変換してくれる）
			generator->SetGameObject(so, sd.saveObjectPos);
			pSaveObject = so;
		}
	}
	{
		StageData sd = generator->GetStageData();
		StartTreasureChest* chest = StartTreasureChest::GetInstance();
		if(chest){
			generator->SetGameObject(chest, sd.chestObjectPos);
			pChest = chest;
		}
	}
	{
		StageData sd = generator->GetStageData();
		EnhancementStone* stone = EnhancementStone::GetInstance();
		if(stone){
			generator->SetGameObject(stone, sd.enhancementStonePos);
			pStone = stone;
		}
	}
	// ボスの配置
	VECTOR pos = generator->GetStageData().bossSpawnPos;

	int enemyType = generator->GetStageData().bossType;
	if (enemyType == -1) return;

	EnemyManager::GetInstance().SpawnBoss((EnemyType)enemyType, VGet(pos.x, 0, pos.z));
}

void StageManager::Generate() {

	AudioManager::GetInstance().Stop("all");

	FadeManager::GetInstance().FadeOut(0.5f);

	LoadFloorData();

	if (floorCount % BossFloorNum == 0) {
		GenerateStage((int)(floorCount / BossFloorNum));
	}
	else {
		GenerateStage();
	}

	AudioManager::GetInstance().PlayBGM("NormalFloor", 0.3f);

	FadeManager::GetInstance().FadeIn(0.5f);

}

void StageManager::NoFadeGenerate() {
	AudioManager::GetInstance().Stop("all");

	LoadFloorData();

	if (floorCount % BossFloorNum == 0) {
		GenerateStage((int)(floorCount / BossFloorNum));
	}
	else {
		GenerateStage();
	}

	AudioManager::GetInstance().PlayBGM("NormalFloor");
	AudioManager::GetInstance().ChangeVolume(0.3f, "NormalFloor");
}

void StageManager::UnuseObject(StageCell* cell) {
	generator->UnuseObject(cell);
}

StageCell* StageManager::UseObject(ObjectType type) {
	return generator->UseObject(type);
}

void StageManager::SetGameObjectRandomPos(GameObject* obj) {
	generator->SetGameObjectRandomPos(obj);
}

void StageManager::SetGameObject(VECTOR pos, GameObject* obj) {
	generator->SetGameObject(obj, pos);
}

void StageManager::ChangeTexture(int num, ObjectType changeObject) {
	generator->ChangeObjectTexture(num, changeObject);
}

void StageManager::SaveTo(BinaryWriter& w)
{
	// 階層情報
	w.WritePOD(floorCount);

	// ステージ状態を委譲して保存
	if (generator) {
		generator->SaveTo(w);
	}
}

void StageManager::LoadFrom(BinaryReader& r, uint32_t saveVersion)
{
	// 階層情報
	r.ReadPOD(floorCount);

	// sanity check
	if (floorCount < 0 || floorCount > 10000) {
		printf("[Save] suspicious floorCount read: %d -> clamped to 0\n", floorCount);
		floorCount = 0;
	}
	printf("[Save] floorCount read: %d\n", floorCount);

	if (generator) {
		generator->LoadFrom(r, saveVersion);

		printf("[Save] after LoadFrom: mapWidth=%d mapHeight=%d roomCount=%d\n",
			generator->mapWidth, generator->mapHeight, generator->roomCount);

		// まず通常どおりオブジェクト生成
		GenerateStage((int)(floorCount / BossFloorNum));

		size_t created = 0;
		for (auto c : generator->cells) {
			if (c) { c->Update(); ++created; }
		}
		if (generator->useStair) { generator->useStair->Update(); ++created; }

		printf("[Save] GenerateStageObject created %zu StageCells, useStair=%p\n", created, (void*)generator->useStair);

		// もし何も生成されなかったら、読み込まれたデータが不完全な可能性が高いので回復処理を試みる
		if (created == 0) {
			printf("[Save] No StageCells created - data may be incomplete. Forcing fallback reconstruction.\n");

			// フォールバック: マップを全領域として扱う
			generator->mapWidth = mapWidth_Large;
			generator->mapHeight = mapHeight_Large;

			created = 0;
			for (auto c : generator->cells) {
				if (c) { c->Update(); ++created; }
			}
			if (generator->useStair) { generator->useStair->Update(); ++created; }

			printf("[Save] After fallback, created %zu StageCells, useStair=%p\n", created, (void*)generator->useStair);
		}
	}
	
}


int StageManager::GetNowRoomNum(VECTOR pos) {
	return generator->GetNowRoomNum(pos);
}

VECTOR StageManager::GetRandomRoomRandomPos() {
	return generator->GetRandomRoomRandomPos();
}




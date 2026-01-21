#pragma once
#include "../GameObject/Stage/StageCell.h"
#include "../GameObject/Stage/StageGenerator.h"
#include "../Definition.h"
#include"SaveManager.h"

#include "EnemyManager.h"
#include <vector>

// forward
class SaveObject;
class StartTreasureChest;

struct FloorData {
	int startFloor;
	int endFloor;
	std::vector<int> spawnEnemyID;
};

struct EnemyData {
	int id;
	int typeID;
};

class StageManager :public Singleton<StageManager>{
#pragma region シングルトンのデータ構造
public:	// コンストラクタとデストラクタ
	/*
	 * @brief	コンストラクタ
	 * @tip		外部で生成されないようにアクセス指定子をprivateにする
	 */
	StageManager();

	/*
	 * @brief	デストラクタ
	 */
	~StageManager();

public:
	class StageGenerator* generator;
	int floorCount = 0;
	std::vector<int> floorDifTexture;
	std::vector<int> wallDifTexture;
	std::vector<int> floorNormalTexture;
	std::vector<int> wallNormalTexture;
	int textureChangeFloor = 10;
	const int BossFloorNum = 10;
	FloorData floorData;

	const std::string TEXTURE_FILEPATH = "Res/Model/Stage/Texture/";

	// SaveObject を StageManager が所有して Update/Render を呼ぶ
	SaveObject* pSaveObject = nullptr;
	StartTreasureChest* pChest = nullptr;
public:
	void Update();
	void Render();
	void LoadFloorData();
	void LoadFloorTexture();

	int GetMapData(int x,int y);
	int SetMapData(int x,int y,int setValue);
	int GetRoomStatus(int roomNum,RoomStatus status);
	void SetGameObjectRandomPos(GameObject* obj);
	void SetGameObject(VECTOR pos,GameObject* obj);
	int GetNowRoomNum(VECTOR pos);
	VECTOR GetRandomRoomRandomPos();
	void Generate();
	void NoFadeGenerate();

	void UnuseObject(StageCell* cell);
	StageCell* UseObject(ObjectType type);

	inline void ResetFloorCount() { floorCount = 0; }

private:
	void GenerateStage();
	void GenerateStage(int stageID);
	void ChangeTexture(int num, ObjectType changeObject);

public:
	// セーブ/ロード用の API（SaveManager 経由で呼ばれる）
	void SaveTo(BinaryWriter& w);
	void LoadFrom(BinaryReader& r, uint32_t saveVersion);
};


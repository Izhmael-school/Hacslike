#pragma once
#include "../GameObject/Stage/StageCell.h"
#include "../GameObject/Stage/StageGenerator.h"
#include "../Definition.h"

#include "EnemyManager.h"
#include <vector>

struct FloorData {
	int startFloor;
	int endFloor;
	std::string floorTextureName;
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
	std::vector<int> floorNormalTexture;
	int textureChangeFloor = 10;
	const int BossFloorNum = 10;
	FloorData floorData;

public:
	void Update();
	void Render();
	void LoadFloorData();

	int GetMapData(int x,int y);
	int SetMapData(int x,int y,int setValue);
	int GetRoomStatus(int roomNum,RoomStatus status);
	void SetGameObjectRandomPos(GameObject* obj);
	int GetNowRoomNum(VECTOR pos);
	VECTOR GetRandomRoomRandomPos();
	void Generate();

	void UnuseObject(StageCell* cell);
	StageCell* UseObject(ObjectType type);

private:
	void GenerateStage();
	void GenerateStage(int stageID);
	void ChangeTexture(int textureHandle, ObjectType changeObject);

};


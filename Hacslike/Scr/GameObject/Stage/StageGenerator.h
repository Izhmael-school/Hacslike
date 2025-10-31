#pragma once
#include "StageCell.h"
#include <random>
#include <list>

class StageCell;

<<<<<<< Updated upstream
=======
struct StageData {
	int id;
	int stageData[mapWidth_Large][mapHeight_Large];
	VECTOR playerSpawnPos;
	VECTOR bossSpawnPos;
	VECTOR stairSpawnPos;
};

>>>>>>> Stashed changes
class StageGenerator {
private:
	int groundModel = -1;
	int wallModel = -1;
	int roadModel = -1;
	int stairModel = -1;

	VECTOR defaultPos = VGet(0, 0, 0);


	int roomMinNum = 3;	// 部屋の最小数
	int roomNum;	// 部屋の数
	int parentNum;	// 分割する部屋番号
	int maxArea;		// 最大面積
	int roomCount;	// 部屋カウント
	int line;		// 分割点
	int mapSize ;
	VECTOR mapOffset;

public:
	enum RoomStatus {	// 部屋の配列ステータス
		x,	// マップX座標
		y,	// マップY座標
		w,	// 分割した幅
		h,	// 分割した高さ

		rx,	// 部屋の生成位置
		ry,	// 部屋の生成位置
		rw,	// 部屋の幅
		rh,	// 部屋の高さ
		Max,
	};

	int map[mapWidth_Large][mapHeight_Large];	// マップ管理配列
	bool mapObjects[mapWidth_Large][mapHeight_Large];	// マップ上のオブジェクトの配置
	int roomStatus[RoomStatus::Max][RoomMax_Large];	// 部屋の配列ステータス
	bool stageMap[mapWidth_Large][mapHeight_Large];

	std::list<StageCell*> cells ;
	std::list<StageCell*> unuseWall;
	std::list<StageCell*> unuseRoad;
	std::list<StageCell*> unuseRoom;
	StageCell* unuseStair;
	StageCell* useStair;
	int gr;

	StageCell* test;

public:
	StageGenerator();
	~StageGenerator();

	void Update();
	void Render();

	void ClearStage();
	void StageGenerate();
	int Random(int min, int max);
	bool SplitPoint(int x, int y);
	bool CheckEightDir(int x, int y);
	void SetGameObjectRandomPos(GameObject* obj);

	StageCell* UseObject(ObjectType type);
	void UnuseObject(StageCell*& cell);

	void DrawMap();

	void ChangeObjectTexture(int textureHandle,ObjectType changeObject);
	int GetNowRoomNum(VECTOR pos);

	VECTOR GetRandomRoomRandomPos();
<<<<<<< Updated upstream
=======
	// 読み込んだステージデータの取得
	inline StageData GetStageData() { return stage; }
	// 隠し階段の出現
	void AppearHiddenStair();
>>>>>>> Stashed changes
};


#pragma once
#include "StageCell.h"
#include <random>
#include <list>
#include "../../CommonModule.h"
#include"../../Manager/SaveManager.h"


class StageCell;

struct StageData {
	int id;
	int stageData[mapWidth_Large][mapHeight_Large];
	VECTOR playerSpawnPos;
	VECTOR saveObjectPos;
	VECTOR chestObjectPos;
	VECTOR bossSpawnPos;
	int bossType;
};

class StageGenerator {
public:
	int groundModel = -1;
	int wallModel = -1;
	int roadModel = -1;
	int stairModel = -1;

	VECTOR defaultPos = VGet(0, 0, 0);

	int mapWidth = 0;
	int mapHeight = 0;
	int roomMinNum = 3;	// 部屋の最小数
	int roomNum;	// 部屋の数
	int parentNum;	// 分割する部屋番号
	int maxArea;		// 最大面積
	int roomCount;	// 部屋カウント
	int line;		// 分割点
	int mapSize ;
	VECTOR mapOffset;

	StageData stage;

public:
	
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

public:
	StageGenerator();
	~StageGenerator();

	void Update();
	void Render();

	// ステージの初期化
	void ClearStage();
	// ステージデータのランダム生成
	void GenerateStageData();
	// ステージデータの読み込み生成
	void LoadStageData(int stageID);
	// ステージのオブジェクト生成
	void GenerateStageObject();
	// 分割点2点のうち大きいほうを分割する
	bool SplitPoint(int x, int y);
	// 八方向チェック
	bool CheckEightDir(int x, int y);
	// オブジェクトのランダム設置
	void SetGameObjectRandomPos(GameObject* obj);
	// オブジェクトの設置
	void SetGameObject(GameObject* _obj, VECTOR _pos);

	// ステージオブジェクトのプーリング
	StageCell* UseObject(ObjectType type);
	void UnuseObject(StageCell*& cell);

	// マップの描画
	void DrawMap();
	// 壁の透過をするかどうか
	bool TransparencyWall(StageCell* cell);

	// ステージオブジェクトのテクスチャ張替
	void ChangeObjectTexture(int num,ObjectType changeObject);
	// 座標から今いる部屋番号を返す
	int GetNowRoomNum(VECTOR pos);
	int GetNowRoomNum(int x,int z);
	// ランダムな部屋のランダムな座標を返す
	VECTOR GetRandomRoomRandomPos();
	// 読み込んだステージデータの取得
	inline StageData GetStageData() { return stage; }
	
	// シリアライズ / デシリアライズ
	void SaveTo(BinaryWriter& w);
	void LoadFrom(BinaryReader& r, uint32_t ver);
};


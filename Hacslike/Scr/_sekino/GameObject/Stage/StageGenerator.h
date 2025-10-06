#pragma once
#include <vector>
#include "../GameObject.h"
#include <random>

class StageGenerator {
private:
#define mapWidth (20)	// マップの横サイズ
#define mapHeight (20)	// マップの縦サイズ
#define offsetWall (2)	// 壁から離す距離 
#define offset (1)		// 調節用

#define RoomMax (10)

private:
	int map[mapWidth][mapHeight];	// マップ管理配列

	int roomMinNum;	// 部屋の最小数
	int roomRand;	// 部屋の加算数
	int roomNum;	// 部屋の数
	int parentNum;	// 分割する部屋番号
	int max;		// 最大面積
	int roomCount = 0;	// 部屋カウント
	int line;		// 分割点

public:
	enum RoomStatus{	// 部屋の配列ステータス
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

	int roomStatus[RoomStatus::Max][RoomMax];	// 部屋の配列ステータス

	int iti[2];

	std::vector<GameObject> mapObjects;	// マップオブジェクト管理配列

	int tileModelHandle = -1;

public:
	StageGenerator();
	~StageGenerator();

	void ClearStage();
	void StageGenerate();
	bool SplitPoint(int x,int y);
};


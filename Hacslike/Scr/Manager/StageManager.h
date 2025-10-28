#pragma once
#include "../GameObject/Stage/StageCell.h"
#include "../GameObject/Stage/StageGenerator.h"
#include "../Definition.h"
#include "../GameObject/Character/Character.h"
#include "EnemyManager.h"
#include <vector>

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

	void Update();
	void Render();

	int GetMapData(int x,int y);
	void GenerateStage();
	void GenerateStage(int stageID);

	void SetGameObjectRandomPos(GameObject* obj);
	void ChangeTexture(int textureHandle, ObjectType changeObject);
	int GetNowRoomNum(VECTOR pos);

	VECTOR GetRandomRoomRandomPos();
};


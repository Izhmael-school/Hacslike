#include "StageGenerator.h"
#include "../Character/Character.h"
#include <climits>

StageGenerator::StageGenerator()
	:roomMinNum(3)
	, roomNum(0)
	, parentNum(0)
	, maxArea(0)
	, roomCount(0)
	, line(0)
	, mapSize(4)
	, mapOffset(VGet(950, 0, 800))
	, stage() {
	wallModel = MV1LoadModel("Res/Model/Stage/Wall.mv1");
	groundModel = MV1LoadModel("Res/Model/Stage/Room.mv1");
	roadModel = MV1LoadModel("Res/Model/Stage/Room.mv1");
	stairModel = MV1LoadModel("Res/Model/Stage/Stair.mv1");

	gr = LoadGraph("Res/Model/Stage/Texture/Wood_A.png");
	int tex = MV1GetMaterialDifMapTexture(groundModel, 0);
	int i = MV1SetTextureGraphHandle(groundModel, tex, gr, false);

	unuseStair = nullptr;
	useStair = nullptr;

	for (int w = 0; w < mapWidth_Large; w++) {
		for (int h = 0; h < mapHeight_Large; h++) {
			// 壁にする
			map[w][h] = Wall;
		}
	}

	for (int w = 0; w < mapWidth_Large; w++) {
		for (int h = 0; h < mapHeight_Large; h++) {
			mapObjects[w][h] = false;
		}
	}

	for (int w = 0; w < mapWidth_Large; w++) {
		for (int h = 0; h < mapHeight_Large; h++) {
			stageMap[w][h] = false;
		}
	}

	for (int w = 0; w < RoomStatus::Max; w++) {
		for (int h = 0; h < RoomMax_Large; h++) {
			roomStatus[w][h] = -1;
		}
	}
}

StageGenerator::~StageGenerator() {
	delete[] map;
	delete[] roomStatus;
	delete[] mapObjects;
	delete[] stageMap;

	delete unuseStair;
	delete useStair;

	DeleteGraph(gr);

	for (auto c : cells) {
		MV1DeleteModel(c->GetModelHandle());
		delete c;
	}

	for (auto c : unuseRoad) {
		MV1DeleteModel(c->GetModelHandle());
		delete c;
	}

	for (auto c : unuseRoom) {
		MV1DeleteModel(c->GetModelHandle());
		delete c;
	}

	for (auto c : unuseWall) {
		MV1DeleteModel(c->GetModelHandle());
		delete c;
	}

	MV1DeleteModel(wallModel);
	MV1DeleteModel(groundModel);
	MV1DeleteModel(roadModel);
	MV1DeleteModel(stairModel);
}

void StageGenerator::Update() {
	for (auto c : cells) {
		c->Update();
	}

	// 階層変化時階段オブジェクトで例外スローが起こるため、オブジェクトを分けておく
	if (useStair != nullptr)
		useStair->Update();

}

void StageGenerator::Render() {

	for (auto c : cells) {

		// 壁の透過処理
		bool fact = TransparencyWall(c);

		if (fact) continue;

		c->Render();
	}

	if (useStair != nullptr)
		useStair->Render();

	DrawMap();
}

void StageGenerator::ClearStage() {
	for (auto c : cells) {
		UnuseObject(c);
	}

	cells.clear();

	UnuseObject(useStair);

	// 初期化
	for (int w = 0; w < mapWidth_Large; w++) {
		for (int h = 0; h < mapHeight_Large; h++) {
			// 壁にする
			map[w][h] = Wall;
		}
	}

	// 初期化
	for (int w = 0; w < mapWidth_Large; w++) {
		for (int h = 0; h < mapHeight_Large; h++) {
			mapObjects[w][h] = false;
		}
	}

	for (int w = 0; w < mapWidth_Large; w++) {
		for (int h = 0; h < mapHeight_Large; h++) {
			stageMap[w][h] = false;
		}
	}

	for (int w = 0; w < RoomStatus::Max; w++) {
		for (int h = 0; h < RoomMax_Large; h++) {
			roomStatus[w][h] = -1;
		}
	}

	EnemyManager::GetInstance().UnuseAllEnemy();

	roomNum = 0;
	parentNum = 0;
	maxArea = 0;
	roomCount = 0;
	line = 0;
}

void StageGenerator::GenerateStageObject() {

	// オブジェクトを生成する
	for (int nowH = 0; nowH < mapHeight; nowH++) {
		for (int nowW = 0; nowW < mapWidth; nowW++) {

			if (!CheckEightDir(nowW, nowH)) continue;

			// セルの生成
			StageCell* c = UseObject((ObjectType)map[nowW][nowH]);
			c->SetPosition(VGet(defaultPos.x + nowW * CellSize, 0, defaultPos.z + nowH * CellSize));
			c->SetDataPos(VGet(nowW, 0, nowH));
			if (c->GetObjectType() == Stair) {
				useStair = c;
			}
			else {
				cells.push_back(c);
			}
		}
	}
}

/// <summary>
/// 分割点2点のうち大きいほうを分割する
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
bool StageGenerator::SplitPoint(int _x, int _y) {
	// 最小分割可能サイズチェック（十分な余裕が無ければ分割不可）
	int minNeeded = roomMinNum * 2 + (offsetWall * 4);
	if (_x < minNeeded && _y < minNeeded) {
		return false;
	}

	if (_x > _y) {
		int min = roomMinNum + (offsetWall * 2);
		int max = _x - (offsetWall * 2 + roomMinNum);
		if (max <= min) return false; // 範囲不備 -> 分割不能
		line = Random(min, max);
		return true;
	}
	else {
		int min = roomMinNum + (offsetWall * 2);
		int max = _y - (offsetWall * 2 + roomMinNum);
		if (max <= min) return false; // 範囲不備 -> 分割不能
		line = Random(min, max);
		return false;
	}
}

void StageGenerator::SetGameObjectRandomPos(GameObject* obj) {
	VECTOR pos;
	while (1) {

		int rand = GetRand(roomNum - 1);
		int x = Random(roomStatus[RoomStatus::rx][rand], roomStatus[RoomStatus::rx][rand] + roomStatus[RoomStatus::rw][rand] - 1);
		int y = Random(roomStatus[RoomStatus::ry][rand], roomStatus[RoomStatus::ry][rand] + roomStatus[RoomStatus::rh][rand] - 1);

		if (mapObjects[x][y]) continue;

		mapObjects[x][y] = true;

		pos = VGet(x * CellSize, 0, y * CellSize);
		if (map[x][y] == 0) break;
	}

	obj->SetPosition(pos);
}

void StageGenerator::SetGameObject(GameObject* _obj, VECTOR _pos) {
	VECTOR pos = VGet(_pos.x * CellSize, 0, _pos.z * CellSize);

	_obj->SetPosition(pos);
}

void StageGenerator::GenerateStageData() {
	int roomMax = 0;

	// フロアの大きさを決める
	switch (GetRand(2)) {
	case 0:
		mapWidth = mapWidth_Small;
		mapHeight = mapHeight_Small;
		roomMax = RoomMax_Small;
		break;
	case 1:
		mapWidth = mapWidth_Middle;
		mapHeight = mapHeight_Middle;
		roomMax = RoomMax_Middle;
		break;
	case 2:
		mapWidth = mapWidth_Large;
		mapHeight = mapHeight_Large;
		roomMax = RoomMax_Large;
		break;
	}

	// 部屋数をランダムに決める
	roomNum = Random(roomMinNum, roomMax);

	// 部屋を入れる
	roomStatus[(int)RoomStatus::x][roomCount] = 0;
	roomStatus[(int)RoomStatus::y][roomCount] = 0;
	roomStatus[(int)RoomStatus::w][roomCount] = mapWidth;
	roomStatus[(int)RoomStatus::h][roomCount] = mapHeight;

	// カウント追加
	roomCount++;

	// 部屋の数だけ分割する
	for (int splitNum = 0; splitNum < roomNum - 1; splitNum++) {
		// 初期化
		parentNum = 0;
		maxArea = 0;

		// 最大の部屋番号を調べる
		for (int maxCheck = 0; maxCheck < roomNum; maxCheck++) {
			int area = roomStatus[(int)RoomStatus::w][maxCheck] * roomStatus[(int)RoomStatus::h][maxCheck];
			// 面接比較
			if (maxArea < area) {
				// 最大面積上書き
				maxArea = area;

				// 親の部屋番号セット
				parentNum = maxCheck;
			}
		}

		// 取得した部屋をさらに割る
		if (SplitPoint(roomStatus[(int)RoomStatus::w][parentNum], roomStatus[(int)RoomStatus::h][parentNum])) {
			// 取得
			roomStatus[(int)RoomStatus::x][roomCount] = roomStatus[(int)RoomStatus::x][parentNum];
			roomStatus[(int)RoomStatus::y][roomCount] = roomStatus[(int)RoomStatus::y][parentNum];
			roomStatus[(int)RoomStatus::w][roomCount] = roomStatus[(int)RoomStatus::w][parentNum] - line;
			roomStatus[(int)RoomStatus::h][roomCount] = roomStatus[(int)RoomStatus::h][parentNum];

			// 親の部屋を整形する
			roomStatus[(int)RoomStatus::x][parentNum] += roomStatus[(int)RoomStatus::w][roomCount];
			roomStatus[(int)RoomStatus::w][parentNum] -= roomStatus[(int)RoomStatus::w][roomCount];
		}
		else {
			// 取得
			roomStatus[(int)RoomStatus::x][roomCount] = roomStatus[(int)RoomStatus::x][parentNum];
			roomStatus[(int)RoomStatus::y][roomCount] = roomStatus[(int)RoomStatus::y][parentNum];
			roomStatus[(int)RoomStatus::w][roomCount] = roomStatus[(int)RoomStatus::w][parentNum];
			roomStatus[(int)RoomStatus::h][roomCount] = roomStatus[(int)RoomStatus::h][parentNum] - line;

			// 親の部屋を整形する
			roomStatus[(int)RoomStatus::y][parentNum] += roomStatus[(int)RoomStatus::h][roomCount];
			roomStatus[(int)RoomStatus::h][parentNum] -= roomStatus[(int)RoomStatus::h][roomCount];
		}

		// カウントを加算
		roomCount++;
	}

	// 分割した中でランダムな大きさの部屋を生成
	for (int i = 0; i < roomNum; i++) {
		// 生成座標の設定
		roomStatus[(int)RoomStatus::rx][i] = Random(roomStatus[(int)RoomStatus::x][i] + offsetWall, (roomStatus[(int)RoomStatus::x][i] + roomStatus[(int)RoomStatus::w][i]) - (roomMinNum + offsetWall));
		roomStatus[(int)RoomStatus::ry][i] = Random(roomStatus[(int)RoomStatus::y][i] + offsetWall, (roomStatus[(int)RoomStatus::y][i] + roomStatus[(int)RoomStatus::h][i]) - (roomMinNum + offsetWall));

		// 部屋の大きさを設定
		roomStatus[(int)RoomStatus::rw][i] = Random(roomMinNum, roomStatus[(int)RoomStatus::w][i] - (roomStatus[(int)RoomStatus::rx][i] - roomStatus[(int)RoomStatus::x][i]) - offsetS);
		roomStatus[(int)RoomStatus::rh][i] = Random(roomMinNum, roomStatus[(int)RoomStatus::h][i] - (roomStatus[(int)RoomStatus::ry][i] - roomStatus[(int)RoomStatus::y][i]) - offsetS);
	}

	// マップ上書き
	for (int c = 0; c < roomNum; c++) {
		// 取得した部屋の確認
		for (int h = 0, maxH = roomStatus[(int)RoomStatus::h][c]; h < maxH; h++) {
			for (int w = 0, maxW = roomStatus[(int)RoomStatus::w][c]; w < maxW; w++) {
				// 部屋チェックポイント
				map[w + roomStatus[(int)RoomStatus::x][c]][h + roomStatus[(int)RoomStatus::y][c]] = 1;
			}
		}

		// 生成した部屋
		for (int h = 0, maxH = roomStatus[(int)RoomStatus::rh][c]; h < maxH; h++) {
			for (int w = 0, maxW = roomStatus[(int)RoomStatus::rw][c]; w < maxW; w++) {
				map[w + roomStatus[(int)RoomStatus::rx][c]][h + roomStatus[(int)RoomStatus::ry][c]] = 0;
			}
		}
	}

	// 道の生成
	int splitLength[4];
	int rootPoint = 0;

	// 部屋から一番近い境界線を調べる
	for (int nowRoom = 0; nowRoom < roomNum; nowRoom++) {
		// 左の壁からの距離
		splitLength[0] = roomStatus[(int)RoomStatus::x][nowRoom] > 0 ?
			roomStatus[(int)RoomStatus::rx][nowRoom] - roomStatus[(int)RoomStatus::x][nowRoom] :
			INT_MAX;

		// 右の壁からの距離
		splitLength[1] = (roomStatus[RoomStatus::x][nowRoom] + roomStatus[(int)RoomStatus::w][nowRoom]) < mapWidth ?
			(roomStatus[(int)RoomStatus::x][nowRoom] + roomStatus[(int)RoomStatus::w][nowRoom]) - (roomStatus[(int)RoomStatus::rx][nowRoom] + roomStatus[(int)RoomStatus::rw][nowRoom]) :
			INT_MAX;

		// 下の壁からの距離
		splitLength[2] = roomStatus[(int)RoomStatus::y][nowRoom] > 0 ?
			roomStatus[(int)RoomStatus::ry][nowRoom] - roomStatus[(int)RoomStatus::y][nowRoom] :
			INT_MAX;

		// 上の壁からの距離
		splitLength[3] = (roomStatus[RoomStatus::y][nowRoom] + roomStatus[(int)RoomStatus::h][nowRoom]) < mapHeight ?
			(roomStatus[(int)RoomStatus::y][nowRoom] + roomStatus[(int)RoomStatus::h][nowRoom]) - (roomStatus[(int)RoomStatus::ry][nowRoom] + roomStatus[(int)RoomStatus::rh][nowRoom]) :
			INT_MAX;

		// マックスじゃないものだけ先に
		for (int j = 0, maxLength = sizeof(splitLength) / sizeof(splitLength[0]); j < maxLength; j++) {
			if (splitLength[j] == INT_MAX) continue;
			// 上下左右判定
			if (j < 2) {
				// 道を引く場所を決定
				rootPoint = Random(roomStatus[(int)RoomStatus::ry][nowRoom] + offsetS, roomStatus[(int)RoomStatus::ry][nowRoom] + roomStatus[(int)RoomStatus::rh][nowRoom] - offsetS);

				// マップに書き込む
				for (int w = 1; w <= splitLength[j]; w++) {
					if (j == 0) {
						// 左
						map[(-w) + roomStatus[(int)RoomStatus::rx][nowRoom]][rootPoint] = 2;
					}
					else {
						// 右
						map[w + roomStatus[(int)RoomStatus::rx][nowRoom] + roomStatus[(int)RoomStatus::rw][nowRoom] - offsetS][rootPoint] = 2;

						// 最後
						if (w == splitLength[j]) {
							// 一つ多く作る
							map[w + offsetS + roomStatus[(int)RoomStatus::rx][nowRoom] + roomStatus[(int)RoomStatus::rw][nowRoom] - offsetS][rootPoint] = 2;
						}
					}
				}
			}
			else {
				// 道を引く場所を決定
				rootPoint = Random(roomStatus[(int)RoomStatus::rx][nowRoom] + offsetS, roomStatus[(int)RoomStatus::rx][nowRoom] + roomStatus[(int)RoomStatus::rw][nowRoom] - offsetS);

				// マップに書き込む
				for (int h = 1; h <= splitLength[j]; h++) {
					// 上下判定
					if (j == 2) {
						// 下
						map[rootPoint][(-h) + roomStatus[(int)RoomStatus::ry][nowRoom]] = 2;
					}
					else {
						// 上
						map[rootPoint][h + roomStatus[(int)RoomStatus::ry][nowRoom] + roomStatus[(int)RoomStatus::rh][nowRoom] - offsetS] = 2;

						if (h == splitLength[j]) {
							// 一つ多く作る
							map[rootPoint][h + offsetS + roomStatus[(int)RoomStatus::ry][nowRoom] + roomStatus[(int)RoomStatus::rh][nowRoom] - offsetS] = 2;
						}
					}
				}
			}
		}
	}

	int roadVec1 = 0;	// 道の始点
	int roadVec2 = 0;	// 道の終点

	// 道の接続
	for (int nowRoom = 0; nowRoom < roomNum; nowRoom++) {
		roadVec1 = 0;
		roadVec2 = 0;

		// 道をつなげる
		for (int rootScan = 0; rootScan < roomStatus[(int)RoomStatus::w][nowRoom]; rootScan++) {
			// 道を検索
			if (map[rootScan + roomStatus[(int)RoomStatus::x][nowRoom]][roomStatus[(int)RoomStatus::y][nowRoom]] == Road) {
				if (roadVec1 == 0) {
					// 始点セット
					roadVec1 = rootScan + roomStatus[(int)RoomStatus::x][nowRoom];
				}
				else {
					// 終点セット
					roadVec2 = rootScan + roomStatus[(int)RoomStatus::x][nowRoom];
				}
			}
		}

		// 道を引く
		for (int roadSet = roadVec1; roadSet < roadVec2; roadSet++) {
			// 境界線を上書き
			map[roadSet][roomStatus[(int)RoomStatus::y][nowRoom]] = 2;
		}

		roadVec1 = 0;
		roadVec2 = 0;

		// 道をつなげる
		for (int rootScan = 0; rootScan < roomStatus[(int)RoomStatus::h][nowRoom]; rootScan++) {
			// 道を検索
			if (map[roomStatus[(int)RoomStatus::x][nowRoom]][rootScan + roomStatus[(int)RoomStatus::y][nowRoom]] == Road) {
				if (roadVec1 == 0) {
					// 始点セット
					roadVec1 = rootScan + roomStatus[(int)RoomStatus::y][nowRoom];
				}
				else {
					// 終点セット
					roadVec2 = rootScan + roomStatus[(int)RoomStatus::y][nowRoom];
				}
			}
		}

		// 道を引く
		for (int roadSet = roadVec1; roadSet < roadVec2; roadSet++) {
			// 境界線を上書き
			map[roomStatus[(int)RoomStatus::x][nowRoom]][roadSet] = Road;
		}

	}
	// 階段の生成
	while (1) {

		int rand = GetRand(roomNum - 1);
		int x = Random(roomStatus[RoomStatus::rx][rand], roomStatus[RoomStatus::rx][rand] + roomStatus[RoomStatus::rw][rand] - 1);
		int y = Random(roomStatus[RoomStatus::ry][rand], roomStatus[RoomStatus::ry][rand] + roomStatus[RoomStatus::rh][rand] - 1);

		if (mapObjects[x][y]) continue;
		if (map[x][y] != ObjectType::Room) continue;

		mapObjects[x][y] = true;
		map[x][y] = ObjectType::Stair;
		break;
	}

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (map[x][y] == ObjectType::Stair)
				continue;
		}
	}
}

/// <summary>
/// ロードしたステージをint型多次元配列mapに入れる
/// </summary>
/// <param name="stageID"></param>
void StageGenerator::LoadStageData(int stageID) {
	// ステージのデータを読み込む
	auto data = LoadJsonFile("Scr/Data/StageData.json");
	for (auto s : data) {
		// 指定のステージIDと同じステージを探す
		if (s["id"] != stageID) continue;

		stage.id = stageID;
		stage.playerSpawnPos = VGet(s["playerSpawnPos"][0], 0, s["playerSpawnPos"][1]);
		stage.bossSpawnPos = VGet(s["bossSpawnPos"][0], 0, s["bossSpawnPos"][1]);
		stage.bossType = s["bossType"];

		for (int i = 0; i < mapWidth_Large; i++) {
			for (int j = 0; j < mapHeight_Large; j++) {
				if (s["stageData"][i][j] == nullptr) continue;

				// ステージのデータを入れる
				map[i][j] = s["stageData"][i][j];
			}
		}
	}
	mapWidth = mapWidth_Small;
	mapHeight = mapHeight_Small;

	for (int x = 0; x < mapWidth; x++) {
		for (int z = 0; z < mapHeight; z++) {
			// オブジェクトタイプがRoomじゃないならコンテニュー
			if (map[x][z] != (ObjectType)Room) continue;
			// roomStatusにあればコンテニュー
			if (GetNowRoomNum(VGet(x, 0, z)) != -1) continue;
			// 部屋の生成位置を入れる
			roomStatus[rx][roomCount] = x;
			roomStatus[ry][roomCount] = z;

			for (int w = x; w < mapWidth; w++) {
				if (map[w][z] == (ObjectType)Room) continue;
				// 部屋の幅を入れる
				roomStatus[rw][roomCount] = w - x;
				break;
			}

			for (int h = z; h < mapHeight; h++) {
				if (map[x][h] == (ObjectType)Room) continue;
				// 部屋の高さを入れる
				roomStatus[rh][roomCount] = h - z;
				break;
			}

			roomCount++;
		}
	}
}

StageCell* StageGenerator::UseObject(ObjectType type) {
	StageCell* cell = nullptr;
	switch (type) {
	case Room:
		if (unuseRoom.size() > 0) {
			cell = unuseRoom.front();
			unuseRoom.erase(unuseRoom.begin());
			cell->SetVisible(true);
		}
		else {
			cell = new StageCell(MV1DuplicateModel(groundModel), ObjectType::Room, VZero);
		}
		return cell;
	case Wall:
		if (unuseWall.size() > 0) {
			cell = unuseWall.front();
			unuseWall.erase(unuseWall.begin());
			cell->SetVisible(true);
		}
		else {
			cell = new StageCell(MV1DuplicateModel(wallModel), ObjectType::Wall, VZero);
		}
		return cell;
	case Road:
		if (unuseRoad.size() > 0) {
			cell = unuseRoad.front();
			unuseRoad.erase(unuseRoad.begin());
			cell->SetVisible(true);
		}
		else {
			cell = new StageCell(MV1DuplicateModel(roadModel), ObjectType::Road, VZero);
		}
		return cell;
	case Stair:
		if (unuseStair != nullptr) {
			cell = unuseStair;
			unuseStair = nullptr;
			cell->SetVisible(true);
		}
		else {
			cell = new StageCell(MV1DuplicateModel(stairModel), ObjectType::Stair, VZero);
		}

		return cell;
	}
}

void StageGenerator::UnuseObject(StageCell*& cell) {
	if (cell == nullptr) return;

	cell->SetVisible(false);
	switch (cell->GetObjectType()) {
	case Room:
		unuseRoom.push_back(cell);
		cell = nullptr;
		break;
	case Wall:
		unuseWall.push_back(cell);
		cell = nullptr;
		break;
	case Road:
		unuseRoad.push_back(cell);
		cell = nullptr;
		break;
	case Stair:
		unuseStair = useStair;
		useStair = nullptr;
		cell = nullptr;
		break;
	}
}

void StageGenerator::DrawMap() {
	if (Character::player == nullptr) return;

	// プレイヤーのポジション取得
	VECTOR playerPos = Character::player->GetPosition();
	int x = (int)std::round(playerPos.x / CellSize);
	int z = (int)std::round(playerPos.z / CellSize);

	// マップの表示フラグ
	if (map[x][z] == Room && !stageMap[x][z]) {
		for (int i = 0; i < roomCount; i++) {
			// 部屋のステータスを見て部屋番号i番目の部屋だったら
			if (roomStatus[rx][i] <= x && roomStatus[rx][i] + roomStatus[rw][i] >= x &&
				roomStatus[ry][i] <= z && roomStatus[ry][i] + roomStatus[rh][i] >= z) {

				for (int w = roomStatus[rx][i], wMax = roomStatus[rx][i] + roomStatus[rw][i]; w < wMax; w++) {
					for (int h = roomStatus[ry][i], hMax = roomStatus[ry][i] + roomStatus[rh][i]; h < hMax; h++) {
						stageMap[w][h] = true;
					}
				}

				break;
			}
		}
	}
	else if (map[x][z] != Wall && !stageMap[x][z]) {
		stageMap[x][z] = true;
	}

	// 地図の描画
	for (int w = 0; w < mapWidth_Large; w++) {
		for (int h = 0; h < mapHeight_Large; h++) {
			int color = 0;

			if (!stageMap[w][h]) continue;

			switch (map[w][h]) {
			case Road:
				color = white;
				break;
			case Room:
				color = yellow;
				break;
			case Stair:
				color = green;
				break;
			case Wall:
				continue;
			}
			DrawBox((w * mapSize) + mapOffset.x, (-h * mapSize) + mapOffset.z, (w * mapSize + mapSize) + mapOffset.x, (-h * mapSize + mapSize) + mapOffset.z, color, true);
		}
	}

	// プレイヤーの描画
	DrawBox((x * mapSize) + mapOffset.x, (-z * mapSize) + mapOffset.z, (x * mapSize + mapSize) + mapOffset.x, (-z * mapSize + mapSize) + mapOffset.z, red, true);
}

bool StageGenerator::TransparencyWall(StageCell* cell) {
	if (cell->GetObjectType() == Wall) {
		VECTOR pos = cell->GetPosition();
		VECTOR playerPos = Character::player->GetPosition();
		int x = (int)std::round(playerPos.x / CellSize);
		int z = (int)std::round(playerPos.z / CellSize);

		// プレイヤーのいるマスの一つ下が壁の場合
		if (map[x][z - 1] == (int)Wall) {
			// プレイヤーの座標から見て±1マス内の壁だったら
			if (pos.x + NextCellEnd >= playerPos.x && pos.x - NextCellEnd <= playerPos.x &&
				pos.z + NextCellEnd >= playerPos.z && pos.z + CellEnd <= playerPos.z)
				return true;
		}
	}
	return false;
}

void StageGenerator::ChangeObjectTexture(int textureHandle, ObjectType changeObject) {
	int mHandle;
	int texNum;
	int correct;
	switch (changeObject) {
	case Room:
		for (auto c : unuseRoom) {
			mHandle = c->GetModelHandle();
			texNum = MV1GetMaterialDifMapTexture(mHandle, 0);
			correct = MV1SetTextureGraphHandle(mHandle, texNum, textureHandle, false);
		}

		texNum = MV1GetMaterialDifMapTexture(groundModel, 0);
		correct = MV1SetTextureGraphHandle(groundModel, texNum, textureHandle, false);
		break;
	case Road:
		for (auto c : unuseRoad) {
			mHandle = c->GetModelHandle();
			texNum = MV1GetMaterialDifMapTexture(mHandle, 0);
			correct = MV1SetTextureGraphHandle(mHandle, texNum, textureHandle, false);
		}

		texNum = MV1GetMaterialDifMapTexture(roadModel, 0);
		correct = MV1SetTextureGraphHandle(roadModel, texNum, textureHandle, false);
		break;
	case Wall:
		for (auto c : unuseWall) {
			mHandle = c->GetModelHandle();
			texNum = MV1GetMaterialDifMapTexture(wallModel, 0);
			correct = MV1SetTextureGraphHandle(wallModel, texNum, textureHandle, false);
		}

		texNum = MV1GetMaterialDifMapTexture(groundModel, 0);
		correct = MV1SetTextureGraphHandle(groundModel, texNum, textureHandle, false);
		break;
	case Stair:
		break;
	}
}

int StageGenerator::GetNowRoomNum(VECTOR pos) {
	for (int i = 0; i < roomCount; i++) {
		// 部屋のステータスを見て部屋番号i番目の部屋だったら
		if (roomStatus[rx][i] <= pos.x && roomStatus[rx][i] + roomStatus[rw][i] - 1 >= pos.x &&
			roomStatus[ry][i] <= pos.z && roomStatus[ry][i] + roomStatus[rh][i] - 1 >= pos.z) {

			// 部屋番号を返す
			return i;
		}
	}
	// 無かったら-1で返す
	return -1;
}

int StageGenerator::GetNowRoomNum(int x, int z) {
	for (int i = 0; i < roomCount; i++) {
		// 部屋のステータスを見て部屋番号i番目の部屋だったら
		if (roomStatus[rx][i] <= x && roomStatus[rx][i] + roomStatus[rw][i] - 1 >= x &&
			roomStatus[ry][i] <= z && roomStatus[ry][i] + roomStatus[rh][i] - 1 >= z) {

			// 部屋番号を返す
			return i;
		}
	}
	// 無かったら-1で返す
	return -1;
}

VECTOR StageGenerator::GetRandomRoomRandomPos() {
	int rand = Random(0, roomCount - 1);

	int x = Random(roomStatus[RoomStatus::rx][rand], roomStatus[RoomStatus::rx][rand] + roomStatus[RoomStatus::rw][rand] - 1);
	int y = Random(roomStatus[RoomStatus::ry][rand], roomStatus[RoomStatus::ry][rand] + roomStatus[RoomStatus::rh][rand] - 1);

	return VGet(defaultPos.x + x * CellSize, 0, defaultPos.z + y * CellSize);
}


/// <summary>
/// 座標地点の八方向に壁があるかの判定
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
bool StageGenerator::CheckEightDir(int x, int y) {
	// 範囲外なら false
	if (x < 0 || x >= mapWidth_Large || y < 0 || y >= mapHeight_Large) return false;

	// タイルが壁でないならtrueで返す
	if (map[x][y] != (int)ObjectType::Wall) return true;

	// 8方向をチェックして、どれかが壁でなければ true（周囲が全部壁なら false）
	const int dx[8] = { -1, 1, 0, 0, -1, -1, 1, 1 };
	const int dy[8] = { 0, 0, -1, 1, -1, 1, -1, 1 };

	for (int i = 0; i < 8; ++i) {
		int nx = x + dx[i], ny = y + dy[i];
		if (nx < 0 || nx >= mapWidth_Large || ny < 0 || ny >= mapHeight_Large) {
			// 範囲外は壁と見なす
			continue;
		}
		if (map[nx][ny] != (int)ObjectType::Wall) {
			return true; // 周囲に壁以外があればtrueで返す
		}
	}

	// 周囲がすべて壁なら処理不要
	return false;
}
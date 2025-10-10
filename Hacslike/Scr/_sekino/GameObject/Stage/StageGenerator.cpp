#include "StageGenerator.h"
#include <climits>

StageGenerator::StageGenerator()
	:roomMinNum(8)
	, roomNum(0)
	, parentNum(0)
	, maxArea(0)
	, roomCount(0)
	, line(0) {
	wallModel = MV1LoadModel("Res/Model/Stage/Wall.mv1");
	groundModel = MV1LoadModel("Res/Model/Stage/Room.mv1");
	roadModel = MV1LoadModel("Res/Model/Stage/Road.mv1");
	stairModel = MV1LoadModel("Res/Model/Stage/Stair.mv1");

}

StageGenerator::~StageGenerator() {
	delete[] map;

	for (auto r : roomStatus) {
		delete r;
		r = nullptr;
	}

	delete[] roomStatus;


	MV1DeleteModel(wallModel);
	MV1DeleteModel(groundModel);
	MV1DeleteModel(roadModel);
	MV1DeleteModel(stairModel);
}

void StageGenerator::Update() {
	for (auto c: cells) {
		c->Update();
	}
}

void StageGenerator::Render() {
	for (auto c : cells) {
		c->Render();
	}
}

void StageGenerator::ClearStage() {
	for (auto c : cells) {
		UnuseObject(c);
	}
}

void StageGenerator::StageGenerate() {
	// 部屋数をランダムに決める
	roomNum = Random(roomMinNum, RoomMax);

	// 初期化
	for (int w = 0; w < mapWidth; w++) {
		for (int h = 0; h < mapHeight; h++) {
			// 壁にする
			map[w][h] = 2;
		}
	}

	// 初期化
	for (int w = 0; w < mapWidth; w++) {
		for (int h = 0; h < mapHeight; h++) {
			mapObjects[w][h] = false;
		}
	}

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
			if (splitLength[j] != INT_MAX) {
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
			int x = Random(roomStatus[RoomStatus::rx][rand], roomStatus[RoomStatus::rw][rand]);
			int y = Random(roomStatus[RoomStatus::ry][rand], roomStatus[RoomStatus::rh][rand]);

			if (mapObjects[x][y]) continue;
			if (map[x][y] == ObjectType::Wall) continue;

			mapObjects[x][y] = true;
			map[x][y] = ObjectType::Stair;
			break;
		}

		// オブジェクトを生成する
		for (int nowH = 0; nowH < mapHeight; nowH++) {
			for (int nowW = 0; nowW < mapWidth; nowW++) {
				int dupMHandle = -1;

				if (!CheckEightDir(nowW, nowH)) continue;

				// 複製したモデルハンドルが不正値ならやり直す
				while (dupMHandle == -1) {

					switch ((ObjectType)map[nowW][nowH]) {
					case ObjectType::Wall:
						dupMHandle = MV1DuplicateModel(wallModel);
						break;
					case ObjectType::Room:
						dupMHandle = MV1DuplicateModel(groundModel);
						break;
					case ObjectType::Road:
						dupMHandle = MV1DuplicateModel(roadModel);
						break;
					case ObjectType::Stair:
						dupMHandle = MV1DuplicateModel(stairModel);
						break;
					}

				}
				// セルの生成
				StageCell* c = UseObject((ObjectType)map[nowW][nowH]);
				c->SetPosition(VGet(defaultPos.x + nowW * CellSize, 0, defaultPos.z + nowH * CellSize));
				cells.push_back(c);
			}
		}
}

int StageGenerator::Random(int min, int max) {
	return (min)+GetRand(max - min);
}

/// <summary>
/// 分割点2点のうち大きいほうを分割する
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
bool StageGenerator::SplitPoint(int _x, int _y) {
	//// 分割位置の決定
	//if (_x > _y) {
	//	// 縦を割る
	//	line = Random(roomMinNum + (offsetWall * 2), _x - (offsetWall * 2 + roomMinNum));
	//	return true;
	//}
	//else {
	//	// 横に割る
	//	line = Random(roomMinNum + (offsetWall * 2), _y - (offsetWall * 2 + roomMinNum));
	//	return false;
	//}

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
		int x = Random(roomStatus[RoomStatus::rx][rand], roomStatus[RoomStatus::rw][rand]);
		int y = Random(roomStatus[RoomStatus::ry][rand], roomStatus[RoomStatus::rh][rand]);

		if (mapObjects[x][y]) continue;

		mapObjects[x][y] = true;

		pos = VGet(x * CellSize, 0, y * CellSize);
		if (map[x][y] == 0) break;
	}

	obj->SetPosition(pos);
}

StageCell* StageGenerator::UseObject(ObjectType type) {
	StageCell* cell;
	switch (type) {
	case Room:
		if (unuseRoom.size() > 0) {
			cell = unuseRoom.front();
			unuseRoom.erase(unuseRoom.begin());
		}
		else {
			cell = new StageCell(MV1DuplicateModel(groundModel),ObjectType::Room,VZero);
		}
		return cell;
	case Wall:
		if (unuseWall.size() > 0) {
			cell = unuseWall.front();
			unuseWall.erase(unuseWall.begin());
		}
		else {
			cell = new StageCell(MV1DuplicateModel(wallModel), ObjectType::Wall, VZero);
		}
		return cell;
	case Road:
		if (unuseRoad.size() > 0) {
			cell = unuseRoad.front();
			unuseRoad.erase(unuseRoad.begin());
		}
		else {
			cell = new StageCell(MV1DuplicateModel(roadModel), ObjectType::Road, VZero);
		}
		return cell;
	case Stair:
		if (unuseStair != nullptr) {
			cell = unuseStair;
			unuseStair = nullptr;
		}
		else {
			cell = new StageCell(MV1DuplicateModel(stairModel), ObjectType::Stair, VZero);
		}
		
		return cell;
	}
}

void StageGenerator::UnuseObject(StageCell*& cell) {
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
			unuseStair = cell;
			cell = nullptr;
			break;
	}
}


/// <summary>
/// 座標地点の八方向に壁があるかの判定
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
bool StageGenerator::CheckEightDir(int x, int y) {
	// 安全第一：範囲外なら false（更新/描画不要）
	if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) return false;

	// タイルが壁でないなら処理対象にしたい（元コードの意図に合わせ "true" を返す）
	if (map[x][y] != (int)ObjectType::Wall) return true;

	// 8方向をチェックして、どれかが壁でなければ true（周囲が全部壁なら false）
	const int dx[8] = { -1, 1, 0, 0, -1, -1, 1, 1 };
	const int dy[8] = { 0, 0, -1, 1, -1, 1, -1, 1 };

	for (int i = 0; i < 8; ++i) {
		int nx = x + dx[i], ny = y + dy[i];
		if (nx < 0 || nx >= mapWidth || ny < 0 || ny >= mapHeight) {
			// 範囲外は壁と見なす（または必要に応じて無視）
			continue;
		}
		if (map[nx][ny] != (int)ObjectType::Wall) {
			return true; // 周囲に壁以外があれば処理対象
		}
	}

	// 周囲がすべて壁なら処理不要
	return false;
}
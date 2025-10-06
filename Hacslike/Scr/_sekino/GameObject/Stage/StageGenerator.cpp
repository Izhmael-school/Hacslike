#include "StageGenerator.h"
#include <climits>

StageGenerator::StageGenerator() {
	tileModelHandle = MV1LoadModel("");
}

StageGenerator::~StageGenerator() {
	delete[] map;

	for (auto r : roomStatus) {
		delete r;
		r = nullptr;
	}

	delete[] roomStatus;

	mapObjects.clear();
	mapObjects.shrink_to_fit();

	MV1DeleteModel(tileModelHandle);
}

void StageGenerator::ClearStage() {
	for (auto r : roomStatus) {
		delete r;
		r = nullptr;
	}
	mapObjects.clear();
}

void StageGenerator::StageGenerate() {
	// 部屋数をランダムに決める
	roomNum = roomMinNum + GetRand(roomRand);

	// 初期化
	for (int w = 0; w < mapWidth; w++) {
		for (int h = 0; h < mapHeight; h++) {
			map[w][h] = -1;
		}
	}

	// 部屋を入れる
	roomStatus[(int)x][roomCount] = 0;
	roomStatus[(int)y][roomCount] = 0;
	roomStatus[(int)w][roomCount] = mapWidth;
	roomStatus[(int)h][roomCount] = mapHeight;

	roomCount++;

	// 部屋の数だけ分割する
	for (int splitNum = 0; splitNum < roomNum - 1; splitNum++) {
		// 初期化
		parentNum = 0;
		max = 0;

		// 最大の部屋番号を調べる
		for (int maxCheck = 0; maxCheck < roomNum; maxCheck++) {
			int area = roomStatus[(int)RoomStatus::w][maxCheck] * roomStatus[(int)RoomStatus::h][maxCheck];
			// 面接比較
			if (max < area) {
				// 最大面積上書き
				max = area;

				// 親の部屋番号セット
				parentNum = maxCheck;
			}
		}

		// 取得した部屋をさらに割る
		if (SplitPoint(roomStatus[(int)RoomStatus::w][roomCount], roomStatus[(int)RoomStatus::h][roomCount])) {
			// 取得
			roomStatus[(int)RoomStatus::x][roomCount] = roomStatus[(int)RoomStatus::x][parentNum];
			roomStatus[(int)RoomStatus::y][roomCount] = roomStatus[(int)RoomStatus::y][parentNum];
			roomStatus[(int)RoomStatus::w][roomCount] = roomStatus[(int)RoomStatus::w][parentNum] - line;
			roomStatus[(int)RoomStatus::h][roomCount] = roomStatus[(int)RoomStatus::h][parentNum];

			// 親の部屋を整形する
			roomStatus[(int)RoomStatus::x][roomCount] += roomStatus[(int)RoomStatus::w][roomCount];
			roomStatus[(int)RoomStatus::w][roomCount] -= roomStatus[(int)RoomStatus::w][roomCount];
		}
		else {
			// 取得
			roomStatus[(int)RoomStatus::x][roomCount] = roomStatus[(int)RoomStatus::x][parentNum];
			roomStatus[(int)RoomStatus::y][roomCount] = roomStatus[(int)RoomStatus::y][parentNum];
			roomStatus[(int)RoomStatus::w][roomCount] = roomStatus[(int)RoomStatus::w][parentNum];
			roomStatus[(int)RoomStatus::h][roomCount] = roomStatus[(int)RoomStatus::h][parentNum] - line;

			// 親の部屋を整形する
			roomStatus[(int)RoomStatus::y][roomCount] += roomStatus[(int)RoomStatus::h][roomCount];
			roomStatus[(int)RoomStatus::h][roomCount] -= roomStatus[(int)RoomStatus::h][roomCount];
		}

		// カウントを加算
		roomCount++;
	}

	// 分割した中でランダムな大きさの部屋を生成
	for (int i = 0; i < roomNum; i++) {
		// 生成座標の設定
		roomStatus[(int)RoomStatus::rx][i] = roomStatus[(int)RoomStatus::x][i] + offsetWall + GetRand((roomStatus[(int)RoomStatus::x][i] + roomStatus[(int)RoomStatus::w][i]) - (roomMinNum + offsetWall));
		roomStatus[(int)RoomStatus::rx][i] = roomStatus[(int)RoomStatus::y][i] + offsetWall + GetRand((roomStatus[(int)RoomStatus::y][i] + roomStatus[(int)RoomStatus::h][i]) - (roomMinNum + offsetWall));

		// 部屋の大きさを設定
		roomStatus[(int)RoomStatus::rw][i] = roomMinNum + GetRand(roomStatus[(int)RoomStatus::w][i] - (roomStatus[(int)RoomStatus::rx][i]) - roomStatus[(int)RoomStatus::x][i] - offset);
		roomStatus[(int)RoomStatus::rw][i] = roomMinNum + GetRand(roomStatus[(int)RoomStatus::h][i] - (roomStatus[(int)RoomStatus::ry][i]) - roomStatus[(int)RoomStatus::y][i] - offset);
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

		for (int h = 0, maxH = roomStatus[(int)RoomStatus::rh][c]; h < maxH; h++) {
			for (int w = 0, maxW = roomStatus[(int)RoomStatus::rw][c]; w < maxW; w++) {
				map[w + roomStatus[(int)RoomStatus::rx][c]][h + roomStatus[(int)RoomStatus::ry][c]] = 1;
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

		// 右の壁からの距離
		splitLength[1] = (roomStatus[RoomStatus::y][nowRoom] + roomStatus[(int)RoomStatus::h][nowRoom]) < mapWidth ?
			(roomStatus[(int)RoomStatus::y][nowRoom] + roomStatus[(int)RoomStatus::h][nowRoom]) - (roomStatus[(int)RoomStatus::ry][nowRoom] + roomStatus[(int)RoomStatus::rh][nowRoom]) :
			INT_MAX;

		// マックスじゃないものだけ先に
		for (int j = 0, maxLength = sizeof(splitLength) / sizeof(splitLength[0]); j < maxLength; j++) {
			if (splitLength[j] != INT_MAX) {
				// 上下左右判定
				if (j < 2) {
					// 道を引く場所を決定
					rootPoint = roomStatus[(int)RoomStatus::ry][nowRoom] + offset + GetRand(roomStatus[(int)RoomStatus::ry][nowRoom] + roomStatus[(int)RoomStatus::rh][nowRoom] - offset);
				
					// マップに書き込む

				}
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
	// 分割位置の決定
	if (_x > _y) {
		// 縦を割る
		line = roomMinNum + (offsetWall * 2) + GetRand(_x - (offsetWall * 2 + roomMinNum));
		return true;
	}
	else {
		// 横に割る
		line = roomMinNum + (offsetWall * 2) + GetRand(_y - (offsetWall * 2 + roomMinNum));
		return false;
	}
}

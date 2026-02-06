#pragma once
#include <DxLib.h>
#include "Definition.h"
#include <iostream>

/// <summary>
/// 実際の座標をマップデータ用の座標にする
/// </summary>
/// <param name="pos"></param>
/// <returns></returns>
inline VECTOR ChangeMapPos(VECTOR pos) {
	int x = std::floor(pos.x / CellSize);
	int z = std::floor(pos.z / CellSize);
	return VGet(x, 0, z);
}

inline VECTOR ChangePosMap(VECTOR pos) {
	int x = std::floor(pos.x * CellSize);
	int z = std::floor(pos.z * CellSize);
	return VGet(x, 0, z);
}
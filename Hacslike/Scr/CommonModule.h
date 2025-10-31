#pragma once
#include <vector>
#include "../json.hpp"
#include <DxLib.h>
#include <fstream>
#include <iostream>
using namespace std;

inline nlohmann::json_abi_v3_12_0::json LoadJsonFile(const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open: " << path << endl;
        return nullptr;
    }
    nlohmann::json data;
    file >> data;
    return data;
}

inline int Random(int min, int max){ return (min)+GetRand(max - min); }

/// <summary>
/// ê≥ãKâª
/// </summary>
/// <param name="dir"></param>
/// <returns></returns>
inline VECTOR Normalize(VECTOR dir) {
    float norm = sqrt((pow(dir.x, 2) + pow(dir.y, 2) + pow(dir.z, 2)));
    return VGet(dir.x / norm, dir.y / norm, dir.z / norm);
}

/// <summary>
/// ì‡êœ
/// </summary>
/// <param name="dir1"></param>
/// <param name="dir2"></param>
/// <returns></returns>
inline float Dot(VECTOR dir1,VECTOR dir2) {
    return (dir1.x * dir2.x) + (dir1.y * dir2.y) + (dir1.z * dir2.z);
}

inline VECTOR Cross(VECTOR dir1, VECTOR dir2) {
    float x = dir1.y * dir2.z - dir1.z * dir2.y;
    float y = dir1.x * dir2.z - dir1.z * dir2.x;
    float z = dir1.x * dir2.y - dir1.y * dir2.x;
    return VGet(x, y, z);
}
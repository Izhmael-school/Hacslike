#pragma once
#include <vector>
#include "../json.hpp"
#include <DxLib.h>
#include <fstream>
#include <iostream>

inline nlohmann::json_abi_v3_12_0::json LoadJsonFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open: " << path << std::endl;
        return nullptr;
    }
    nlohmann::json data;
    file >> data;
    return data;
}

inline int Random(int min, int max){ return (min)+GetRand(max - min); }
/// <summary>
/// ³‹K‰»
/// </summary>
/// <param name="dir"></param>
/// <returns></returns>
inline VECTOR Normalize(VECTOR dir) {
    float norm = sqrt((pow(dir.x, 2) + pow(dir.y, 2) + pow(dir.z, 2)));
    return VGet(dir.x / norm, dir.y / norm, dir.z / norm);
}

/// <summary>
/// Œü‚«
/// </summary>
/// <param name="pos1"></param>
/// <param name="pos2"></param>
/// <returns></returns>
inline VECTOR Dir(VECTOR pos1, VECTOR pos2) {
    return VSub(pos1,pos2);
}

/// <summary>
/// ³‹K‰»•t‚«Œü‚«ŒvZ
/// </summary>
/// <param name="pos1"></param>
/// <param name="pos2"></param>
/// <returns></returns>
inline VECTOR NormDir(VECTOR pos1, VECTOR pos2) {
    return Normalize(VSub(pos1, pos2));
}

inline VECTOR Velocity(VECTOR dir,float speed) {

}

/// <summary>
/// “àÏ
/// </summary>
/// <param name="dir1"></param>
/// <param name="dir2"></param>
/// <returns></returns>
inline float Dot(VECTOR dir1, VECTOR dir2) {
    return (dir1.x * dir2.x) + (dir1.y * dir2.y) + (dir1.z * dir2.z);
}

/// <summary>
/// ŠOÏ
/// </summary>
/// <param name="dir1"></param>
/// <param name="dir2"></param>
/// <returns></returns>
inline VECTOR Cross(VECTOR dir1, VECTOR dir2) {
    float x = dir1.y * dir2.z - dir1.z * dir2.y;
    float y = dir1.x * dir2.z - dir1.z * dir2.x;
    float z = dir1.x * dir2.y - dir1.y * dir2.x;
    return VGet(x, y, z);
}

/// <summary>
/// •¶š—ñ‚Ì‡‘Ì
/// </summary>
/// <param name="text"></param>
/// <param name=""></param>
/// <returns></returns>
template<typename... Args>
inline std::string MergeString(const Args&... args) {

    std::string mergedText;
    
    (mergedText += ... += args);

    return mergedText;
}

#pragma once
#include <vector>
#include "Data/json.hpp"
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
/// 正規化
/// </summary>
/// <param name="dir"></param>
/// <returns></returns>
inline VECTOR Normalize(VECTOR dir) {
    float norm = sqrt((pow(dir.x, 2) + pow(dir.y, 2) + pow(dir.z, 2)));
    return VGet(dir.x / norm, dir.y / norm, dir.z / norm);
}

/// <summary>
/// 向き
/// </summary>
/// <param name="pos1"></param>
/// <param name="pos2"></param>
/// <returns></returns>
inline VECTOR Dir(VECTOR pos1, VECTOR pos2) {
    return VSub(pos1,pos2);
}

/// <summary>
/// 正規化付き向き計算
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
/// 内積
/// </summary>
/// <param name="dir1"></param>
/// <param name="dir2"></param>
/// <returns></returns>
inline float Dot(VECTOR dir1, VECTOR dir2) {
    return (dir1.x * dir2.x) + (dir1.y * dir2.y) + (dir1.z * dir2.z);
}

/// <summary>
/// 外積
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
/// 文字列の合体
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

/// <summary>
/// 引数の座標が同じか
/// </summary>
/// <param name="_vec1"></param>
/// <param name="_vec2"></param>
/// <returns></returns>
inline bool CompareVECTOR(VECTOR _vec1,VECTOR _vec2) {
    if (_vec1.x != _vec2.x) return false;
    if (_vec1.y != _vec2.y) return false;
    if (_vec1.z != _vec2.z) return false;
    return true;
}

/// <summary>
/// 中央揃えのための座標計算（参照）
/// </summary>
/// <param name="_str"></param>
/// <param name="_fontHandle"></param>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="exRateX"></param>
/// <param name="exRateY"></param>
inline void StringCenterPos(const TCHAR* _str,int _fontHandle,int* posX , int* posY,int exRateX = 1,int exRateY = 1) {
    int w, h,line;

    GetDrawStringSizeToHandle(&w, &h, &line, _str,(int)_tcslen(_str),_fontHandle);
    *posX -= (int)(w * exRateX) / 2;
    *posY -= (int)(h * exRateY) / 2;
}

/// <summary>
/// 中央揃えのための座標計算（返り値）
/// </summary>
/// <param name="_str"></param>
/// <param name="_fontHandle"></param>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="exRateX"></param>
/// <param name="exRateY"></param>
inline VECTOR StringCenterPos(const TCHAR* _str, int _fontHandle, int posX, int posY, int exRateX = 1, int exRateY = 1) {
    int w, h, line;

    GetDrawStringSizeToHandle(&w, &h, &line, _str, (int)_tcslen(_str), _fontHandle);
    posX -= (int)(w * exRateX) / 2;
    posY -= (int)(h * exRateY) / 2;

    return VGet(posX, posY, 0);
}

/// <summary>
/// 右詰めのための座標計算
/// </summary>
/// <param name="_str"></param>
/// <param name="_fontHandle"></param>
/// <param name="posX"></param>
/// <param name="exRateX"></param>
/// <returns></returns>
inline int StringRightPos(const TCHAR* _str, int _fontHandle, int posX, int exRateX = 1) {
    int w, h, line;

    GetDrawStringSizeToHandle(&w, &h, &line, _str, (int)_tcslen(_str), _fontHandle);

    return posX -= (int)(w * exRateX);
}
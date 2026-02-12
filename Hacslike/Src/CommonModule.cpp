#include "CommonModule.h"

nlohmann::json_abi_v3_12_0::json LoadJsonFile(const std::string& path)
 {
     std::ifstream file(path);
     if (!file.is_open()) {
         std::cerr << "Failed to open: " << path << std::endl;
         return nullptr;
     }
     nlohmann::json data;
     file >> data;
     return data;
}

void StringCenterPos(const TCHAR* _str, int _fontHandle, int* posX, int* posY, int exRateX, int exRateY) {
    int w, h, line;

    if (exRateX == 0) exRateX = 1.0f;
    if (exRateY == 0) exRateY = 1.0f;

    GetDrawStringSizeToHandle(&w, &h, &line, _str, (int)_tcslen(_str), _fontHandle);
    *posX -= (int)(w * exRateX) / 2;
    *posY -= (int)(h * exRateY) / 2;
}

VECTOR StringCenterPos(const TCHAR* _str, int _fontHandle, int posX, int posY, float exRateX, float exRateY) {
    int w, h, line;

    if (exRateX == 0) exRateX = 1.0f;
    if (exRateY == 0) exRateY = 1.0f;

    GetDrawStringSizeToHandle(&w, &h, &line, _str, (int)_tcslen(_str), _fontHandle);
    posX -= (int)(w * exRateX) / 2;
    posY -= (int)(h * exRateY) / 2;

    return VGet(posX, posY, 0);
}

int StringRightPos(const TCHAR* _str, int _fontHandle, int posX, int exRateX) {
    int w, h, line;

    if (exRateX == 0) exRateX = 1.0f;

    GetDrawStringSizeToHandle(&w, &h, &line, _str, (int)_tcslen(_str), _fontHandle);

    return posX -= (int)(w * exRateX);
}

inline VECTOR ImageCenterPos(int gHandle, VECTOR pos, int exRateX, int exRateY) {
    int x, y;
    GetGraphSize(gHandle, &x, &y);

    if (exRateX == 0) exRateX = 1.0f;
    if (exRateY == 0) exRateY = 1.0f;

    pos.x -= (int)(x * exRateX) / 2;
    pos.y -= (int)(y * exRateY) / 2;

    return pos;
}

VECTOR ExpendImageCenterPos(VECTOR pos, int exRateX, int exRateY) {

    if (exRateX == 0) exRateX = 1.0f;
    if (exRateY == 0) exRateY = 1.0f;

    pos.x -= (int)(exRateX) / 2;
    pos.y -= (int)(exRateY) / 2;

    return pos;
}

VECTOR ImageTopPos(int gHandle, int exRateX, int exRateY) {
    int x, y;
    GetGraphSize(gHandle, &x, &y);

    if (exRateX == 0) exRateX = 1.0f;
    if (exRateY == 0) exRateY = 1.0f;

    x = (int)(x * exRateX);
    y = (int)(y * exRateY);

    return VGet(x, y, 0);
}
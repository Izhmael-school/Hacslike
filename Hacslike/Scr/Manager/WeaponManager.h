#pragma once
#include <unordered_map>
#include <string>
#include "../json.hpp"
#include "../Component/Singleton.h"
#include "../Definition.h"
#include <DxLib.h> // DxLibのヘッダ

struct WeaponData {
    int id;
    std::string name;
    std::string modelPath;   // JSONのパス
    int type;
    std::array<float, ATTACK_SPEED_NUM> attackSpeed;
    std::array<float, COL_LENGTH_NUM> colLength;
    std::array<float, COL_RADIUS_NUM> colRadius;
    int modelHandle = -1;    // DxLibのモデルハンドル
};

class WeaponManager : public Singleton<WeaponManager>  {
#pragma region シングルトン
public:
    WeaponManager();
    ~WeaponManager();
#pragma endregion

private:
    std::unordered_map<int, WeaponData> weaponTable;

public:
    void LoadWeapons(const std::string& path);       // JSONロード＋モデルロード
    WeaponData* GetWeapon(int id);                   // IDで取得
    void UnloadAllWeapons();                         // モデル解放
    WeaponData* GetWeaponByName(const std::string& name);

    

    int GetMaxWeaponId() const;
};

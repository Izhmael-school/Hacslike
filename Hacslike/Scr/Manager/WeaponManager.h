#pragma once
#include <unordered_map>
#include <string>
#include "../json.hpp"
#include <DxLib.h> // DxLibのヘッダ

struct WeaponData {
    int id;
    std::string name;
    std::string modelPath;   // JSONのパス
    int type;
    int modelHandle = -1;    // DxLibのモデルハンドル
};

class WeaponManager {
#pragma region シングルトン
private:
    static WeaponManager* pInstance;

private:
    WeaponManager() = default;
    ~WeaponManager() = default;

public:
    WeaponManager(const WeaponManager&) = delete;
    WeaponManager& operator=(const WeaponManager&) = delete;
    WeaponManager(WeaponManager&&) = delete;
    WeaponManager& operator=(WeaponManager&&) = delete;

private:
    static void CreateInstance();

public:
    static WeaponManager* GetInstance();
    static void DestroyInstance();
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

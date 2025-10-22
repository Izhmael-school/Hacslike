#include "WeaponManager.h"
#include <fstream>
#include <iostream>

// 静的メンバ
WeaponManager* WeaponManager::pInstance = nullptr;

void WeaponManager::CreateInstance() {
    if (!pInstance) pInstance = new WeaponManager();
}

WeaponManager* WeaponManager::GetInstance() {
    if (!pInstance) CreateInstance();
    return pInstance;
}

void WeaponManager::DestroyInstance() {
    if (pInstance) {
        pInstance->UnloadAllWeapons(); // モデル解放
        delete pInstance;
        pInstance = nullptr;
    }
}

void WeaponManager::LoadWeapons(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open: " << path << std::endl;
        return;
    }

    nlohmann::json data;
    file >> data;

    for (auto& w : data) {
        if (!w.contains("id")) continue; // ヘッダ行除外
        WeaponData weapon;
        weapon.id = w["id"];
        weapon.name = w["name"];
        weapon.modelPath = w["modelPath"];
        weapon.type = w["type"];
        // attackSpeed は配列想定（3要素）
        if (w.contains("attackSpeed") && w["attackSpeed"].is_array()) {
            for (int i = 0; i < 3; i++) {
                weapon.attackSpeed[i] = (i < w["attackSpeed"].size()) ? w["attackSpeed"][i].get<float>() : 1.0f;
            }
        }
        else {
            weapon.attackSpeed = { 1.0f, 1.0f, 1.0f };
        }
        weapon.modelHandle = MV1LoadModel(weapon.modelPath.c_str());
        if (weapon.modelHandle == -1)
            std::cerr << "Failed to load model: " << weapon.modelPath << std::endl;

        weaponTable[weapon.id] = weapon;
    }
}

WeaponData* WeaponManager::GetWeapon(int id) {
    auto it = weaponTable.find(id);
    if (it != weaponTable.end()) return &it->second;
    return nullptr;
}

void WeaponManager::UnloadAllWeapons() {
    for (auto& pair : weaponTable) {
        if (pair.second.modelHandle != -1)
            MV1DeleteModel(pair.second.modelHandle);
    }
    weaponTable.clear();
}

WeaponData* WeaponManager::GetWeaponByName(const std::string& name) {
    for (auto& [id, weapon] : weaponTable) {
        if (weapon.name == name) return &weapon;
    }
    return nullptr;
}

int WeaponManager::GetMaxWeaponId() const {
    if (weaponTable.empty()) return 0;

    int maxId = 0;
    for (auto& [id, w] : weaponTable) {
        if (id > maxId) maxId = id;
    }
    return maxId;
}


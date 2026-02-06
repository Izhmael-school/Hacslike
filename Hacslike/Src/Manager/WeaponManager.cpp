#include "WeaponManager.h"
#include <fstream>
#include <iostream>


WeaponManager::WeaponManager() {
}

WeaponManager::~WeaponManager() {
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
        if (w.contains("attackSpeed") && w["attackSpeed"].is_array()) {
            for (int i = 0; i < ATTACK_SPEED_NUM; i++) {
                weapon.attackSpeed[i] = (i < w["attackSpeed"].size()) ? w["attackSpeed"][i].get<float>() : 1.0f;
            }
        }
        else {
            weapon.attackSpeed =  DEFAULT_ATTACK_SPEED ;
        }
        if (w.contains("colLength") && w["colLength"].is_array()) {
            for (int i = 0; i < COL_LENGTH_NUM; i++) {
                weapon.colLength[i] = (i < w["colLength"].size()) ? w["colLength"][i].get<float>() : 1.0f;
            }
        }
        else {
            weapon.colLength = DEFAULT_COL_LENGTH;
        }
        if (w.contains("colRadius") && w["colRadius"].is_array()) {
            for (int i = 0; i < COL_RADIUS_NUM; i++) {
                weapon.colRadius[i] = (i < w["colRadius"].size()) ? w["colRadius"][i].get<float>() : 1.0f;
            }
        }
        else {
            weapon.colRadius = DEFAULT_COL_RADIUS;
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


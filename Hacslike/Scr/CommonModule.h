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
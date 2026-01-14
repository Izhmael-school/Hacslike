#include "SaveManager.h"
#include"../GameObject/Character/Player/Player.h"
#include"../Save/SaveIO.h"
#include "../Save/SaveFormat.h"
#include <fstream>
#include <sys/stat.h>
#include <vector>
#include <cstring>
#include <iostream>
#include <cassert>
#ifdef _WIN32
#include <direct.h> // _mkdir
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include "StageManager.h"

static void EnsureSaveDir() {
#ifdef _WIN32
    _mkdir("saves");
#else
    mkdir("saves", 0755);
#endif
}

SaveManager& SaveManager::GetInstance() {
    static SaveManager inst;
    return inst;
}

SaveManager::SaveManager() {
    for (auto& m : slots) m = SaveSlotMeta();
    LoadMeta();
}

void SaveManager::LoadMeta() {
    const auto path = SlotMetaFile();
    std::ifstream in(path, std::ios::binary);
    if (!in) return; // no meta yet
    for (int i = 0; i < 10; ++i) {
        in.read(reinterpret_cast<char*>(&slots[i]), sizeof(SaveSlotMeta));
    }
    in.close();
}

void SaveManager::SaveMeta() {
    EnsureSaveDir();
    const auto path = SlotMetaFile();
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    for (int i = 0; i < 10; ++i) {
        out.write(reinterpret_cast<const char*>(&slots[i]), sizeof(SaveSlotMeta));
    }
    out.close();
}

void SaveManager::RegisterSaveHandler(const std::function<void(BinaryWriter&)>& handler)
{
    saveHandlers.push_back(handler);
}

void SaveManager::RegisterLoadHandler(const std::function<void(BinaryReader&, uint32_t)>& handler)
{
    loadHandlers.push_back(handler);
}

void SaveManager::RegisterSavers()
{
    RegisterSaveHandler([](BinaryWriter& w) {
        if (Player::GetInstance()) {
            Player::GetInstance()->SaveTo(w);
        }
        });
    RegisterLoadHandler([](BinaryReader& r, uint32_t ver) {
        if (Player::GetInstance()) {
            Player::GetInstance()->LoadFrom(r, ver);
        }
        });
   
}

bool SaveManager::Save(int slotIndex) {
    if (slotIndex < 0 || slotIndex >= 10) return false;
    EnsureSaveDir();
    std::string tmp = SlotFileName(slotIndex) + ".tmp";
    std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
    if (!out) return false;

    // Header
    out.write(SAVE_MAGIC, sizeof(SAVE_MAGIC) - 1);
    uint32_t ver = SAVE_VERSION;
    out.write(reinterpret_cast<const char*>(&ver), sizeof(ver));

    BinaryWriter w(out); // BinaryWriter は SaveIO.h により定義済み
    for (auto& h : saveHandlers) {
        h(w); // 正常に呼べるはず
    }
    out.close();

    std::string final = SlotFileName(slotIndex);
    std::remove(final.c_str());
    std::rename(tmp.c_str(), final.c_str());

    slots[slotIndex].exists = true;
    slots[slotIndex].timestamp = std::time(nullptr);
    SaveMeta();
    return true;
}

bool SaveManager::Load(int slotIndex) {
    if (slotIndex < 0 || slotIndex >= 10) return false;
    std::string fname = SlotFileName(slotIndex);
    std::ifstream in(fname, std::ios::binary);
    if (!in) return false;
    char magicBuf[9] = { 0 };
    in.read(magicBuf, sizeof(SAVE_MAGIC) - 1);
    if (std::strncmp(magicBuf, SAVE_MAGIC, sizeof(SAVE_MAGIC) - 1) != 0) {
        in.close();
        return false;
    }
    uint32_t ver = 0;
    in.read(reinterpret_cast<char*>(&ver), sizeof(ver));
    BinaryReader r(in); // BinaryReader は SaveIO.h により定義済み
    for (auto& h : loadHandlers) {
        h(r, ver); // 正常に呼べるはず
    }
    in.close();
    return true;
}

bool SaveManager::Delete(int slotIndex)
{
    if (slotIndex < 0 || slotIndex >= 10) return false;
    std::string fname = SlotFileName(slotIndex);
    if (std::remove(fname.c_str()) != 0) {
        // 削除失敗（ファイルが存在しない場合もある）
        // それでもメタを更新しておく
    }
    slots[slotIndex].exists = false;
    slots[slotIndex].timestamp = 0;
    slots[slotIndex].playerLevel = 0;
    slots[slotIndex].floor = 0;
    std::memset(slots[slotIndex].desc, 0, sizeof(slots[slotIndex].desc));
    SaveMeta();
    return true;
}

const std::array<SaveSlotMeta, 10>& SaveManager::GetSlots() const
{
    return slots;
}

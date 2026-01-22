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
// 追加: Inventory ヘッダ（Player 経由でアクセスする想定）
#include "../GameObject/Item/Inventory.h"
#include"ArtifactManager.h"
#include"SkillManager.h"
#include"EnemyManager.h"
#include "../Enhancement/StatusEnhancement.h" // 追加: StatusEnhancement のセーブ/ロード

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

    RegisterSavers();
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
    // Prevent multiple registrations
    if (saversRegistered) {
        std::cout << "[SaveManager] RegisterSavers called but already registered - skipping." << std::endl;
        return;
    }
    saversRegistered = true;
    // 追加: EnemyManager の保存 / 読み込みハンドラ
    RegisterSaveHandler([](BinaryWriter& w) {
        EnemyManager::GetInstance().SaveTo(w);
        });
    RegisterLoadHandler([](BinaryReader& r, uint32_t ver) {
        EnemyManager::GetInstance().LoadFrom(r, ver);
        });
    // Stage save/load first
    RegisterSaveHandler([](BinaryWriter& w) {
        StageManager::GetInstance().SaveTo(w);
        });
    RegisterLoadHandler([](BinaryReader& r, uint32_t ver) {
        StageManager::GetInstance().LoadFrom(r, ver);
        });

    // Player save/load second
    RegisterSaveHandler([](BinaryWriter& w) {
        Player::GetInstance()->SaveTo(w);
        });
    RegisterLoadHandler([](BinaryReader& r, uint32_t ver) {
        Player::GetInstance()->LoadFrom(r, ver);
        });
    // 追加: Inventory を個別に保存 / 読み込みするハンドラ
    // （Player が Inventory を所有している場合）：
    RegisterSaveHandler([](BinaryWriter& w) {
        if (Player::GetInstance()) {
            // Player にアクセサ GetInventory() を実装しておくこと
            Inventory* inv = Player::GetInstance()->GetInventory();
            if (inv) inv->Save(w);
        }
        });
    RegisterLoadHandler([](BinaryReader& r, uint32_t ver) {
        if (Player::GetInstance()) {
            Inventory* inv = Player::GetInstance()->GetInventory();
            if (inv) inv->Load(r);
        }
        });

    // 追加: ArtifactManager の保存 / 読み込みハンドラ
    RegisterSaveHandler([](BinaryWriter& w) {
        ArtifactManager::GetInstance().SaveTo(w);
        });
    RegisterLoadHandler([](BinaryReader& r, uint32_t ver) {
        ArtifactManager::GetInstance().LoadFrom(r, ver);
        });
    // 追加: SkillManager の保存 / 読み込みハンドラ
    RegisterSaveHandler([](BinaryWriter& w) {
        SkillManager::GetInstance().SaveTo(w);
        });
    RegisterLoadHandler([](BinaryReader& r, uint32_t ver) {
        SkillManager::GetInstance().LoadFrom(r, ver);
        });

    // 追加: StatusEnhancement の保存 / 読み込み
    RegisterSaveHandler([](BinaryWriter& w) {
        StatusEnhancement::GetInstance()->SaveTo(w);
        });
    RegisterLoadHandler([](BinaryReader& r, uint32_t ver) {
        // 注意: StatusEnhancement の実インスタンスが存在している必要がある
        StatusEnhancement::GetInstance()->LoadFrom(r, ver);
        });
}

bool SaveManager::HasLoadedSave() const
{
    return lastLoadSucceeded;
}

void SaveManager::ClearLoadedFlag()
{
    lastLoadSucceeded = false;
}

bool SaveManager::Save(int slotIndex) {
    if (slotIndex < 0 || slotIndex >= 10) return false;
    EnsureSaveDir();
    std::string tmp = SlotFileName(slotIndex) + ".tmp";
    std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
    if (!out) {
        std::cout << "[SaveManager] Failed to open tmp file for writing: " << tmp << std::endl;
        return false;
    }

    std::cout << "[SaveManager] Saving to slot " << slotIndex << " tmp=" << tmp << std::endl;
    std::cout << "[SaveManager] Number of save handlers: " << saveHandlers.size() << std::endl;

    // Header
    out.write(SAVE_MAGIC, sizeof(SAVE_MAGIC) - 1);
    uint32_t ver = SAVE_VERSION;
    out.write(reinterpret_cast<const char*>(&ver), sizeof(ver));

    BinaryWriter w(out); // BinaryWriter �� SaveIO.h �ɂ������`�ς�
    for (auto& h : saveHandlers) {
        h(w); // �����ɌĂׂ��͂�
    }
    out.close();

    // Debug: print size of tmp file
    {
        std::ifstream fin(tmp, std::ios::binary | std::ios::ate);
        if (fin) {
            auto size = fin.tellg();
            std::cout << "[SaveManager] tmp file size = " << size << " bytes" << std::endl;
        }
    }

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
    if (!in) {
        std::cout << "[SaveManager] Failed to open save file for reading: " << fname << std::endl;
        return false;
    }

    // Debug: file size
    in.seekg(0, std::ios::end);
    std::streampos fileSize = in.tellg();
    in.seekg(0, std::ios::beg);
    std::cout << "[SaveManager] Loading from " << fname << " size=" << fileSize << " bytes" << std::endl;
    std::cout << "[SaveManager] Number of load handlers: " << loadHandlers.size() << std::endl;

    char magicBuf[9] = { 0 };
    in.read(magicBuf, sizeof(SAVE_MAGIC) - 1);
    std::cout << "[SaveManager] Read magic: '" << magicBuf << "'" << std::endl;
    if (std::strncmp(magicBuf, SAVE_MAGIC, sizeof(SAVE_MAGIC) - 1) != 0) {
        in.close();
        std::cout << "[SaveManager] Invalid magic - aborting load" << std::endl;
        return false;
    }
    uint32_t ver = 0;
    in.read(reinterpret_cast<char*>(&ver), sizeof(ver));
    std::cout << "[SaveManager] Save version: " << ver << std::endl;

    BinaryReader r(in); // BinaryReader �� SaveIO.h �ɂ������`�ς�
    for (auto& h : loadHandlers) {
        h(r, ver); // �����ɌĂׂ��͂�
    }
    in.close();

    lastLoadSucceeded = true;
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

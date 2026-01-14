#pragma once
#include"../Save/SaveFormat.h"
#include "../Save/SaveIO.h"

#include <array>
#include <vector>
#include <functional>
#include <cstdint>

class BinaryWriter;
class BinaryReader;

class SaveManager
{
public:
    static SaveManager& GetInstance();

    // セーブ・ロード（slotIndex: 0..9）
    bool Save(int slotIndex);
    bool Load(int slotIndex);
    bool Delete(int slotIndex);

    // メタデータ参照（メニュー用）
    const std::array<SaveSlotMeta, 10>& GetSlots() const;

    // 強制的にメタ読み込み（起動時に呼ぶ）
    void LoadMeta();
    void SaveMeta();

    // 関数を登録して、セーブ時に各サブシステムが書き込むようにする
    void RegisterSaveHandler(const std::function<void(BinaryWriter&)>& handler);
    void RegisterLoadHandler(const std::function<void(BinaryReader&, uint32_t)>& handler);
    void RegisterSavers();

private:
    SaveManager();
    std::array<SaveSlotMeta, 10> slots;
    std::vector<std::function<void(BinaryWriter&)>> saveHandlers;
    std::vector<std::function<void(BinaryReader&, uint32_t)>> loadHandlers;
};


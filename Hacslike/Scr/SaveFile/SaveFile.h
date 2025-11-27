#pragma once
#include "PlayerData.h"
#include "ArtifactData.h"
#include "SkillData.h"

//#include "InventoryData.h"
//#include "DungeonState.h"

class SaveFile
{
    int version = 1;

    PlayerData player;
    ArtifactData artifacts;
    SkillData skills;
   /* InventoryData inventory;
    DungeonState dungeon;*/

public:
    // 必要な総バイト数を返す（読み書き時に使用）
    size_t GetBinarySize() const;

    // バイナリへの書き込み
    void SaveToBuffer(char* buffer) const;

    // バイナリから読み込み
    void LoadFromBuffer(const char* buffer);
};


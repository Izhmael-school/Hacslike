#pragma once
#include"../SaveFile/SaveFile.h"
#include <string>

class SaveManager
{
public:
    static const int SLOT_MAX = 10;

    // Save / Load 本体（SaveFile は既存の構造体）
    static bool Save(int slot, const SaveFile& data);
    static bool Load(int slot, SaveFile& out);

    // スロット存在確認（セーブファイルの存在チェック）
    static bool SlotExists(int slot);

    // セーブ削除（ファイルとメタを削除）
    static bool Delete(int slot);

private:
    static void EncryptDecrypt(char* data, size_t size);
    static std::string GetSlotFileName(int slot);
  

   
};


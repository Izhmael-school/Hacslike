#pragma once
#include"../SaveFile/SaveFile.h"
#include"../SaveFile/SaveMeta.h"
#include <string>

class SaveManager
{
public:
    static const int SLOT_MAX = 10;

    static bool Save(int slot, const SaveFile& data);
    static bool Load(int slot, SaveFile& out);

    static bool SlotExists(int slot);

    // ’Ç‰Á
    static bool LoadMeta(int slot, SaveMeta& meta);

    static void Delete(int slot, const SaveFile& data);

private:
    static void EncryptDecrypt(char* data, size_t size);
    static std::string GetSlotFileName(int slot);

     // ’Ç‰Á
    static std::string GetMetaFileName(int slot);
    static bool SaveMetaFile(int slot, const SaveFile& data);
};


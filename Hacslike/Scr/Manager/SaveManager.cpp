#include "SaveManager.h"
#include<fstream>
#include <filesystem>
#include <iostream>

static const unsigned char KEY[] = { 0x5A, 0xAF, 0x13, 0x80 };
static const size_t KEY_SIZE = sizeof(KEY);

std::string SaveManager::GetSlotFileName(int slot)
{
    return std::string("save") + std::to_string(slot) + ".dat";
}


bool SaveManager::SlotExists(int slot)
{
    return std::filesystem::exists(GetSlotFileName(slot));
}

void SaveManager::EncryptDecrypt(char* data, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        data[i] ^= KEY[i % KEY_SIZE];
    }
}


bool SaveManager::Save(int slot, const SaveFile& data)
{
    if (slot < 0 || slot >= SLOT_MAX) return false;

    std::string filename = GetSlotFileName(slot);

    // バイナリサイズ確保（GetBinarySize は SaveFile 側で定義済み）
    size_t size = data.GetBinarySize();
    if (size == 0) return false;

    char* buff = new (std::nothrow) char[size];
    if (!buff) return false;

    char* p = buff;
    // SaveToBuffer は SaveFile 側で実装済み
    data.SaveToBuffer(p);

    // 暗号化（XOR）
    EncryptDecrypt(buff, size);

    // 書き込み
    std::ofstream ofs(filename, std::ios::binary | std::ios::trunc);
    if (!ofs) { delete[] buff; return false; }

    ofs.write(buff, static_cast<std::streamsize>(size));
    ofs.close();

    delete[] buff;

   

    return true;
}

bool SaveManager::Load(int slot, SaveFile& out)
{
    if (slot < 0 || slot >= SLOT_MAX) return false;

    std::string filename = GetSlotFileName(slot);
    if (!std::filesystem::exists(filename)) return false;

    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) return false;

    ifs.seekg(0, std::ios::end);
    std::streamsize sz = ifs.tellg();
    if (sz <= 0) return false;
    ifs.seekg(0, std::ios::beg);

    std::vector<char> buf((size_t)sz);
    ifs.read(buf.data(), sz);
    ifs.close();

    // 復号
    EncryptDecrypt(buf.data(), buf.size());

    // バッファを渡して復元（LoadFromBuffer は SaveFile 側で実装済み）
    const char* p = buf.data();
    try {
        out.LoadFromBuffer(p);
    }
    catch (...) {
        return false;
    }
    return true;
}


bool SaveManager::Delete(int slot)
{
    if (slot < 0 || slot >= SLOT_MAX) return false;

    bool ok = true;
    std::error_code ec;

    std::filesystem::remove(GetSlotFileName(slot), ec);
    if (ec) {
        // ファイルが存在しないなどは無視
    }

    if (ec) {
        // 無視
    }

    return ok;
}

#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <ctime>


/*
  Save file format:
  - Header: magic "HACSLIKE" (8 bytes), uint32_t version
  - Then sections serialized in fixed order: Player, Enemies, Stage, Items, Skills, Artifacts
  - Each section is written by its manager. For forward compatibility each section should write its
    own length or follow manager-specific format.
*/

static const char SAVE_MAGIC[] = "HACSLIKE";
static const uint32_t SAVE_VERSION = 1;

struct SaveSlotMeta {
    bool exists = false;
    uint32_t playerLevel = 0;
    uint32_t floor = 0;
    std::time_t timestamp = 0;
    // optional short description
    char desc[128] = { 0 };

};

inline std::string SlotFileName(int slotIndex) {
    char buf[64];
#ifdef _MSC_VER
    // Visual Studio 向けの安全関数
    sprintf_s(buf, sizeof(buf), "saves/slot_%02d.sav", slotIndex + 1);
#else
    // POSIX / 他のコンパイラ向け
    std::snprintf(buf, sizeof(buf), "saves/slot_%02d.sav", slotIndex + 1);
#endif
    return std::string(buf);
}
// メタファイル名を返す
inline std::string SlotMetaFile() {
    return std::string("saves/slots.meta");
}
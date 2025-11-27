#include "SaveFile.h"
#include <cstring>

#define WRITE(v) memcpy(buffer,&v,sizeof(v)); buffer += sizeof(v)
#define READ(v) memcpy(&v,buffer,sizeof(v)); buffer += sizeof(v)

size_t SaveFile::GetBinarySize() const
{
    // 可変長のデータがあるため一旦仮バッファに書き込んで測る
    size_t approx = 1024 * 16; // 十分大きければOK
    char* buff = new char[approx];
    char* p = buff;

    SaveToBuffer(p);
    size_t total = p - buff;

    delete[] buff;
    return total;
}

void SaveFile::SaveToBuffer(char* buffer) const
{
    WRITE(version);

    player.Save(buffer);
    artifacts.Save(buffer);
    skills.Save(buffer);
   /* inventory.Save(buffer);
    dungeon.Save(buffer);*/
}

void SaveFile::LoadFromBuffer(const char* buffer)
{
    READ(version);

    player.Load(buffer);
    artifacts.Load(buffer);
    skills.Load(buffer);
   /* inventory.Load(buffer);
    dungeon.Load(buffer);*/
}

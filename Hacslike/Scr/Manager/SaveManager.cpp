#include "SaveManager.h"
#include<fstream>

static const unsigned char KEY[] = { 0x5A, 0xAF, 0x13, 0x80 };
static const int KEY_SIZE = sizeof(KEY);

bool SaveManager::Save(int slot, const SaveFile& data)
{
	std::string filename = GetSlotFileName(slot);

	size_t size = data.GetBinarySize();
	char* buff = new char[size];
	char* p = buff;

	data.SaveToBuffer(p);

	EncryptDecrypt(buff, size);

	std::ofstream ofs(filename, std::ios::binary);
	if (!ofs) { delete[] buff; return false; }

	ofs.write(buff, size);
	delete[] buff;

	// ← ここでメタデータも保存
	SaveMetaFile(slot, data);

	return true;
}

bool SaveManager::Load(int slot, SaveFile& out)
{
	std::string filename = GetSlotFileName(slot);

	std::ifstream ifs(filename, std::ios::binary);
	if (!ifs) return false;

	// ファイルサイズ取得
	ifs.seekg(0, std::ios::end);
	size_t size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	char* buff = new char[size];
	ifs.read(buff, size);

	EncryptDecrypt(buff, size);

	const char* p = buff;
	out.LoadFromBuffer(p);

	delete[] buff;
	return true;
}

bool SaveManager::SlotExists(int slot)
{
	std::ifstream ifs(GetSlotFileName(slot), std::ios::binary);
	return ifs.good();
}

bool SaveManager::LoadMeta(int slot, SaveMeta& meta)
{
	std::ifstream ifs(GetMetaFileName(slot));
	if (!ifs)
	{
		meta.isValid = false;
		return false;
	}

	ifs >> meta.floor >> meta.level;
	meta.isValid = true;
	return true;
}

void SaveManager::Delete(int slot, const SaveFile& data)
{
	
}

void SaveManager::EncryptDecrypt(char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		data[i] ^= KEY[i % KEY_SIZE];
	}
}

std::string SaveManager::GetSlotFileName(int slot)
{
	return "save" + std::to_string(slot) + ".dat";
}

std::string SaveManager::GetMetaFileName(int slot)
{
	return "save" + std::to_string(slot) + ".meta";
}

bool SaveManager::SaveMetaFile(int slot, const SaveFile& data)
{
	SaveMeta meta;
	meta.isValid = true;
	/*meta.floor = data.floor;
	meta.level = data.level;*/

	std::ofstream ofs(GetMetaFileName(slot));
	if (!ofs) return false;

	ofs << meta.floor << " " << meta.level;
	return true;
}

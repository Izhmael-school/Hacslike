#include "SkillData.h"
#include <cstring>

#define WRITE(v) memcpy(buffer,&v,sizeof(v)); buffer += sizeof(v)
#define READ(v) memcpy(&v,buffer,sizeof(v)); buffer += sizeof(v)


void SkillData::Save(char*& buffer) const
{
	int count = learned.size();
	WRITE(count);
	for (int id : learned) { WRITE(id); }

	count = locked.size();
	WRITE(count);
	for (int id : locked) { WRITE(id); }
}

void SkillData::Load(const char*& buffer)
{
	int count;

	READ(count);
	learned.resize(count);
	for (int i = 0; i < learned.size(); i++) { READ(learned[i]); }

	READ(count);
	locked.resize(count);
	for (int i = 0; i < locked.size(); i++) { READ(locked[i]); }

}

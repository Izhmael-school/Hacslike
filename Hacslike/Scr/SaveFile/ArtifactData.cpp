#include "ArtifactData.h"
#include<cstring>

#define WRITE(v) memcpy(buffer,&v,sizeof(v)); buffer += sizeof(v)
#define READ(v) memcpy(&v,buffer,sizeof(v)); buffer += sizeof(v)

void ArtifactData::Save(char*& buffer) const
{
	int count = obtained.size();
	WRITE(count);
	for (int id : obtained) { WRITE(id); }

	count = notobtained.size();
	WRITE(count);
	for (int id : notobtained) { WRITE(id); }
}

void ArtifactData::Load(const char*& buffer)
{
	int count;

	READ(count);
	obtained.resize(count);
	for (int i = 0; i < count; i++) { READ(obtained[i]); }

	READ(count);
	notobtained.resize(count);
	for (int i = 0; i < count; i++) { READ(notobtained[i]); }

}

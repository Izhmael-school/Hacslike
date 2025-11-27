#include "PlayerData.h"
#include <cstring>

#define WRITE(v) memcpy(buffer,&v,sizeof(v)); buffer += sizeof(v)
#define READ(v) memcpy(&v,buffer,sizeof(v)); buffer += sizeof(v)


void PlayerData::Save(char*& buffer) const
{
	WRITE(level);
	WRITE(baseAttack);
	WRITE(defense);
	WRITE(maxHp);
	WRITE(hp);
	WRITE(criticalHitRate);
	WRITE(criticalDamage);
	WRITE(coin);
	WRITE(exp);
	WRITE(maxExp);
	WRITE(proximityCorrection);
	WRITE(rangedCorrection);

}

void PlayerData::Load(const char*& buffer)
{

	READ(level);
	READ(baseAttack);
	READ(defense);
	READ(maxHp);
	READ(hp);
	READ(criticalHitRate);
	READ(criticalDamage);
	READ(coin);
	READ(exp);
	READ(maxExp);
	READ(proximityCorrection);
	READ(rangedCorrection);

}

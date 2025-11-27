#pragma once
#include<vector>

class PlayerData
{
/// <summary>
/// セーブする変数（ステータス達）
/// </summary>
public:
	int level;
	int baseAttack;
	int defense;
	int maxHp;
	int hp;

	int criticalHitRate;
	float criticalDamage;

	int coin;
	int exp;
	int maxExp;

	float proximityCorrection;
	float rangedCorrection;

	void Save(char*& buffer) const;
	void Load(const char*& buffer);

};


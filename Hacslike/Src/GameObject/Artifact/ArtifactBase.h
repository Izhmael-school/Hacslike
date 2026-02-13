#pragma once
#include<vector>
#include <string>
#include"../../Manager/SaveManager.h"
class Player;
class PlayerMovement;

#pragma region アーティファクトベース
/// <summary>
/// アーティファクトの基盤クラス
/// これを派生させて行う
/// </summary>
class ArtifactBase
{
protected://メンバ変数
	int id;
	std::string name;			//名前
	std::string description;	//説明
	std::string iconPath;		//アイコン画像のパス
	int iconHandle = -1;

public:	//コンストラクタとデストラクタ
	ArtifactBase(const int _id ,const std::string& _name, const std::string& _desc, const std::string& _icon);

	virtual ~ArtifactBase() = default;

public:	//オーバーライドするメンバ関数
	virtual void Update(Player* player) = 0;
	virtual void Apply(Player* player) = 0;
	virtual void Remove(Player* player) = 0;
	virtual void Save(BinaryWriter& w) = 0;
	virtual void Load(BinaryReader& r, uint32_t version);
	virtual void Restore(Player* player);
public:
	const int GetID() const { return id; }
	const std::string& GetName() const { return name; }
	const std::string& GetDescription() const { return description; }
	const std::string& GetIconPath() const { return iconPath; }
	inline int GetIconHandle() const { return iconHandle; }

};
#pragma endregion

/// --- 派生スキル ---
#pragma region 派生アーティファクト群
/// <summary>
/// HPが一定値以上だと攻撃力上昇
/// </summary>
class conditional_attack_power_raise_HP : public ArtifactBase {
	int attackBoost;
	bool isBoosted;
public:
	conditional_attack_power_raise_HP(int boost = 2);
	void Update(Player* player) override;
	void Apply(Player* player) override;
	void Remove(Player* player) override;
	void Save(BinaryWriter& w) override;
	void Load(BinaryReader& r, uint32_t version) override;
};

/// <summary>
/// HP上限上昇
/// </summary>
class Hp_Max_Raise : public ArtifactBase {
	int MaxHpBoost;

public:
	Hp_Max_Raise(int boost = 100);
	void Update(Player* player) override;
	void Apply(Player* player) override;
	void Remove(Player* player) override;
	void Save(BinaryWriter& w) override;
	void Load(BinaryReader& r, uint32_t version) override;
};

/// <summary>
/// コインの取得量上昇
/// </summary>
class CoinValue_raise : public ArtifactBase {
	int UpValue;
public:
	CoinValue_raise(int value = 2);
	void Update(Player* player) override;
	void Apply(Player* player) override;
	void Remove(Player* player) override;
	void Save(BinaryWriter& w) override;
	void Load(BinaryReader& r, uint32_t version) override;

};

/// <summary>
/// HPが一定値以下だと防御力上昇
/// </summary>
class conditional_defense_power_raise_HP : public ArtifactBase {
	float defenseBoost;
	bool isDefBoosted;
public:
	conditional_defense_power_raise_HP(float boost = 1.2f);
	void Update(Player* player) override;
	void Apply(Player* player) override;
	void Remove(Player* player) override;
	void Save(BinaryWriter& w) override;
	void Load(BinaryReader& r, uint32_t version) override;

};

/// <summary>
/// コイン取得時に攻撃力が少し上昇
/// </summary>
class attactPower_raise_GetCoin : public ArtifactBase {
private:
	float boostAmount;   // 攻撃力上昇量
	float duration;      // 効果時間
	float timer;         // 残り時間
	bool isBoosted;      // 現在上昇中かどうか
	int originalAtk;   //元の攻撃力を保存

public:
	attactPower_raise_GetCoin(float boost = 1.2f, float time = 5.0f);
	// コイン取得時に呼ばれる
	void OnGetCoin(Player* player);
	void Update(Player* player) override;
	void Apply(Player* player) override;
	void Remove(Player* player) override;
	void Save(BinaryWriter& w) override;
	void Load(BinaryReader& r, uint32_t version) override;
};

/// <summary>
/// コイン取得時にアイテムドロップ率が上昇
/// </summary>
class itemDropRateUpwardOnCoinAcquisition : public ArtifactBase {
private:
	float dropRateUpward; //生成確立上昇
	bool  isBoost;		  //上昇中かどうか
	float duration;       // 効果時間
	float timer;          // 残り時間

public:
	itemDropRateUpwardOnCoinAcquisition(float boost = 0.3, float time = 5.0f);
	void OnGetCoin_Item(Player* player);
	void Update(Player* player) override;
	void Apply(Player* player) override;
	void Remove(Player* player) override;
	void Save(BinaryWriter& w) override;
	void Load(BinaryReader& r, uint32_t version) override;
};

/// <summary>
/// 回避後数秒間攻撃力上昇
/// </summary>
class AttackincreasesforSeveralSecondsAfterEvasion : public ArtifactBase {
private:
	float attackPowerUp;  //攻撃力上昇
	bool  isBoost;		  //上昇中かどうか
	float duration;       // 効果時間
	float timer;          // 残り時間
	float originalAtk;   //元の攻撃力を保存
public:
	AttackincreasesforSeveralSecondsAfterEvasion(float boost = 1.5, float time = 3.0f);
	void OnBlinking(PlayerMovement* playermove);
	void Update(Player* player)override;
	void Apply(Player* player) override;
	void Remove(Player* player) override;
	void Save(BinaryWriter& w) override;
	void Load(BinaryReader& r, uint32_t version) override;
	
};

class CriticalHitRateIncreasesForSeveralSecondsAfterEvasion : public ArtifactBase {
private:
	float CriticalUp;  //攻撃力上昇
	bool  isBoost;		  //上昇中かどうか
	float duration;       // 効果時間
	float timer;          // 残り時間
	float originalAtk;   //元の攻撃力を保存
public:
	CriticalHitRateIncreasesForSeveralSecondsAfterEvasion(float boost = 10.0f, float time = 3.0f);
	void OnBlinking(PlayerMovement* playermove);
	void Update(Player* player)override;
	void Apply(Player* player) override;
	void Remove(Player* player) override;
	void Save(BinaryWriter& w) override;
	void Load(BinaryReader& r, uint32_t version) override;
};


class Map : public ArtifactBase {
private:
	bool isDrawMap = false;
public:
	Map();
	void Update(Player* player)override;
	void Apply(Player* player) override;
	void Remove(Player* player) override;
	void Save(BinaryWriter& w) override;
	void Load(BinaryReader& r, uint32_t version) override;


};
#pragma endregion

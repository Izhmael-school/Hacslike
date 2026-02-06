#pragma once
#include "../ItemBase.h"
#include"../../Bullet/BulletPool.h"
#include"../../Effect/Effect.h"
#include"../../../Save/SaveIO.h"



class ItemHeal : public ItemBase
{
private://メンバ変数

	int healValue; //回復量

public://コンストラクタ

	ItemHeal(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0);

public://メンバ関数

	void Start()override;

	void Render()override;

	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;

	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;

};

#pragma region 回復系
/// <summary>
/// 小回復
/// </summary>
class SmallHealItem : public ItemBase {
private://メンバ変数

	int healValue; //回復量

public://コンストラクタ

	SmallHealItem(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0);

public://メンバ関数

	void Start()override;

	void Update()override;

	void Render()override;


	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;


	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;
	inline bool IsEffectFinished() const { return isEffectFinished; }

	ItemType GetItemType() const override { return ItemType::Heal; }

	HealSize GetHealType() const override { return HealSize::Small; }
};

/// <summary>
/// 中回復
/// </summary>
class MiddleHealItem : public ItemBase {
private://メンバ変数

	int healValue; //回復量

public://コンストラクタ

	MiddleHealItem(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0);

public://メンバ関数

	void Start()override;

	void Render()override;

	void Update()override;


	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;


	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;
	inline bool IsEffectFinished() const { return isEffectFinished; }
	ItemType GetItemType() const override { return ItemType::Invaled; }
	HealSize GetHealType() const override { return HealSize::Medium; }

};

/// <summary>
/// 大回復
/// </summary>
class LargeHealItem : public ItemBase {
private://メンバ変数

	int healValue; //回復量

public://コンストラクタ

	LargeHealItem(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0);

public://メンバ関数

	void Start()override;

	void Render()override;
	void Update()override;


	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;


	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;
	inline bool IsEffectFinished() const { return isEffectFinished; }
	ItemType GetItemType() const override { return ItemType::Invaled; }
	HealSize GetHealType() const override { return HealSize::Large; }
};

class Elixir : public ItemBase {
	int healValue; //回復量

public://コンストラクタ

	Elixir(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0);

public://メンバ関数

	void Start()override;

	void Render()override;
	void Update()override;


	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;


	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;
	inline bool IsEffectFinished() const { return isEffectFinished; }
	ItemType GetItemType() const override { return ItemType::Invaled; }
	HealSize GetHealType() const override { return HealSize::Large; }

};
#pragma endregion

#pragma region バフ系
/// <summary>
/// 攻撃力上昇ポーション
/// </summary>
class AttactPotion : public ItemBase {
private://メンバ変数

	int attactValue; //回復量
	bool isBoost;
	float originAttack;//元の攻撃
	float duration;       // 効果時間
	float timer;          // 残り時間

public://コンストラクタ

	AttactPotion(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0,float _time = 120.0f);

public://メンバ関数

	void Start()override;

	void Render()override;

	void Update()override;

	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;


	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;
	inline bool IsEffectFinished() const { return isEffectFinished; }
	ItemType GetItemType() const override { return ItemType::AttackPotion; }
	HealSize GetHealType() const override { return HealSize::Invaled; }

};

/// <summary>
/// 防御力を上げるポーション
/// </summary>
class DefensePotion : public ItemBase {
private://メンバ変数

	int  defenseValue; //回復量
	bool isBoost;
	float originDefense;//元の攻撃
	float duration;       // 効果時間
	float timer;          // 残り時間

public://コンストラクタ

	DefensePotion(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0, float _time = 120.0f);

public://メンバ関数

	void Start()override;

	void Render()override;

	void Update()override;

	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;


	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;
	inline bool IsEffectFinished() const { return isEffectFinished; }
	ItemType GetItemType() const override { return ItemType::DefensePotion; }
	HealSize GetHealType() const override { return HealSize::Invaled; }

};

#pragma endregion

#pragma region グレネード
class Grenade : public ItemBase {
private:
	float timer;			//爆発するまでの時間
	int damage;			//爆発ダメージ
	int grenadeModel;	//グレネードのモデル
	bool isExploded;    // 爆発済み
	BulletPool* pBulletPool;
	Effect* pEffe;
public:
	Grenade(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0);
public://メンバ関数

	void Start()override;

	void Render()override;

	void Update()override;

	void Explode();

	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;


	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;
	inline bool IsEffectFinished() const { return isEffectFinished; }
	ItemType GetItemType() const override { return ItemType::Grenade; }
	HealSize GetHealType() const override { return HealSize::Invaled; }


};
#pragma endregion
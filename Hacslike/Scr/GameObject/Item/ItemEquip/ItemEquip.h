#pragma once
#include "../ItemBase.h"
#include"../../../Save/SaveIO.h"

class Player;

#pragma region 刀
class ItemSword : public ItemBase
{
private://メンバ変数

	int attackValue;		 //攻撃力
	int modelPath = 4;
	std::string weaponType;  //武器の種類

public://コンストラクタ

	ItemSword(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0,const std::string& _weaponType = "");

public://メンバ関数

	void Start()override;

	void Render()override;

	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;

	void UnEquip()override;

	void SaveTo(BinaryWriter& w) override;   
	void LoadFrom(BinaryReader& r) override ;

	inline int GetEffectValue() const { return attackValue; }


public:
	/// <summary>
   /// 武器の種類
   /// </summary>
   /// <returns></returns>
	inline const std::string& GetWeaponType() const { return weaponType; }
	ItemType GetItemType() const override { return ItemType::Invaled; }
	HealSize GetHealType() const override { return HealSize::Invaled; }
	inline void  SetAttackValue(int _v) { attackValue = _v; }

};
#pragma endregion

#pragma region 斧
class ItemAxe : public ItemBase {
private://メンバ変数

	int attackValue; //攻撃力
	int modelPath = 3;
	std::string weaponType;  //武器の種類
public://コンストラクタ

	ItemAxe(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0, const std::string& _weaponType = "");

public://メンバ関数

	void Start()override;

	void Render()override;

	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;
	void UnEquip()override;
	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;
	inline int GetEffectValue() const { return attackValue; }

public:
	/// <summary>
   /// 武器の種類
   /// </summary>
   /// <returns></returns>
	inline const std::string& GetWeaponType() const { return weaponType; }
	ItemType GetItemType() const override { return ItemType::Invaled; }
	HealSize GetHealType() const override { return HealSize::Invaled; }
	inline void  SetAttackValue(int _v) { attackValue = _v; }


};
#pragma endregion

#pragma region 木の棒
class ItemStick : public ItemBase {
private://メンバ変数

	int attackValue; //攻撃力
	int modelPath = 10;
	std::string weaponType;  //武器の種類
public://コンストラクタ

	ItemStick(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0, const std::string& _weaponType = "");

public://メンバ関数

	void Start()override;

	void Render()override;

	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;
	void UnEquip()override;
	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;
	inline int GetEffectValue() const { return attackValue; }

public:
	/// <summary>
   /// 武器の種類
   /// </summary>
   /// <returns></returns>
	inline const std::string& GetWeaponType() const { return weaponType; }
	ItemType GetItemType() const override { return ItemType::Invaled; }
	HealSize GetHealType() const override { return HealSize::Invaled; }
	inline void  SetAttackValue(int _v) { attackValue = _v; }




};
#pragma endregion

#pragma region グレソ
class Greatsword : public ItemBase {
private://メンバ変数

	int attackValue; //攻撃力
	int modelPath = 9;
	std::string weaponType;  //武器の種類
public://コンストラクタ

	Greatsword(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0, const std::string& _weaponType = "");

public://メンバ関数

	void Start()override;

	void Render()override;

	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;
	void UnEquip()override;
	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;
	inline int GetEffectValue() const { return attackValue; }

public:
	/// <summary>
   /// 武器の種類
   /// </summary>
   /// <returns></returns>
	inline const std::string& GetWeaponType() const { return weaponType; }
	ItemType GetItemType() const override { return ItemType::Invaled; }
	HealSize GetHealType() const override { return HealSize::Invaled; }
	inline void  SetAttackValue(int _v) { attackValue = _v; }


};
#pragma endregion

#pragma region 槍
class Spear : public ItemBase {

	int attackValue; //攻撃力
	int modelPath = 2;
	std::string weaponType;  //武器の種類
public://コンストラクタ

	Spear(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0, const std::string& _weaponType = "");

public://メンバ関数

	void Start()override;

	void Render()override;

	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;
	void UnEquip()override;
	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;
	inline int GetAttackValue() const { return attackValue; }

public:
	/// <summary>
   /// 武器の種類
   /// </summary>
   /// <returns></returns>
	inline const std::string& GetWeaponType() const { return weaponType; }
	ItemType GetItemType() const override { return ItemType::Invaled; }
	HealSize GetHealType() const override { return HealSize::Invaled; }
	inline void  SetAttackValue(int _v) { attackValue = _v; }


}; 
#pragma endregion

#pragma region 銃
class gun : public ItemBase {

	int attackValue; //攻撃力
	std::string weaponType;  //武器の種類
public://コンストラクタ

	gun(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0, const std::string& _weaponType = "");

public://メンバ関数

	void Start()override;

	void Render()override;

	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;
	void UnEquip()override;

	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;
	inline int GetEffectValue() const { return attackValue; }


public:
	/// <summary>
   /// 武器の種類
   /// </summary>
   /// <returns></returns>
	inline const std::string& GetWeaponType() const { return weaponType; }
	ItemType GetItemType() const override { return ItemType::Invaled; }
	HealSize GetHealType() const override { return HealSize::Invaled; }
	inline void  SetAttackValue(int _v) { attackValue = _v; }


};
#pragma endregion


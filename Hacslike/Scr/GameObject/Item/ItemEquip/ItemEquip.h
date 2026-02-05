#pragma once
#include "../ItemBase.h"
#include"../../../Save/SaveIO.h"

class Player;

#pragma region åï
class ItemSword : public ItemBase
{
private:
	int attackValue;		 // çUåÇóÕ
	int modelPath = 4;
	std::string weaponType;  // ïêäÌÇÃéÌóﬁ

public:
	ItemSword(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0, const std::string& _weaponType = "");

	void Start()override;
	void Render()override;
	void Use()override;
	void UnEquip()override;

	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;

	inline int GetEffectValue() const override { return attackValue; }
	inline const std::string& GetWeaponType() const { return weaponType; }
	ItemType GetItemType() const override { return ItemType::Invaled; }
	HealSize GetHealType() const override { return HealSize::Invaled; }
	inline void SetAttackValue(int _v) { attackValue = _v; }
};
#pragma endregion

#pragma region ïÄ
class ItemAxe : public ItemBase {
private:
	int attackValue;
	int modelPath = 3;
	std::string weaponType;

public:
	ItemAxe(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0, const std::string& _weaponType = "");

	void Start()override;
	void Render()override;
	void Use()override;
	void UnEquip()override;

	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;

	inline int GetEffectValue() const override { return attackValue; }
	inline const std::string& GetWeaponType() const { return weaponType; }
	ItemType GetItemType() const override { return ItemType::Invaled; }
	HealSize GetHealType() const override { return HealSize::Invaled; }
	inline void SetAttackValue(int _v) { attackValue = _v; }
};
#pragma endregion

#pragma region ñÿÇÃñ_
class ItemStick : public ItemBase {
private:
	int attackValue;
	int modelPath = 10;
	std::string weaponType;

public:
	ItemStick(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0, const std::string& _weaponType = "");

	void Start()override;
	void Render()override;
	void Use()override;
	void UnEquip()override;

	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;

	inline int GetEffectValue() const override { return attackValue; }
	inline const std::string& GetWeaponType() const { return weaponType; }
	ItemType GetItemType() const override { return ItemType::Invaled; }
	HealSize GetHealType() const override { return HealSize::Invaled; }
	inline void SetAttackValue(int _v) { attackValue = _v; }
};
#pragma endregion

#pragma region ÉOÉåÉ\
class Greatsword : public ItemBase {
private:
	int attackValue;
	int modelPath = 9;
	std::string weaponType;

public:
	Greatsword(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0, const std::string& _weaponType = "");

	void Start()override;
	void Render()override;
	void Use()override;
	void UnEquip()override;

	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;

	inline int GetEffectValue() const override { return attackValue; }
	inline const std::string& GetWeaponType() const { return weaponType; }
	ItemType GetItemType() const override { return ItemType::Invaled; }
	HealSize GetHealType() const override { return HealSize::Invaled; }
	inline void SetAttackValue(int _v) { attackValue = _v; }
};
#pragma endregion

#pragma region ëÑ
class Spear : public ItemBase {
private:
	int attackValue;
	int modelPath = 2;
	std::string weaponType;

public:
	Spear(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0, const std::string& _weaponType = "");

	void Start()override;
	void Render()override;
	void Use()override;
	void UnEquip()override;

	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;

	inline int GetEffectValue() const override { return attackValue; }

	inline const std::string& GetWeaponType() const { return weaponType; }
	ItemType GetItemType() const override { return ItemType::Invaled; }
	HealSize GetHealType() const override { return HealSize::Invaled; }
	inline void SetAttackValue(int _v) { attackValue = _v; }
};
#pragma endregion

#pragma region èe
class gun : public ItemBase {
private:
	int attackValue;
	std::string weaponType;

public:
	gun(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0, const std::string& _weaponType = "");

	void Start()override;
	void Render()override;
	void Use()override;
	void UnEquip()override;

	void SaveTo(BinaryWriter& w) override;
	void LoadFrom(BinaryReader& r) override;

	inline int GetEffectValue() const override { return attackValue; }
	inline const std::string& GetWeaponType() const { return weaponType; }
	ItemType GetItemType() const override { return ItemType::Invaled; }
	HealSize GetHealType() const override { return HealSize::Invaled; }
	inline void SetAttackValue(int _v) { attackValue = _v; }
};
#pragma endregion
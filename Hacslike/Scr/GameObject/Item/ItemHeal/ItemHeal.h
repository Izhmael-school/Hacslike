#pragma once
#include "../ItemBase.h"
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
};

#pragma region 回復系
class SmallHealItem : public ItemBase {
private://メンバ変数

	int healValue; //回復量

public://コンストラクタ

	SmallHealItem(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0);

public://メンバ関数

	void Start()override;

	void Render()override;

	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;

};

class MiddleHealItem : public ItemBase {
private://メンバ変数

	int healValue; //回復量

public://コンストラクタ

	MiddleHealItem(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0);

public://メンバ関数

	void Start()override;

	void Render()override;

	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;

};

class LargeHealItem : public ItemBase {
private://メンバ変数

	int healValue; //回復量

public://コンストラクタ

	LargeHealItem(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0);

public://メンバ関数

	void Start()override;

	void Render()override;

	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;

};
#pragma endregion


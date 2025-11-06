#pragma once
#include "../ItemBase.h"

class Player;

class ItemSword : public ItemBase
{
private://メンバ変数

	int attackValue; //攻撃力

public://コンストラクタ

	ItemSword(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0);

public://メンバ関数

	void Start()override;

	void Render()override;

	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;

	void UnEquip()override;


};

class ItemAxe : public ItemBase {
private://メンバ変数

	int attackValue; //攻撃力

public://コンストラクタ

	ItemAxe(VECTOR _pos = VZero, const std::string& _name = "", const std::string& _desc = "", int _value = 0, int _effectValue = 0);

public://メンバ関数

	void Start()override;

	void Render()override;

	/// <summary>
	/// アイテムを使用した時の効果
	/// </summary>
	void Use()override;
	void UnEquip()override;

};
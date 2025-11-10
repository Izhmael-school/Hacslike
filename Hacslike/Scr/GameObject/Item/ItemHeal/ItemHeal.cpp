#include "ItemHeal.h"
#include <DxLib.h>   // DxLibキー入力を使う場合
#include <iostream>
#include"../../Character/Player/Player.h"


ItemHeal::ItemHeal(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue)
	:ItemBase(VZero, "item", _name, _desc, "Consumable", _value, _effectValue, "Res/ItemIcon/potion.png")
	, healValue(_effectValue) {
}

void ItemHeal::Start()
{
}

void ItemHeal::Render()
{
}

void ItemHeal::Use()
{

	Character::player->AddHp(healValue);
}
#pragma region 回復系
/// <summary>
/// 回復（小）
/// </summary>
/// <param name="_pos"></param>
/// <param name="_name"></param>
/// <param name="_desc"></param>
/// <param name="_value"></param>
/// <param name="_effectValue"></param>
SmallHealItem::SmallHealItem(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue)
	:ItemBase(VZero, "item", _name, _desc, "Consumable", _value, _effectValue, "Res/ItemIcon/potion.png")
	, healValue(_effectValue) {
}

void SmallHealItem::Start()
{
}

void SmallHealItem::Render()
{
}

void SmallHealItem::Use()
{

	Character::player->AddHp(healValue);
}

/// <summary>
/// 回復（中）
/// </summary>
/// <param name="_pos"></param>
/// <param name="_name"></param>
/// <param name="_desc"></param>
/// <param name="_value"></param>
/// <param name="_effectValue"></param>
MiddleHealItem::MiddleHealItem(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue)
	:ItemBase(VZero, "item", _name, _desc, "Consumable", _value, _effectValue, "Res/ItemIcon/potion.png")
	, healValue(_effectValue) {
}

void MiddleHealItem::Start()
{
}

void MiddleHealItem::Render()
{
}

void MiddleHealItem::Use()
{
	Character::player->AddHp(healValue);
}
#pragma endregion

/// <summary>
/// 回復(大)
/// </summary>
/// <param name="_pos"></param>
/// <param name="_name"></param>
/// <param name="_desc"></param>
/// <param name="_value"></param>
/// <param name="_effectValue"></param>
LargeHealItem::LargeHealItem(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue)
	:ItemBase(VZero, "item", _name, _desc, "Consumable", _value, _effectValue, "Res/ItemIcon/potion.png")
	, healValue(_effectValue) {
}

void LargeHealItem::Start()
{
}

void LargeHealItem::Render()
{
}

void LargeHealItem::Use()
{
	Character::player->AddHp(healValue);

}

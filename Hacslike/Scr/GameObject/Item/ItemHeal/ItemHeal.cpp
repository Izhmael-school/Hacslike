#include "ItemHeal.h"
#include <DxLib.h>   // DxLibƒL[“ü—Í‚ğg‚¤ê‡
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


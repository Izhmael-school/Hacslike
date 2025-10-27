#include "ItemEquip.h"
#include <iostream>

ItemSword::ItemSword(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue)
	: ItemBase(VZero, "item", _name, _desc, "Equipment", _value, _effectValue, "Res/ItemIcon/sword.png")
	, attackValue(_effectValue) {
}

void ItemSword::Start()
{
}

void ItemSword::Render()
{
}

void ItemSword::Use()
{
	std::cout << name << " ‚ð‘•”õIUŒ‚—Í +" << attackValue << "I\n";
}

ItemAxe::ItemAxe(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue)
	: ItemBase(VZero, "item", _name, _desc, "Equipment", _value, _effectValue, "Res/ItemIcon/Axe.png")
	, attackValue(_effectValue) {
}

void ItemAxe::Start()
{
}

void ItemAxe::Render()
{
}

void ItemAxe::Use()
{
}
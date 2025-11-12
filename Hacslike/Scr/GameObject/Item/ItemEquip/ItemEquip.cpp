#include "ItemEquip.h"
#include <iostream>
#include"../../Character/Character.h"
#include"../../Character/Player/Player.h"

#pragma region “
ItemSword::ItemSword(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue, const std::string& _weaponType)
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
	Player::GetInstance()->ChangeWeapon(4);
	
	Player::GetInstance()->SetAtk(Player::GetInstance()->GetBaseAtk() + Player::GetInstance()->GetProximityCorrection() + attackValue);
	
	std::cout << name << " ‚ð‘•”õIUŒ‚—Í +" << attackValue << "I\n";
}

void ItemSword::UnEquip()
{
}
#pragma endregion

#pragma region •€
ItemAxe::ItemAxe(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue, const std::string& _weaponType)
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
	Player::GetInstance()->ChangeWeapon(3);
	Player::GetInstance()->SetAtk(Player::GetInstance()->GetBaseAtk() + Player::GetInstance()->GetProximityCorrection() + attackValue);

	
}

void ItemAxe::UnEquip()
{
}
#pragma endregion

#pragma region –Ø‚Ì–_
ItemStick::ItemStick(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue, const std::string& _weaponType)
	: ItemBase(VZero, "item", _name, _desc, "Equipment", _value, _effectValue, "Res/ItemIcon/stick.png")
	, attackValue(_effectValue) {
}

void ItemStick::Start()
{
}

void ItemStick::Render()
{
}

void ItemStick::Use()
{
	Player::GetInstance()->ChangeWeapon(10);
	Player::GetInstance()->SetAtk(Player::GetInstance()->GetBaseAtk() + Player::GetInstance()->GetProximityCorrection() + attackValue);

}

void ItemStick::UnEquip()
{
}
#pragma endregion

#pragma region ƒOƒŒƒ\
Greatsword::Greatsword(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue, const std::string& _weaponType)
	: ItemBase(VZero, "item", _name, _desc, "Equipment", _value, _effectValue, "Res/ItemIcon/rune-sword.png")
	, attackValue(_effectValue) {
}

void Greatsword::Start()
{
}

void Greatsword::Render()
{
}

void Greatsword::Use()
{
	Player::GetInstance()->ChangeWeapon(9);
	Player::GetInstance()->SetAtk(Player::GetInstance()->GetBaseAtk() + Player::GetInstance()->GetProximityCorrection() + attackValue);

}

void Greatsword::UnEquip()
{
}
#pragma endregion

#pragma region ‘„
Spear::Spear(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue, const std::string& _weaponType)
	: ItemBase(VZero, "item", _name, _desc, "Equipment", _value, _effectValue, "Res/ItemIcon/coiled-nail.png")
	, attackValue(_effectValue) {
}

void Spear::Start()
{
}

void Spear::Render()
{
}

void Spear::Use()
{
	Player::GetInstance()->ChangeWeapon(2);
	Player::GetInstance()->SetAtk(Player::GetInstance()->GetBaseAtk() + Player::GetInstance()->GetProximityCorrection() + attackValue);

}

void Spear::UnEquip()
{
}
#pragma endregion
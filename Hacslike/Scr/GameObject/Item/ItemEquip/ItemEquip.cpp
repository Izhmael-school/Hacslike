#include "ItemEquip.h"
#include <iostream>
#include"../../Character/Character.h"
#include"../../Character/Player/Player.h"
#include"../../../Manager/AudioManager.h"

#pragma region “
ItemSword::ItemSword(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue, const std::string& _weaponType)
	: ItemBase(VZero, "item","Sword_Iron", _name, _desc, "Equipment", _value, _effectValue, "Res/ItemIcon/sword.png")
	, attackValue(_effectValue) {
	Start();
}

void ItemSword::Start()
{
	AudioManager::GetInstance().Load("Res/SE/UseItem.mp3", "UseEquip", false);
}

void ItemSword::Render()
{
}

void ItemSword::Use()
{
	AudioManager::GetInstance().PlayOneShot("UseEquip");

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
	: ItemBase(VZero, "item","Axe", _name, _desc, "Equipment", _value, _effectValue, "Res/ItemIcon/Axe.png")
	, attackValue(_effectValue) {
	Start();
}

void ItemAxe::Start()
{
	AudioManager::GetInstance().Load("Res/SE/UseItem.mp3", "UseEquip", false);
}

void ItemAxe::Render()
{
}

void ItemAxe::Use()
{
	AudioManager::GetInstance().PlayOneShot("UseEquip");

	Player::GetInstance()->ChangeWeapon(3);
	Player::GetInstance()->SetAtk(Player::GetInstance()->GetBaseAtk() + Player::GetInstance()->GetProximityCorrection() + attackValue);

	
}

void ItemAxe::UnEquip()
{
}
#pragma endregion

#pragma region –Ø‚Ì–_
ItemStick::ItemStick(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue, const std::string& _weaponType)
	: ItemBase(VZero, "item","Stick", _name, _desc, "Equipment", _value, _effectValue, "Res/ItemIcon/stick.png")
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
	: ItemBase(VZero, "item","Greatsword", _name, _desc, "Equipment", _value, _effectValue, "Res/ItemIcon/rune-sword.png")
	, attackValue(_effectValue) {
	Start();
}

void Greatsword::Start()
{
	AudioManager::GetInstance().Load("Res/SE/UseItem.mp3", "UseEquip", false);
}

void Greatsword::Render()
{
}

void Greatsword::Use()
{
	AudioManager::GetInstance().PlayOneShot("UseEquip");

	Player::GetInstance()->ChangeWeapon(9);
	Player::GetInstance()->SetAtk(Player::GetInstance()->GetBaseAtk() + Player::GetInstance()->GetProximityCorrection() + attackValue);

}

void Greatsword::UnEquip()
{
}
#pragma endregion

#pragma region ‘„
Spear::Spear(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue, const std::string& _weaponType)
	: ItemBase(VZero, "item", "Spear", _name, _desc, "Equipment", _value, _effectValue, "Res/ItemIcon/coiled-nail.png")
	, attackValue(_effectValue) {
	Start();
}

void Spear::Start()
{
	AudioManager::GetInstance().Load("Res/SE/UseItem.mp3", "UseEquip", false);
}

void Spear::Render()
{
}

void Spear::Use()
{
	AudioManager::GetInstance().PlayOneShot("UseEquip");

	Player::GetInstance()->ChangeWeapon(2);
	Player::GetInstance()->SetAtk(Player::GetInstance()->GetBaseAtk() + Player::GetInstance()->GetProximityCorrection() + attackValue);

}

void Spear::UnEquip()
{
}
#pragma endregion

#pragma region e
gun::gun(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue, const std::string& _weaponType)
	: ItemBase(VZero, "item", "Gun", _name, _desc, "Equipment", _value, _effectValue, "Res/ItemIcon/Gun.png")
	, attackValue(_effectValue) {
	Start();
}

void gun::Start()
{
	AudioManager::GetInstance().Load("Res/SE/UseItem.mp3", "UseEquip", false);

}

void gun::Render()
{
}

void gun::Use()
{
	AudioManager::GetInstance().PlayOneShot("UseEquip");

	Player::GetInstance()->ChangeWeapon(11);
	Player::GetInstance()->SetAtk(Player::GetInstance()->GetBaseAtk() + Player::GetInstance()->GetRangedCorrection() + attackValue);

}

void gun::UnEquip()
{
}
#pragma endregion
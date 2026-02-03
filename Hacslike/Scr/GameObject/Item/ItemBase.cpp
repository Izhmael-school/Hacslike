#include "ItemBase.h"
#include"../../Manager/InputManager.h"
#include"../../Component/Collider/Collider.h"

ItemBase::ItemBase(VECTOR _pos, std::string tag, const std::string& _id, const std::string& _name, const std::string& _desc, const std::string& _type,
	int _value, int _effectValue, const std::string& _icon)
	:GameObject(_pos, tag)
	, name(_name)
	, description(_desc)
	, type(_type)
	, value(_value)
	, itenEffectValue(_effectValue)
	, itemIcon(_icon)
	,id(_id){

	itemImageHandle = LoadGraph(itemIcon.c_str());
}

ItemBase::~ItemBase()
{
}

void ItemBase::Use()
{
}

void ItemBase::SaveTo(BinaryWriter& w) 
{
}

void ItemBase::LoadFrom(BinaryReader& r)
{
}

ItemBase* ItemBase::item;
#include "ItemEntity.h"

/// <summary>
/// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
/// </summary>
ItemEntity::ItemEntity(std::unique_ptr<ItemBase> _item, const VECTOR& _pos, float _radius)
	:item(std::move(_item)), position(_pos), radius(50), pCollider(nullptr), isCollected(false) {
	// ƒRƒ‰ƒCƒ_[‚ğì¬‚µ‚Ä“o˜^
	pCollider = new SphereCollider(this, position, radius); // ©š‚±‚±‚Å‘ã“ü
	this->SetCollider(pCollider);                        // © GameObject ‚É‚à“o˜^
	//CollisionManager::GetInstance().Register(pCollider); // © “–‚½‚è”»’è“o˜^
	this->SetTag("item");								  //ƒ^ƒO‚Ì’Ç‰Á
}

ItemEntity::~ItemEntity()
{

}

void ItemEntity::Start()
{
}

void ItemEntity::Update()
{
	if (pCollider != nullptr)
	{
		pCollider->SetMatrix(matrix);
		pCollider->Update();
	}
}

void ItemEntity::Render()
{
#if _DEBUG
	//“–‚½‚è”»’è‚Ì•`‰æ
	if (pCollider != nullptr) {
		pCollider->Render();
	}
#endif
}

void ItemEntity::Use()
{
}

std::unique_ptr<ItemBase> ItemEntity::TakeItem()
{
	return std::move(item);
}
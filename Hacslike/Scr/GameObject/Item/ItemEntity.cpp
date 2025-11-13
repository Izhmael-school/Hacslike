#include "ItemEntity.h"

/// <summary>
/// コンストラクタ
/// </summary>
ItemEntity::ItemEntity(std::unique_ptr<ItemBase> _item, const VECTOR& _pos, float _radius)
	:item(std::move(_item)), position(_pos), radius(50), pCollider(nullptr), isCollected(false) {
	// コライダーを作成して登録
	pCollider = new SphereCollider(this, position, radius); // ←★ここで代入
	this->SetCollider(pCollider);                        // ← GameObject にも登録
	CollisionManager::GetInstance().Register(pCollider); // ← 当たり判定登録
	this->SetTag("item");								  //タグの追加
}

ItemEntity::~ItemEntity()
{

}

void ItemEntity::Start()
{
	if (item)
		item->Start(); // ←★ アイテム固有のStartを呼び出す
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
	//当たり判定の描画
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
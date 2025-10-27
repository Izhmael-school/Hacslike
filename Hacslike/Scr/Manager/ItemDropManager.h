#pragma once
#include"../Definition.h"
#include "../GameObject/Item/ItemFactory.h"
#include "../GameObject/Item/ItemEntity.h"
#include"../GameObject/Effect/Effect.h"
#include <string>
#include <random>
#include <vector>

class ItemDropManager
{
public: //シングルトン
	static ItemDropManager& Instance();

public://メンバ変数
	std::vector<std::unique_ptr<ItemEntity>> activeItems;

	Effect* pEffe;

public: //メンバ関数
	void TryDropItem(float _dropRate, VECTOR _pos = VZero);

	void Update();
	void Render();
	/// <summary>
	/// アイテムの削除
	/// </summary>
	/// <param name="target"></param>
	void RemoveItem(ItemEntity* target);
private://メンバ関数
	bool RandomChance(float _rate);
	void PlayDropEffect(VECTOR _pos = VZero);

public:
	// ★ 外部からアイテム一覧を取得できる関数を追加
	const std::vector<std::unique_ptr<ItemEntity>>& GetActiveItems() const { return activeItems; }

};


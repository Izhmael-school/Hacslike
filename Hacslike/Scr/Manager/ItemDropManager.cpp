#include "ItemDropManager.h"
#include"../GameObject/Effect/Effect.h"
#include "../Manager/EffectManager.h"
#include <iostream>

ItemDropManager& ItemDropManager::Instance()
{
    static ItemDropManager instance;
    return instance;
    // TODO: return ステートメントをここに挿入します
}

void ItemDropManager::TryDropItem(float _dropRate, VECTOR _pos)
{
    if (!RandomChance(_dropRate)) return;

    std::vector<std::string> itemIds = { "Potion_Small", "Sword_Iron","Axe" };

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dis(0, static_cast<int>(itemIds.size()) - 1);

    std::string chosenId = itemIds[dis(gen)];
    auto item = ItemFactory::Instance().CreateItem(chosenId);


    if (item)
    {
#if _DEBUG
        printfDx("「%s」を生成した！", item->GetName().c_str(), GetColor(255, 255, 255));
        std::cout << item->GetName() << " をドロップ！\n";

#endif
        PlayDropEffect(_pos);
        // アイテム実体を生成（当たり判定付き）
        activeItems.push_back(std::make_unique<ItemEntity>(std::move(item), _pos, 50.0f));
    }
}

void ItemDropManager::Update()
{
    for (auto& item : activeItems)
    {
        item->Update();
    }

}

void ItemDropManager::Render()
{
    for (auto& item : activeItems)
    {
        item->Render();
    }
}

/// <summary>
/// アイテムの削除
/// </summary>
/// <param name="target"></param>
void ItemDropManager::RemoveItem(ItemEntity* target)
{
    activeItems.erase(
        std::remove_if(activeItems.begin(), activeItems.end(),
            [target](const std::unique_ptr<ItemEntity>& item) {
                if (item.get() == target) {
                    if (item->GetCollider()) {
                        CollisionManager::GetInstance()->UnRegister(item->GetCollider());
                    }
                    return true;
                }
                return false;
            }),
        activeItems.end());

    
}

bool ItemDropManager::RandomChance(float _rate)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen) < _rate;
}

void ItemDropManager::PlayDropEffect(VECTOR _pos)
{
    // 生成エフェクトなど
    //pEffe = EffectManager::GetInstance()->Instantiate("Item", _pos);
}

#include "ItemDropManager.h"
#include"../GameObject/Effect/Effect.h"
#include "../Manager/EffectManager.h"
#include <iostream>
#include "AudioManager.h"


ItemDropManager::ItemDropManager()
    :pEffe()
{
    Start();
}

ItemDropManager::~ItemDropManager()
{
}


void ItemDropManager::TryDropItem(float _dropRate, VECTOR _pos)
{
    if (!RandomChance(_dropRate)) return;

    std::vector<std::string> itemIds = { "Potion_Small","Potion_Middle","Potion_Large","AttactPotion","DefensePotion","Grenade", "Sword_Iron","Axe","Greatsword", "Spear"};

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
        AudioManager::GetInstance().PlayOneShot("DropItem");
        auto* eff = EffectManager::GetInstance().Instantiate("Item", _pos);

        auto entity = std::make_unique<ItemEntity>(std::move(item), _pos, 50.0f);
        entity->SetDropEffect(eff);

        activeItems.push_back(std::move(entity));
    }
}

void ItemDropManager::Start()
{
    AudioManager::GetInstance().Load("Res/SE/itemDrop.mp3", "DropItem", false);
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
                    // エフェクトを消す
                    if (item->GetDropEffect()) {
                        item->GetDropEffect()->Stop();
                    }

                    if (item->GetCollider()) {
                        CollisionManager::GetInstance().UnRegister(item->GetCollider());
                    }
                    return true;
                }
                return false;
            }),
        activeItems.end());

 
}

/// <summary>
/// 全てのアイテム削除
/// </summary>
void ItemDropManager::RemoveItemAll()
{
    // この関数が呼ばれたら全ての落ちているアイテムを消す
    for (auto& item : activeItems) {
        if (!item) continue;

        // エフェクトを消す
        if (item->GetDropEffect()) {
            item->GetDropEffect()->Stop();
        }

        // コライダ登録を解除
        if (item->GetCollider()) {
            CollisionManager::GetInstance().UnRegister(item->GetCollider());
        }
    }

    // unique_ptr のデストラクタでアイテムは削除される
    activeItems.clear();
}

bool ItemDropManager::RandomChance(float _rate)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen) < _rate;
}


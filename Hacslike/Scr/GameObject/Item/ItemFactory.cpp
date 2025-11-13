#include "ItemFactory.h"
#include"ItemEquip/ItemEquip.h"
#include"ItemHeal/ItemHeal.h"

ItemFactory& ItemFactory::Instance()
{
    // TODO: return ステートメントをここに挿入します
    static ItemFactory instance;
    return instance;
}

ItemFactory* ItemFactory::GetInstance()
{
    static ItemFactory instance;
    return &instance;
}

/// <summary>
/// アイテムの設定
/// </summary>
/// <param name="id"></param>
/// <param name="generator"></param>
void ItemFactory::RegisterItem(const std::string& _id, std::function<std::unique_ptr<ItemBase>()> _generator)
{
    itemTemplates[_id] = _generator;
}

/// <summary>
/// アイテムの生成
/// </summary>
/// <param name="id"></param>
/// <returns></returns>
std::unique_ptr<ItemBase> ItemFactory::CreateItem(const std::string& _id)
{
    auto it = itemTemplates.find(_id);
    if (it != itemTemplates.end()) {
        return it->second();
    }

    printfDx("ItemFactory: ID '%s' のアイテムが登録されていません。\n", _id.c_str());

    return nullptr;
}

void ItemFactory::InitializeDefaultItems()
{

    RegisterItem("Potion_Small", []() {
        return std::make_unique<SmallHealItem>(VGet(0, 0, 0),
        "ポーション(小)", "体力を少し回復する", 50, 20);
        });

    RegisterItem("Potion_Middle", []() {
        return std::make_unique<MiddleHealItem>(VGet(0, 0, 0),
        "ポーション(中)", "体力を回復する", 80, 50);
        });

    RegisterItem("Potion_Large", []() {
        return std::make_unique<LargeHealItem>(VGet(0, 0, 0),
        "ポーション(大)", "体力を大幅に回復する", 110, 100);
        });

    RegisterItem("AttactPotion", []() {
        return std::make_unique<AttactPotion>(VGet(0, 0, 0),
        "攻撃のポーション", "2分間攻撃力を上げる", 110, 5,120.0f);
        });

    RegisterItem("DefensePotion", []() {
        return std::make_unique<DefensePotion>(VGet(0, 0, 0),
        "防御のポーション", "2分間防御力を上げる", 110, 5, 120.0f);
        });

    RegisterItem("Sword_Iron", []() {
        return std::make_unique<ItemSword>(VGet(0, 0, 0),
        "剣", "普通の剣", 150, GetRand(20) + 10 ,"MeleeWeapon");
        });

    RegisterItem("Axe", []() {
        return std::make_unique<ItemAxe>(VGet(0, 0, 0),
        "斧", "普通の斧", 200, GetRand(30) + 10, "RangedWeapon");
        });

    RegisterItem("Stick", []() {
        return std::make_unique<ItemStick>(VGet(0, 0, 0),
        "木の棒", "そこら辺に落ちてる木の棒", 0, 10, "MeleeWeapon");
        });

    RegisterItem("Greatsword", []() {
        return std::make_unique<Greatsword>(VGet(0, 0, 0),
        "グレートソード", "重い！強い！かっこいい！", 500, GetRand(150) + 80, "MeleeWeapon");
        });

    RegisterItem("Spear", []() {
        return std::make_unique<Spear>(VGet(0, 0, 0),
        "槍", "槍", 230, GetRand(80) + 40, "MeleeWeapon");
        });
}

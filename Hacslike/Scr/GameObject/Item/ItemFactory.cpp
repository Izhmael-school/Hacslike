#include "ItemFactory.h"
#include"ItemEquip/ItemEquip.h"
#include"ItemHeal/ItemHeal.h"
#include"../../Save/SaveIO.h"

class BinaryReader;

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

std::unique_ptr<ItemBase> ItemFactory::CreateItemForLoad(const std::string& _id)
{
    // ロード用: デフォルト値(0)で生成し、後でLoadFromで上書き
    if (_id == "Potion_Small") {
        return std::make_unique<SmallHealItem>(VGet(0, 0, 0),
            "ポーション(小)", "体力を少し回復する", 50, 20);
    }
    else if (_id == "Potion_Middle") {
        return std::make_unique<MiddleHealItem>(VGet(0, 0, 0),
            "ポーション(中)", "体力を回復する", 80, 50);
    }
    else if (_id == "Potion_Large") {
        return std::make_unique<LargeHealItem>(VGet(0, 0, 0),
            "ポーション(大)", "体力を大幅に回復する", 110, 100);
    }
    else if (_id == "AttactPotion") {
        return std::make_unique<AttactPotion>(VGet(0, 0, 0),
            "攻撃のポーション", "2分間攻撃力が上がる", 110, 5, 120.0f);
    }
    else if (_id == "DefensePotion") {
        return std::make_unique<DefensePotion>(VGet(0, 0, 0),
            "防御のポーション", "2分間防御力が上がる", 110, 5, 120.0f);
    }
    else if (_id == "Grenade") {
        return std::make_unique<Grenade>(VGet(0, 0, 0),
            "グレネード", "3秒後に爆発する", 110, 80);
    }
    // 武器: ロード時はダミー値0で生成（LoadFromで上書きされる）
    else if (_id == "Sword_Iron") {
        return std::make_unique<ItemSword>(VGet(0, 0, 0),
            "剣", "普通の剣", 150, 0, "MeleeWeapon");
    }
    else if (_id == "Axe") {
        return std::make_unique<ItemAxe>(VGet(0, 0, 0),
            "斧", "普通の斧", 200, 0, "MeleeWeapon");
    }
    else if (_id == "Stick") {
        return std::make_unique<ItemStick>(VGet(0, 0, 0),
            "木の棒", "その辺に落ちている木の棒", 0, 0, "MeleeWeapon");
    }
    else if (_id == "Greatsword") {
        return std::make_unique<Greatsword>(VGet(0, 0, 0),
            "グレートソード", "重い!痛い!強い!最高!", 500, 0, "MeleeWeapon");
    }
    else if (_id == "Spear") {
        return std::make_unique<Spear>(VGet(0, 0, 0),
            "槍", "槍", 230, 0, "MeleeWeapon");
    }
    else if (_id == "Gun") {
        return std::make_unique<gun>(VGet(0, 0, 0),
            "銃", "銃", 160, 0, "RangedWeapon");
    }

    printfDx("ItemFactory: ID '%s' はロード対象として登録されていません。\n", _id.c_str());
    return nullptr;
}

std::unique_ptr<ItemBase> ItemFactory::CreateItem(const std::string& _id, BinaryReader& r)
{
    auto it = itemTemplates.find(_id);
    if (it != itemTemplates.end()) {
        auto item = it->second();

        item->LoadFrom(r);

        return item;
    }

    printfDx("ItemFactory: ID '%s' のアイテムが登録されていません。\n", _id.c_str());
    return nullptr;
}

int ItemFactory::GenerateEffectValueSeed(const std::string& itemId, int baseValue, int variance)
{
    // 時間ベースのシードで真の乱数を生成
    static std::mt19937 generator(static_cast<unsigned>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()
        ));

    std::uniform_int_distribution<int> distribution(baseValue - variance, baseValue + variance);
    return distribution(generator);
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

    RegisterItem("Grenade", []() {
        return std::make_unique<Grenade>(VGet(0, 0, 0),
        "グレネード", "3秒後に爆発する", 110, 80);
        });

    RegisterItem("Sword_Iron", []() {
        int effectValue = GenerateEffectValueSeed("Sword_Iron", 15, 5);
        return std::make_unique<ItemSword>(VGet(0, 0, 0),
        "剣", "普通の剣", 150, effectValue,"MeleeWeapon");
        });

    RegisterItem("Axe", []() {
        int effectValue = GenerateEffectValueSeed("Axe", 20, 10);
        return std::make_unique<ItemAxe>(VGet(0, 0, 0),
        "斧", "普通の斧", 200, effectValue, "MeleeWeapon");
        });

    RegisterItem("Stick", []() {
        return std::make_unique<ItemStick>(VGet(0, 0, 0),
        "木の棒", "そこら辺に落ちてる木の棒", 0, 5, "MeleeWeapon");
        });

    RegisterItem("Greatsword", []() {
        int effectValue = GenerateEffectValueSeed("Greatsword", 95, 25);
        return std::make_unique<Greatsword>(VGet(0, 0, 0),
        "グレートソード", "重い！強い！かっこいい！", 500, effectValue, "MeleeWeapon");
        });

    RegisterItem("Spear", []() {
        int effectValue = GenerateEffectValueSeed("Greatsword", 50,20 );
        return std::make_unique<Spear>(VGet(0, 0, 0),
        "槍", "槍", 230, effectValue, "MeleeWeapon");
        });

    RegisterItem("Gun", []() {
        return std::make_unique<gun>(VGet(0, 0, 0),
        "銃", "銃", 160, GetRand(100) + 40, "RangedWeapon");
        });
}


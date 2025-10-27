#include "ItemFactory.h"

ItemFactory& ItemFactory::Instance()
{
    // TODO: return ステートメントをここに挿入します
    static ItemFactory instance;
    return instance;
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
    if (it != itemTemplates.end())
        return it->second();

    return nullptr;
}

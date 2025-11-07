#pragma once
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include "ItemBase.h"
#include <unordered_map>


class Inventory
{
public:
    struct InventoryItem {
        std::unique_ptr<ItemBase> item;
        int quantity;

        InventoryItem(std::unique_ptr<ItemBase> _item, int _quantity = 1);
    };
/// <summary>
/// アイテム取得表示用構造体
/// </summary>
    struct GainedItemInfo {
        std::string name;
        std::string iconPath;
        int quantity;
        int timer;    // 残り表示時間
        int alpha;    // 透明度
    };
    std::vector<GainedItemInfo> gainedItems; // 取得リスト

private:
    std::vector<InventoryItem> items;
    const int baseX = 16;               // インベントリウィンドウ左上X
    const int baseY = 16;               // インベントリウィンドウ左上Y
    const int boxW = 320;              // ウィンドウ幅
    const int boxH = 360;              // ウィンドウ最大高さ（表示領域）
    const int padding = 6;
    const int titleHeight = 22;
    const int lineHeight = 28;          // 1行あたりの高さ
    const int iconSize = 20;            // アイコン表示サイズ（アイコンはロード時にこのサイズに合わせるか、DrawExtendGraphで拡縮）
    const int iconX = baseX + padding;
    const int textX = iconX + iconSize + 8;
    const int qtyRightPad = 12;
    int infoW;
    int infoH;
    int infoX;
    int infoY;
    int desPosX;
    int desPosY;
    std::string itemName;
    std::string itemDes;
    std::string itemEffect;
    int itemEffectValue;
    int itemValue;

    // 選択/スクロール状態
    int currentIndex;
    int scrollOffset;

    // Enterで出すメニュー状態
    bool menuActive;
    int menuChoice; // 0 = Use, 1 = Drop

    // アイコン読み込みキャッシュ（パス -> グラフィックハンドル）
    std::unordered_map<std::string, int> iconCache;
    // 装備用変数
    ItemBase* equippedItem = nullptr;


    // 内部ヘルパー
    void DropItemAtIndex(int idx);
    int GetMaxVisible() const;
public:
    Inventory();
    ~Inventory();

    /// <summary>
    /// アイテムを追加する
    /// </summary>
    /// <param name="newItem">取得したアイテム（unique_ptr）</param>
    void AddItem(std::unique_ptr<ItemBase> newItem);

    /// <summary>
    /// 指定されたアイテムを使用
    /// </summary>
    /// <param name="name">使用するアイテム名</param>
    void UseItem(int index);


    void Update();

    /// <summary>
    /// 装備
    /// </summary>
    /// <param name="item"></param>
    void EquipItem(ItemBase* item);  // 新規追加

    /// <summary>
    /// アイテム並び替え用
    /// </summary>
    void SortItemsByOrder();

    void Render();
    /// <summary>
    /// 入手したアイテムの取得
    /// </summary>
    /// <param name="item"></param>
    /// <param name="qty"></param>
    void OnItemGained(const ItemBase* item, int qty);

    /// <summary>
    /// アイテムの取得時のUI表示
    /// </summary>
    void AddItemRender();

public:
    inline const std::vector<InventoryItem>& GetItems() const { return items; }

};


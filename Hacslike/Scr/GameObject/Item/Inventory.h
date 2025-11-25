#pragma once
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include "ItemBase.h"
#include <unordered_map>
#include"ItemHeal/ItemHeal.h"

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

    struct ActiveItem {
        ItemBase* item; // Inventoryが持つポインタを指すだけ
    };
    
    /// <summary>
    /// ショートカット用アイテムスロット
    /// </summary>
    struct Slot {
        std::string itemID = "";  // ← これだけを保存
    };

    Slot slotUp;      // 回復
    Slot slotRight;   // 攻撃ポーション
    Slot slotLeft;    // 防御ポーション
    Slot slotDown;    // グレネード
private:
    std::vector<InventoryItem> items;
    const int baseX = 250;               // インベントリウィンドウ左上X
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

    std::vector<ItemBase*> activeItems; // 現在効果中のアイテム
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

   


    void Update(Player* player);

    /// <summary>
    /// 装備
    /// </summary>
    /// <param name="item"></param>
    void EquipItem(ItemBase* item);  // 新規追加

    /// <summary>
    /// アイテム並び替え用
    /// </summary>
    void SortItemsByOrder();

    void ItemDropRender();

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

    /// <summary>
    /// ゲーム中に行われるアイテム使用ショートカットの更新
    /// </summary>
    void UseItemShortcutUpdate();

    void UseSlot(Slot& slot);

    /// <summary>
    /// ゲーム中に行われるアイテム使用ショートカットの描画
    /// </summary>
    void UseItemShortcutRender();

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="slot"></param>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="size"></param>
    void DrawSlot(const Slot& slot, int x, int y, int size);

    /// <summary>
    /// ショートカットスロットに入れる
    /// </summary>
    /// <param name="invItem"></param>
    void AssignToShortcut(InventoryItem* invItem);

    /// <summary>
    /// IDの取得
    /// </summary>
    /// <param name="id"></param>
    /// <returns></returns>
    InventoryItem* FindItemByID(const std::string& id);

    /// <summary>
    /// 消す
    /// </summary>
    /// <param name="id"></param>
    void RemoveItemByID(const std::string& id);

    /// <summary>
    /// HPの取得
    /// </summary>
    /// <returns></returns>
    HealSize GetRequiredHealSize();

    /// <summary>
    /// IDの取得
    /// </summary>
    /// <param name="id"></param>
    /// <returns></returns>
    InventoryItem* FindHealBySize(HealSize size);

    void RefreshHealShortcut();
public:
    inline const std::vector<InventoryItem>& GetItems() const { return items; }

    public:
        InventoryItem* GetLastItem() {
            if (items.empty()) return nullptr;
            return &items.back();
        }
        // すでにある public 関数で装備できる場合はこれを使うだけでもOK

};


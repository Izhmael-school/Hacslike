#include "Inventory.h"
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <DxLib.h>
#include <algorithm>
#include <string>
#include <cstddef> // size_t
#include"../../Manager/InputManager.h"
#include"../Character/Character.h"
#include"../Character/Player/Player.h"
#include "../../Manager/AudioManager.h"


Inventory::InventoryItem::InventoryItem(std::unique_ptr<ItemBase> _item, int _quantity)
    : item(std::move(_item)), quantity(_quantity)
{
   
}

Inventory::Inventory() : currentIndex(0),
scrollOffset(0),
menuActive(false),
menuChoice(0)
{
    AudioManager::GetInstance().Load("Res/SE/ItemGet.mp3", "GetItem", false);
    AudioManager::GetInstance().Load("Res/SE/ビープ音4.mp3", "NotRemoveItem", false);
    AudioManager::GetInstance().Load("Res/SE/決定ボタンを押す2.mp3", "SelectSkill", false);
    AudioManager::GetInstance().Load("Res/SE/決定ボタンを押す38.mp3", "DecisionSkill", false);
};
Inventory::~Inventory() {
    // キャッシュされたグラフィックハンドルを解放（必要なら）
    for (auto& p : iconCache) {
        if (p.second >= 0) {
            DeleteGraph(p.second);
        }
    }
    iconCache.clear();
};

/// <summary>
/// アイテム追加処理
/// </summary>
void Inventory::AddItem(std::unique_ptr<ItemBase> newItem)
{
    AudioManager::GetInstance().PlayOneShot("GetItem");
#if _DEBUG
    printfDx("[Inventory::AddItem] this=%p BEFORE items.size=%d\n", this, (int)items.size());

#endif
    const std::string& itemName = newItem->GetName();
    const std::string& itemType = newItem->GetType();

    // 消費系（Consumable）はスタック可能
    if (itemType == "Consumable")
    {
        auto it = std::find_if(items.begin(), items.end(),
            [&](const InventoryItem& invItem)
            {
                return invItem.item->GetName() == itemName;
            });

        if (it != items.end()) {
            it->quantity++;
            #if _DEBUG
            printfDx("「%s」をストック！ x%d\n", itemName.c_str(), it->quantity);
            #endif  
            OnItemGained(it->item.get(), it->quantity);
            AssignToShortcut(&(*it));
            return;
        }
    }

    // 新規登録（装備系や初取得の消費系）
    items.emplace_back(std::move(newItem), 1);
    
    InventoryItem& addedItem = items.back();
    OnItemGained(addedItem.item.get(), addedItem.quantity);
    AssignToShortcut(&addedItem);
#if _DEBUG
    printfDx("「%s」をインベントリに追加！\n", itemName.c_str());
    printfDx("[Inventory::AddItem] this=%p AFTER items.size=%d\n", this, (int)items.size());
#endif
}



/// <summary>
/// アイテム使用処理
/// </summary>
void Inventory::UseItem(int index)
{
    if (index < 0 || index >= items.size()) return;

    auto& inv = items[index];

    if (inv.item->GetType() == "Consumable") {

        // --- すでに使用中の同一IDアイテムがあるか確認 ---
        bool isAlreadyActive = false;
        for (auto* active : activeItems)
        {
            if (active && active->GetID() == inv.item->GetID())
            {
                isAlreadyActive = true;
                break;
            }
            }

        if (isAlreadyActive)
        {
#if _DEBUG
            printfDx("「%s」はまだ効果中のため使用できません！\n", inv.item->GetName().c_str());
#endif
            return; // 効果中なら使用禁止
        }

        // --- 使用処理 ---
        inv.item->Use(); // 効果開始

        // 使用中リストに登録
        activeItems.push_back(inv.item.get());

        // ストック数を1減らす（削除はしない）
        inv.quantity--;
        if (inv.quantity < 0) inv.quantity = 0;

#if _DEBUG
        printfDx("「%s」を使用した。残り x%d\n", inv.item->GetName().c_str(), inv.quantity);
#endif
        }
    else if (inv.item->GetType() == "Equipment") {
        EquipItem(inv.item.get());
    }
    
}

/// <summary>
/// アイテムを捨てる
/// </summary>
/// <param name="idx"></param>
void Inventory::DropItemAtIndex(int idx)
{
    if (idx < 0 || idx >= (int)items.size()) return;

    InventoryItem& inv = items[idx];
    ItemBase* target = inv.item.get();   // ★ 捨てようとしているアイテムの生ポインタを取得

    // ★★★ 装備中チェック ★★★
    if (equippedItem == target)
    {
        AudioManager::GetInstance().PlayOneShot("NotRemoveItem");
#if _DEBUG
        printfDx("装備中のアイテム「%s」は捨てられません！\n", target->GetName().c_str());
#endif
        return; // 捨て処理を中断
    }

    // --- 通常の捨て処理 ---
    inv.quantity--;

#if _DEBUG
    printfDx("「%s」を捨てた。残り x%d\n", target->GetName().c_str(), inv.quantity);
#endif

    if (inv.quantity <= 0) {
        items.erase(items.begin() + idx);

        // カーソルが範囲外なら調整
        if (currentIndex >= (int)items.size()) {
            currentIndex = (std::max)(0, (int)items.size() - 1);
        }
    }
}

int Inventory::GetMaxVisible() const
{
    int maxVisible = (boxH - titleHeight - padding) / lineHeight;
    if (maxVisible <= 0) maxVisible = 1;
    return maxVisible;
}



/// <summary>
/// 更新
/// </summary>
void Inventory::Update(Player* player)
{
    if (player->GetIsMenuSelected() == true) {

        InputManager* input = &InputManager::GetInstance();
        // 並び替え
        if (input->IsKeyDown(KEY_INPUT_R) || input->IsButtonDown(XINPUT_GAMEPAD_Y))
        {
            SortItemsByOrder();
            currentIndex = 0;
        }
        // 上
        if (input->IsKeyDown(KEY_INPUT_UP) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_UP)) {
            if (!menuActive) {
                if (!items.empty()) {
                    currentIndex = (std::max)(0, currentIndex - 1);
                    // スクロール調整
                    if (currentIndex < scrollOffset) scrollOffset = currentIndex;
                    AudioManager::GetInstance().PlayOneShot("SelectSkill");
                }
            }
            else {
                // メニュー内移動（上下で切り替える）
                menuChoice = (menuChoice + 1) % 2; // 0/1 切替
                AudioManager::GetInstance().PlayOneShot("SelectSkill");
            }
        }

        // 下
        if (input->IsKeyDown(KEY_INPUT_DOWN) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN)) {
            if (!menuActive) {
                if (!items.empty()) {
                    currentIndex = (std::min)((int)items.size() - 1, currentIndex + 1);
                    int maxVisible = GetMaxVisible();
                    if (currentIndex >= scrollOffset + maxVisible) {
                        scrollOffset = currentIndex - (maxVisible - 1);
                        if (scrollOffset < 0) scrollOffset = 0;
                    }
                    AudioManager::GetInstance().PlayOneShot("SelectSkill");
                }
            }
            else {
                // メニュー内移動
                menuChoice = (menuChoice + 1) % 2;
                AudioManager::GetInstance().PlayOneShot("SelectSkill");
            }
        }

        // Enter
        if (input->IsKeyDown(KEY_INPUT_RETURN) || input->IsButtonDown(XINPUT_GAMEPAD_B)) {
            if (!menuActive) {
                // メニューを開く（アイテムが存在するとき）
                if (!items.empty()) {
                    AudioManager::GetInstance().PlayOneShot("DecisionSkill");
                    menuActive = true;
                    menuChoice = 0; // デフォルトは Use
                }
            }
            else {
                // メニューで選択を実行
                if (currentIndex >= 0 && currentIndex < (int)items.size()) {
                    InventoryItem& inv = items[currentIndex];
                    const std::string name = inv.item->GetName();
                    if (menuChoice == 0) {
                        // Use
                        UseItem(currentIndex);
                       

                        
                    }
                    else {
                        // Drop
                        DropItemAtIndex(currentIndex);
                       

                    }
                }
                menuActive = false;
            }
        }

        // Escape キャンセル
        if (input->IsKeyDown(KEY_INPUT_ESCAPE) || input->IsButtonDown(XINPUT_GAMEPAD_A)) {
            if (menuActive) {
                menuActive = false;
                AudioManager::GetInstance().PlayOneShot("DecisionSkill");
            }
        }

        // 左右でメニューの切替（EnterでOKする方式の場合は不要だが、対応）
        if (menuActive) {
            if (input->IsKeyDown(KEY_INPUT_LEFT) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)) {
                AudioManager::GetInstance().PlayOneShot("SelectSkill");
                menuChoice = (std::max)(0, menuChoice - 1);
            }
            if (input->IsKeyDown(KEY_INPUT_RIGHT) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) {
                AudioManager::GetInstance().PlayOneShot("SelectSkill");
                menuChoice = (std::min)(1, menuChoice + 1);
            }
        }
    }
  
    // --- アイテム効果更新 ---
    for (int i = 0; i < (int)activeItems.size(); )
    {
        ItemBase* item = activeItems[i];
        if (item)
        {
            item->Update(); // 効果時間を進める
           


            // 効果終了判定
            if (item->IsEffectFinished())
            {
#if _DEBUG
                printfDx("「%s」の効果が切れた。\n", item->GetName().c_str());
#endif
                // activeItemsから削除
                activeItems.erase(activeItems.begin() + i);

                // 同じIDのアイテムがInventory内にあれば、
                // ストックが0ならここで完全削除
                for (int j = 0; j < (int)items.size(); ++j)
                {
                    if (items[j].item->GetID() == item->GetID() && items[j].quantity <= 0)
                    {
                        items.erase(items.begin() + j);
                        break;
                    }
                }

                continue; // eraseしたのでインデックス進めない
        }
    }
        ++i;
}
}

/// <summary>
/// アイテム装備
/// </summary>
/// <param name="item"></param>
void Inventory::EquipItem(ItemBase* item)
{
    if (!item ) return;

    if (equippedItem == item) {
#if _DEBUG
        printfDx("すでに「%s」を装備中です。\n", item->GetName().c_str());
#endif
        return;
    }

    if (equippedItem) {
#if _DEBUG
        printfDx("「%s」から「%s」に装備を切り替えました。\n",
            equippedItem->GetName().c_str(), item->GetName().c_str());
#endif
        equippedItem->UnEquip();
    }
    else {
#if _DEBUG
        printfDx("「%s」を装備しました！\n", item->GetName().c_str());
#endif
    }

    equippedItem = item; // equippedItem は ItemBase* でOK
    item->Use();
}

/// <summary>
/// アイテム格納マップ
/// </summary>
static const std::unordered_map<std::string, std::string> itemEffectMap = {
    {"ポーション(小)", "回復量"},
    {"ポーション(中)", "回復量"},
    {"ポーション(大)", "回復量"},
    {"攻撃のポーション","効力"},
    {"防御のポーション","効力"},
    {"グレネード","威力"},
    {"剣", "攻撃力"},
    {"斧", "攻撃力"},
    {"木の棒","攻撃力"},
    {"グレートソード","攻撃力"},
    {"槍","攻撃力"},
    {"銃","攻撃力"},
    // ここに増やすだけでOK！
};

// 並び替え順を保持（登録順）
static const std::vector<std::string> itemOrder = {
    "ポーション(小)",
    "ポーション(中)",
    "ポーション(大)",
    "攻撃のポーション",
    "防御のポーション",
    "グレネード",
    "木の棒",
    "剣",
    "斧",
    "グレートソード",
    "槍",
    "銃",
};

/// <summary>
/// アイテムの並び替え用
/// </summary>
void Inventory::SortItemsByOrder()
{
   
    std::sort(items.begin(), items.end(), [](const InventoryItem& a, const InventoryItem& b) {
        auto findIndex = [](const std::string& name) {
            auto it = std::find(itemOrder.begin(), itemOrder.end(), name);
            if (it != itemOrder.end()) return static_cast<int>(std::distance(itemOrder.begin(), it));
            return static_cast<int>(itemOrder.size());
        };
        return findIndex(a.item->GetName()) < findIndex(b.item->GetName());
        });
}

void Inventory::ItemDropRender()
{
    for (int i = 0; i < (int)activeItems.size(); i++ )
    {
        ItemBase* item = activeItems[i];
        if (item)
        {

            item->Render();
        }
    }
}

/// <summary>
/// 描画処理
/// </summary>
void Inventory::Render()
{
    // レイアウト定数
    const int iconX = baseX + padding + 20;
    const int textX = iconX + iconSize + 4;
    const int qtyRightPad = 12;

    // 背景
    const int bgColor = black;
    const int borderColor = GetColor(200, 200, 200);
    DrawBox(baseX, baseY, baseX + boxW, baseY + boxH, bgColor, TRUE);
    DrawBox(baseX, baseY, baseX + boxW, baseY + titleHeight, borderColor, FALSE);

    // タイトル
    const char* title = "持ち物";
    DrawString(baseX + padding, baseY + 2, title, white);

    int maxVisible = GetMaxVisible();
    int startIdx = scrollOffset;
    if (startIdx < 0) startIdx = 0;
    int endIdx = static_cast<int>(items.size());
    if (endIdx - startIdx > maxVisible) endIdx = startIdx + maxVisible;

    int y = baseY + titleHeight + padding;

    for (int i = startIdx; i < endIdx; ++i)
    {
        const InventoryItem& inv = items[i];
        const std::string& name = inv.item->GetName();
        const std::string& type = inv.item->GetType();
        int quantity = inv.quantity;

        // 選択行ハイライト
        bool isSelected = (i == currentIndex);

        if (isSelected) {
            // 枠とマーカー
            int selColor = yellow;
            DrawBox(baseX + 1, y, baseX + boxW - 1, y + lineHeight - 1, selColor, FALSE);
            DrawString(textX - 40, y + 4, "->", selColor);

            // 現在のアイテム情報
            const InventoryItem& curInv = items[currentIndex];
            const std::string curName = curInv.item->GetName();

            // ===== 説明ボックス描画 =====
            infoW = 240;
            infoH = 80;
            infoX = baseX + boxW + 10;
            infoY = y - 4;

            DrawBox(infoX, infoY, infoX + infoW, infoY + infoH, GetColor(40, 40, 40), TRUE);
            DrawBox(infoX, infoY, infoX + infoW, infoY + infoH, GetColor(200, 200, 200), FALSE);

            desPosX = infoX + 12;
            desPosY = infoY + 10;

            // ===== アイテム情報取得 =====
            itemName = curInv.item->GetName();
            itemDes = curInv.item->GetDescription();
            itemEffectValue = curInv.item->GetEffectValue();
            itemValue = curInv.item->GetValue();

            // ===== 効果タイプ判定（マップで管理） =====
            auto it = itemEffectMap.find(curName);
            if (it != itemEffectMap.end()) {
                itemEffect = it->second;
            }
            else {
                itemEffect = "効果不明";
            }

            // ===== 描画 =====
            DrawString(desPosX, desPosY, itemName.c_str(), white);
            DrawString(desPosX, desPosY + 20, itemDes.c_str(), white);
            DrawFormatString(desPosX + 5, desPosY + infoH - 30, white,
                "%s : %d  価値 : %d", itemEffect.c_str(), itemEffectValue, itemValue);
        }

        // アイコン描画
        int targetH = lineHeight - 4;
        int iconY = y + (lineHeight - targetH) / 2;
        int iconHandle = -1;

        std::string iconPath = inv.item->GetItemIcon();
        if (!iconPath.empty()) {
            auto itc = iconCache.find(iconPath);
            if (itc != iconCache.end()) {
                iconHandle = itc->second;
            }
            else {
                int h = LoadGraph(iconPath.c_str());
                if (h >= 0) {
                    iconCache.emplace(iconPath, h);
                    iconHandle = h;
                }
                else {
                    iconCache.emplace(iconPath, -1);
                    iconHandle = -1;
                }
            }
        }

        if (iconHandle >= 0) {
            int gw = 0, gh = 0;
            GetGraphSize(iconHandle, &gw, &gh);
            if (gw > 0 && gh > 0) {
                int destW = static_cast<int>((float)gw * ((float)targetH / (float)gh) + 0.5f);
                DrawExtendGraph(iconX, iconY, iconX + destW, iconY + targetH, iconHandle, TRUE);
            }
            else {
                DrawGraph(iconX, iconY, iconHandle, TRUE);
            }
        }
        else {
            DrawBox(iconX, iconY, iconX + targetH, iconY + targetH, GetColor(120, 120, 120), TRUE);
        }

        // 名前色分け
        int nameColor = GetColor(220, 220, 220);
        if (type == "Consumable") {
            nameColor = palegreen;
        }
        else if (type == "Equipment") {
            nameColor = white;
        }

        DrawString(textX, y + 4, name.c_str(), nameColor);

        // 装備中マーク
        if (equippedItem && equippedItem == inv.item.get()) {
            DrawString(baseX + 3, y + 4, "E", cyan);
        }

        // ★ 使用中アイテム（効果時間継続）のマーク S を表示 ★
        bool isActiveEffect = false;
        for (auto* active : activeItems) {
            if (active == inv.item.get()) {
                isActiveEffect = true;
                break;
            }
        }
        if (isActiveEffect) {
            DrawString(baseX + 3, y + 4, "S", cyan);   // ← 黄系
        }

        // 数量
        if (quantity > 1) {
            char buf[32];
            snprintf(buf, sizeof(buf), "x%d", quantity);
            int qtyX = baseX + boxW - qtyRightPad - 48;
            DrawString(qtyX, y, buf, GetColor(255, 255, 192));
        }

        y += lineHeight;
    }

    // ===== アイテム選択ポップアップ =====
    if (menuActive && currentIndex >= 0 && currentIndex < (int)items.size()) {
        const int popupW = 300;
        const int popupH = 56;

        bool selectedIsVisible = (currentIndex >= startIdx && currentIndex < endIdx);
        int px = baseX + (boxW - popupW) / 2;
        int py = baseY + (boxH - popupH) / 2;

        if (selectedIsVisible) {
            int selRelative = currentIndex - startIdx;
            int selY = baseY + titleHeight + padding + selRelative * lineHeight;
            px = textX;
            py = selY + lineHeight;

            int rightLimit = baseX + boxW - padding;
            if (px + popupW > rightLimit) px = rightLimit - popupW;
            int leftLimit = baseX + padding;
            if (px < leftLimit) px = leftLimit;

            int bottomLimit = baseY + boxH - padding;
            if (py + popupH > bottomLimit) py = selY - popupH;
            if (py < baseY + titleHeight + padding) {
                px = baseX + (boxW - popupW) / 2;
                py = baseY + (boxH - popupH) / 2;
            }
        }

        DrawBox(px, py, px + popupW, py + popupH, darkGray, TRUE);
        DrawBox(px, py, px + popupW, py + popupH, white, FALSE);

        // メニュー内容
        const InventoryItem& curInv = items[currentIndex];
        const std::string curType = curInv.item->GetType();
        const char* opt0 = "使う";
        if (curType == "Equipment") opt0 = "装備";
        else if (curType == "Consumable") opt0 = "使う";

        const char* opt1 = "捨てる";

        int optX = px + 12;
        int optY = py + 12;
        int col0 = (menuChoice == 0) ? GetColor(255, 255, 0) : GetColor(220, 220, 220);
        DrawString(optX, optY, opt0, col0);
        int col1 = (menuChoice == 1) ? GetColor(255, 255, 0) : GetColor(220, 220, 220);
        DrawString(optX + 120, optY, opt1, col1);
        DrawString(px + 8, py + popupH - 18, "Enter: 決定  Esc: キャンセル", GetColor(180, 180, 180));
    }
}

/// <summary>
/// 入手したアイテムの取得
/// </summary>
/// <param name="item"></param>
/// <param name="qty"></param>
void Inventory::OnItemGained(const ItemBase* item, int qty)
{
    GainedItemInfo info;
    info.name = item->GetName();
    info.iconPath = item->GetItemIcon();
    info.quantity = qty;
    info.timer = 180;  // 約3秒
    info.alpha = 255;

    gainedItems.push_back(info);
}

/// <summary>
/// アイテムの取得時のUI表示
/// </summary>
void Inventory::AddItemRender()
{
    if (gainedItems.empty()) return;

    const int baseX = WINDOW_WIDTH - 350;
    const int baseY = WINDOW_HEIGHT - 400; // 下部寄せ表示
    const int iconSize = 28;
    const int lineH = 36;
    const int padding = 6;
    const int boxW = 320;

    int y = baseY;

    for (auto& g : gainedItems)
    {
        // タイマー減少・フェード
        g.timer--;
        if (g.timer < 60) {
            g.alpha = static_cast<int>(255.0f * (g.timer / 60.0f));
            if (g.alpha < 0) g.alpha = 0;
        }

        // 背景と枠
        int bgCol = GetColor(30, 30, 30);
        int frameCol = GetColor(180, 180, 180);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, g.alpha);
        DrawBox(baseX, y, baseX + boxW, y + lineH, bgCol, TRUE);
        DrawBox(baseX, y, baseX + boxW, y + lineH, frameCol, FALSE);

        // アイコン描画
        int iconHandle = -1;
        auto itc = iconCache.find(g.iconPath);
        if (itc != iconCache.end()) {
            iconHandle = itc->second;
        }
        else {
            int h = LoadGraph(g.iconPath.c_str());
            if (h >= 0) {
                iconCache.emplace(g.iconPath, h);
                iconHandle = h;
            }
            else {
                iconCache.emplace(g.iconPath, -1);
                iconHandle = -1;
            }
        }

        if (iconHandle >= 0) {
            DrawExtendGraph(baseX + padding, y + 4, baseX + padding + iconSize, y + 4 + iconSize, iconHandle, TRUE);
        }
        else {
            // アイコンなし時の代替
            DrawBox(baseX + padding, y + 4, baseX + padding + iconSize, y + 4 + iconSize, GetColor(100, 100, 100), TRUE);
        }

        // 名前＋数量を描画
        char buf[128];
        snprintf(buf, sizeof(buf), "%s", g.name.c_str());
        DrawString(baseX + padding + iconSize + 8, y + 8, buf, GetColor(255, 255, 255));

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        y -= (lineH + 4); // 上方向に積み上げていく
    }

    // タイマー切れで削除
    gainedItems.erase(
        std::remove_if(gainedItems.begin(), gainedItems.end(),
            [](const GainedItemInfo& g) { return g.timer <= 0; }),
        gainedItems.end());
}

void Inventory::UseItemShortcutUpdate()
{
    InputManager* input = &InputManager::GetInstance();
    if (input->IsButtonDown(XINPUT_GAMEPAD_DPAD_UP))    UseSlot(slotUp);
    else if (input->IsButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) UseSlot(slotRight);
    else if (input->IsButtonDown(XINPUT_GAMEPAD_DPAD_LEFT))  UseSlot(slotLeft);
    else if (input->IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN))  UseSlot(slotDown);
}

void Inventory::UseSlot(Slot& slot)
{
    if (slot.itemID.empty()) return;

    // ★ 並び替えてもここで現在の位置を見つけられる
    InventoryItem* inv = FindItemByID(slot.itemID);
    if (!inv) {
#if _DEBUG
        printfDx("ショートカット [%s] がインベントリに存在しません\n",
            slot.itemID.c_str());
#endif
        slot.itemID = "";
        return;
    }

    ItemBase* item = inv->item.get();

    // ---- 効果中チェック ----
    if (item->GetType() == "Consumable")
    {
        for (auto* active : activeItems)
        {
            if (active->GetID() == item->GetID())
            {
                AudioManager::GetInstance().PlayOneShot("NotRemoveItem");
                return;
            }
        }
    }

    // ---- 使用 ----
    if (item->GetType() == "Consumable")
    {
        item->Use();
        activeItems.push_back(item);

        inv->quantity--;

        if (inv->quantity < 0) 
        {
            inv->quantity = 0;
            RemoveItemByID(item->GetID());
            slot.itemID.clear();      // ← 0個になったらショートカット解除
        }
    }
    else if (item->GetType() == "Equipment")
    {
        EquipItem(item);
    }
}

void Inventory::UseItemShortcutRender()
{
    int x = WINDOW_WIDTH-130, y = WINDOW_HEIGHT-100; // 右下の中心位置
    int s = 48;

    DrawSlot(slotUp, x, y - 60, s);
    DrawSlot(slotRight, x + 60, y, s);
    DrawSlot(slotLeft, x - 60, y, s);
    DrawSlot(slotDown, x, y + 60, s);
}

void Inventory::DrawSlot(const Slot& slot, int x, int y, int size)
{
    DrawBox(x - size / 2, y - size / 2, x + size / 2, y + size / 2, GetColor(80, 80, 80), FALSE);

    if (slot.itemID.empty()) return;

    InventoryItem* inv = FindItemByID(slot.itemID);
    if (!inv) return;

    // アイコン
    int iconHandle = -1;

    std::string iconPath = inv->item->GetItemIcon();
    if (!iconPath.empty()) {
        auto itc = iconCache.find(iconPath);
        if (itc != iconCache.end()) {
            iconHandle = itc->second;
        }
        else {
            int h = LoadGraph(iconPath.c_str());
            if (h >= 0) {
                iconCache.emplace(iconPath, h);
                iconHandle = h;
            }
            else {
                iconCache.emplace(iconPath, -1);
                iconHandle = -1;
            }
        }
    }
    DrawExtendGraph(x - size/2, y - size/2, x + size/2, y + size/2, iconHandle, TRUE);

    // 残数（右下）
    DrawFormatString(x + size/2 - 12, y + size/2 - 12, GetColor(255,255,255), "%d", inv->quantity);
    bool isActiveEffect = false;
    for (auto* active : activeItems) {
        if (active == inv->item.get()) {
            isActiveEffect = true;
            break;
        }
    }
    if (isActiveEffect) {
        DrawString(x + size / 2 - 45, y + size / 2 - 12, "S", cyan);   // ← 黄系
    }
}

void Inventory::AssignToShortcut(InventoryItem* invItem)
{
    const std::string id = invItem->item->GetID();   // ← ItemBase の一意なID

    if (invItem->item->GetItemType() == ItemType::Heal) {
        HealSize need = GetRequiredHealSize();
        InventoryItem* best = FindHealBySize(need);

        if (best) {
            slotUp.itemID = best->item->GetID();
        }
        else {
            // 該当サイズがない場合 → 今持っている回復の中で一番大きいのを使うなど
            slotUp.itemID = invItem->item->GetID();
        }
        return;
    }
    else if (invItem->item->GetItemType() == ItemType::AttackPotion) {
        slotRight.itemID = id;
    }
    else if (invItem->item->GetItemType() == ItemType::DefensePotion) {
        slotLeft.itemID = id;
    }
    else if (invItem->item->GetItemType() == ItemType::Grenade) {
        slotDown.itemID = id;
    }
}

Inventory::InventoryItem* Inventory::FindItemByID(const std::string& id)
{
    for (auto& item : items)
    {
        if (item.item->GetID() == id)
            return &item;
    }
    return nullptr;
}

void Inventory::RemoveItemByID(const std::string& id)
{
    for (size_t i = 0; i < items.size(); i++)
    {
        if (items[i].item->GetID() == id)
        {
            items.erase(items.begin() + i);
            return;
        }
    }
}

HealSize Inventory::GetRequiredHealSize()
{
    
    float hpRate = static_cast<float>(Player::GetInstance()->GetHp()) / Player::GetInstance()->GetMaxHp();

    if (hpRate < 0.30f)  return HealSize::Large;
    if (hpRate < 0.65f)  return HealSize::Medium;
    return HealSize::Small;
}

Inventory::InventoryItem* Inventory::FindHealBySize(HealSize size)
{
    for (auto& inv : items)
    {
        if (!inv.item) continue;
        if (inv.item->GetItemType() != ItemType::Heal) continue;

        if (inv.item->GetHealType() == size && inv.quantity > 0)
            return &inv;
    }
    return nullptr;
}

void Inventory::RefreshHealShortcut()
{
    HealSize need = GetRequiredHealSize();
    InventoryItem* best = FindHealBySize(need);

    if (best) {
        slotUp.itemID = best->item->GetID();
    }
    else {
        // fallback: 所持している中で最大回復を使う
        InventoryItem* largest = nullptr;
        for (auto& inv : items) {
            if (!inv.item) continue;
            if (inv.item->GetItemType() != ItemType::Heal) continue;

            if (!largest || inv.item->GetHealType() > largest->item->GetHealType())
                largest = &inv;
        }

        if (largest)
            slotUp.itemID = largest->item->GetID();
    }
}

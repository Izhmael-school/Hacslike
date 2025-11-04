#include "Inventory.h"
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <DxLib.h>
#include <algorithm>
#include <string>
#include <cstddef> // size_t
#include"../../Manager/InputManager.h"

Inventory::InventoryItem::InventoryItem(std::unique_ptr<ItemBase> _item, int _quantity)
    : item(std::move(_item)), quantity(_quantity)
{
}

Inventory::Inventory() : currentIndex(0),
scrollOffset(0),
menuActive(false),
menuChoice(0)
{
  
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
            printfDx("「%s」をストック！ x%d\n", itemName.c_str(), it->quantity);
            
            OnItemGained(it->item.get(), it->quantity);
            return;
        }
    }

    // 新規登録（装備系や初取得の消費系）
    items.emplace_back(std::move(newItem), 1);
    
    InventoryItem& addedItem = items.back();
    OnItemGained(addedItem.item.get(), addedItem.quantity);
#if _DEBUG
    printfDx("「%s」をインベントリに追加！\n", itemName.c_str());
    printfDx("[Inventory::AddItem] this=%p AFTER items.size=%d\n", this, (int)items.size());
#endif
}



/// <summary>
/// アイテム使用処理
/// </summary>
void Inventory::UseItem(const std::string& name)
{
    auto it = std::find_if(items.begin(), items.end(),
        [&](const InventoryItem& invItem)
        {
            return invItem.item->GetName() == name;
        });

    if (it == items.end()) {
#if _DEBUG
        printfDx("アイテム「%s」は所持していません！\n", name.c_str());
#endif
        return;
    }

    // Use()関数を呼び出す（ItemBase側に実装されている前提）
    it->item->Use();
    it->quantity--;

    if (it->quantity <= 0) {
#if _DEBUG
        printfDx("「%s」を使い切った！\n", name.c_str());
#endif
        items.erase(it);
    }
}

void Inventory::DropItemAtIndex(int idx)
{
    if (idx < 0 || idx >= (int)items.size()) return;
    InventoryItem& inv = items[idx];
    // ここでワールドにドロップする処理を入れることもできます（未実装）
    inv.quantity--;
#if _DEBUG
    printfDx("「%s」を捨てた。残り x%d\n", inv.item->GetName().c_str(), inv.quantity);
#endif

    if (inv.quantity <= 0) {
        items.erase(items.begin() + idx);
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

void Inventory::Update()
{
    char keyState[256];
    GetHitKeyStateAll(keyState);
    InputManager* input = &InputManager::GetInstance();
    // 上
    if (input->IsKeyDown(KEY_INPUT_UP) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_UP)) {
        if (!menuActive) {
            if (!items.empty()) {
                currentIndex = (std::max)(0, currentIndex - 1);
                // スクロール調整
                if (currentIndex < scrollOffset) scrollOffset = currentIndex;
            }
        }
        else {
            // メニュー内移動（上下で切り替える）
            menuChoice = (menuChoice + 1) % 2; // 0/1 切替
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
            }
        }
        else {
            // メニュー内移動
            menuChoice = (menuChoice + 1) % 2;
        }
    }

    // Enter
    if (input->IsKeyDown(KEY_INPUT_RETURN) || input->IsButtonDown(XINPUT_GAMEPAD_B)) {
        if (!menuActive) {
            // メニューを開く（アイテムが存在するとき）
            if (!items.empty()) {
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
                    UseItem(name);
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
        }
    }

    // 左右でメニューの切替（EnterでOKする方式の場合は不要だが、対応）
    if (menuActive) {
        if (input->IsKeyDown(KEY_INPUT_LEFT) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)) {
            menuChoice = (std::max)(0, menuChoice - 1);
        }
        if (input->IsKeyDown(KEY_INPUT_RIGHT) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) {
            menuChoice = (std::min)(1, menuChoice + 1);
        }
    }

   
}

/// <summary>
/// 描画処理
/// </summary>
void Inventory::Render()
{
    // レイアウト定数はヘッダのとおり
    const int iconX = baseX + padding;
    const int textX = iconX + iconSize + 8;
    const int qtyRightPad = 12;

    // 背景（半透明っぽい暗めの矩形）
    const int bgColor = black;
    const int borderColor = GetColor(200, 200, 200);
    DrawBox(baseX, baseY, baseX + boxW, baseY + boxH, bgColor, TRUE);
    DrawBox(baseX, baseY, baseX + boxW, baseY + titleHeight, borderColor, FALSE); // タイトル上の罫線

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

        // 選択行のハイライト
        bool isSelected = (i == currentIndex);

        if (isSelected) {
            // 枠（黄色）
            int selColor = yellow;
            DrawBox(baseX + 1, y, baseX + boxW - 1, y + lineHeight - 1, selColor, FALSE);
            // 簡易マーカー
            DrawString(baseX + 2, y + 4, ">", selColor);
            //現在選択中のアイテム
            const InventoryItem& curInv = items[currentIndex];
            const std::string curName = curInv.item->GetName();
            //選択中の真横にアイテムの説明用のボックスを表示
             // ===== 説明ボックスを右横に描画 =====
            infoW = 240;         // 幅
            infoH = 80;          // 高さ
            infoX = baseX + boxW + 10;  // インベントリの右横に表示
            infoY = y - 4;       // 選択行の高さに合わせて配置（少し上に）
            // 背景と枠線
            DrawBox(infoX, infoY, infoX + infoW, infoY + infoH, GetColor(40, 40, 40), TRUE);
            DrawBox(infoX, infoY, infoX + infoW, infoY + infoH, GetColor(200, 200, 200), FALSE);
            desPosX = infoX + 12;
            desPosY = infoY + 10;
            itemName = ""; // デフォルト
            itemDes = "";
            itemEffect = "";
            if (curName == "ポーション(小)") {
                itemName = curInv.item->GetName();
                itemDes = curInv.item->GetDescription();
                itemEffectValue = curInv.item->GetEffectValue();
                itemValue = curInv.item->GetValue();
                itemEffect = "回復量";
            }
            else if (curName == "剣") {
                itemName = curInv.item->GetName();
                itemDes = curInv.item->GetDescription();
                itemEffectValue = curInv.item->GetEffectValue();
                itemValue = curInv.item->GetValue();
                itemEffect = "攻撃力";
            }
            else if (curName == "斧") {
                itemName = curInv.item->GetName();
                itemDes = curInv.item->GetDescription();
                itemEffectValue = curInv.item->GetEffectValue();
                itemValue = curInv.item->GetValue();
                itemEffect = "攻撃力";
            }
            DrawString(desPosX, desPosY, itemName.c_str(), white);
            DrawString(desPosX, desPosY + 20, itemDes.c_str(), white);
            DrawFormatString(desPosX + 5, desPosY + infoH - 30, white, "%s : %d  価値 : %d", itemEffect,itemEffectValue, itemValue);
        }

        // アイコンをテキスト高さに合わせて描画するループ内
        int targetH = lineHeight - 4; // 文字と少し余白を入れる（調整可）
        int iconY = y + (lineHeight - targetH) / 2; // 縦中央に合わせる
        // アイコン取得（ItemBase::GetItemIcon() は画像パスを返す想定）
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
            GetGraphSize(iconHandle, &gw, &gh); // 元画像の幅・高さを取得
            if (gw > 0 && gh > 0) {
                // 縦高さに合わせて横幅を算出（アスペクト比維持）
                int destW = static_cast<int>((float)gw * ((float)targetH / (float)gh) + 0.5f);
                // DrawExtendGraph: 左上(x1,y1) 右下(x2,y2)
                DrawExtendGraph(iconX, iconY, iconX + destW, iconY + targetH, iconHandle, TRUE);
            }
            else {
                // サイズ取得できなければそのまま描画
                DrawGraph(iconX, iconY, iconHandle, TRUE);
            }
        }
        else {
            // アイコン無しの代替表示
            DrawBox(iconX, iconY, iconX + targetH, iconY + targetH, GetColor(120, 120, 120), TRUE);
        }

        // 名前の色分け（例: 装備系は水色、消費は緑）
        int nameColor = GetColor(220, 220, 220);
        if (type == "Consumable") {
            nameColor = palegreen; // palegreen
        }
        else if (type == "Equipment") {
            nameColor = white;
        }

        // アイテム名
        DrawString(textX, y, name.c_str(), nameColor);

        // 数量（2以上のみ表示）
        if (quantity > 1) {
            char buf[32];
            snprintf(buf, sizeof(buf), "x%d", quantity);
            // 右寄せ表示（簡易）。DrawString の幅取得を行うならより正確に右寄せできます。
            int qtyX = baseX + boxW - qtyRightPad - 48; // 48はx幅の目安（フォントによって調整）
            DrawString(qtyX, y, buf, GetColor(255, 255, 192));
        }

        y += lineHeight;
    }

    //アイテム選択画面の表示
    // メニューがアクティブならポップアップ表示（選択行のすぐ下に出す）
    if (menuActive && currentIndex >= 0 && currentIndex < (int)items.size()) {
        const int popupW = 300;
        const int popupH = 56;

        // 選択行が現在の表示範囲内かを判定
        bool selectedIsVisible = (currentIndex >= startIdx && currentIndex < endIdx);

        int px = baseX + (boxW - popupW) / 2; // デフォルト (中央)
        int py = baseY + (boxH - popupH) / 2;

        if (selectedIsVisible) {
            // 選択行の相対Yを計算（startIdx からのオフセット）
            int selRelative = currentIndex - startIdx;
            int selY = baseY + titleHeight + padding + selRelative * lineHeight;

            // デフォルトは選択行のすぐ下に表示
            px = textX; // テキスト開始位置の下に出す（任意で調整）
            py = selY + lineHeight; // 選択行の直下

            // 右端にはみ出す場合は左にシフト
            int rightLimit = baseX + boxW - padding;
            if (px + popupW > rightLimit) {
                px = rightLimit - popupW;
            }
            // 左端より左にならないようにクリップ
            int leftLimit = baseX + padding;
            if (px < leftLimit) px = leftLimit;

            // 下にはみ出す場合は選択行の上に表示
            int bottomLimit = baseY + boxH - padding;
            if (py + popupH > bottomLimit) {
                py = selY - popupH;
            }
            // 上限もチェック（タイトル領域に被らないようにする）
            int topLimit = baseY + titleHeight + padding - popupH; // ここは微調整可
            if (py < baseY + titleHeight + padding) {
                // フォールバック：中央表示
                px = baseX + (boxW - popupW) / 2;
                py = baseY + (boxH - popupH) / 2;
            }
        }

        DrawBox(px, py, px + popupW, py + popupH, darkGray, TRUE);
        DrawBox(px, py, px + popupW, py + popupH, white, FALSE);

        // メニュー項目
        // 現在選択しているアイテムの種類に応じてラベルを切り替える
        const InventoryItem& curInv = items[currentIndex];
        const std::string curType = curInv.item->GetType();
        const char* opt0 = "使う"; // デフォルト
        if (curType == "Equipment") {
            opt0 = "装備";
        }
        else if (curType == "Consumable") {
            opt0 = "使う";
        } // 他のタイプがあればここで追加可能

        const char* opt1 = "捨てる";

        int optX = px + 12;
        int optY = py + 12;
        // opt0 (使う/装備)
        int col0 = (menuChoice == 0) ? GetColor(255, 255, 0) : GetColor(220, 220, 220);
        DrawString(optX, optY, opt0, col0);
        // Drop
        int col1 = (menuChoice == 1) ? GetColor(255, 255, 0) : GetColor(220, 220, 220);
        DrawString(optX + 120, optY, opt1, col1);

        // ヒント
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
    const int baseY = WINDOW_HEIGHT - 100; // 下部寄せ表示
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

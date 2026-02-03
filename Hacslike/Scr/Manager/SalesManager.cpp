#include "SalesManager.h"
#include "../GameObject/Item/Inventory.h" 
#include "../GameObject/Character/Player/Player.h"
#include "../Manager/InputManager.h"
#include <string>
#include <algorithm>

SalesManager::SalesManager()
    : isActive(false)
    , state(State::Selecting)
    , targetInventory(nullptr)
    , currentIndex(0)
    , confirmIndex(1)
    , scrollIndex(0)
    , maxDisplay(6) {
}

void SalesManager::StartSellScene(Inventory* inv) {
    if (!inv) return;
    targetInventory = inv;
    isActive = true;
    state = State::Selecting;
    currentIndex = 0;
    scrollIndex = 0;
    confirmIndex = 1;
    SetMouseDispFlag(TRUE);
}

void SalesManager::Update() {
    if (!isActive || !targetInventory) return;

    auto& input = InputManager::GetInstance();
    const auto& items = targetInventory->GetItems();

    // アイテムが空の場合の終了処理
    if (items.empty()) {
        if (input.IsKeyDown(KEY_INPUT_ESCAPE) || input.IsButtonDown(XINPUT_GAMEPAD_A)) {
            isActive = false;
            SetMouseDispFlag(FALSE);
        }
        return;
    }

    // --- 入力デバイス情報の取得 ---
    int mouseX, mouseY;
    GetMousePoint(&mouseX, &mouseY);
    int wheel = GetMouseWheelRotVol(); // マウスホイール
    float stickY = input.IsJoypadSthick("L_Vertical");
    float stickX = input.IsJoypadSthick("L_Horizontal");

    if (state == State::Selecting) {
        // --- 1. スクロール処理 (マウスホイール) ---
        if (wheel > 0) {
            scrollIndex = (std::max)(0, scrollIndex - 1);
        }
        else if (wheel < 0) {
            if (scrollIndex + maxDisplay < (int)items.size()) scrollIndex++;
        }

        // --- 2. 選択移動 (キーボード / 十字キー / スティック) ---
        if (input.IsKeyDown(KEY_INPUT_UP) || input.IsButtonDown(XINPUT_GAMEPAD_DPAD_UP) || stickY > 0.5f) {
            currentIndex = (currentIndex <= 0) ? (int)items.size() - 1 : currentIndex - 1;
        }
        if (input.IsKeyDown(KEY_INPUT_DOWN) || input.IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN) || stickY < -0.5f) {
            currentIndex = (currentIndex >= (int)items.size() - 1) ? 0 : currentIndex + 1;
        }

        // --- 3. マウスによるホバー選択 & クリック ---
        const int startX = 90;
        const int endX = 1100;
        const int startY = 100;
        const int itemH = 85;
        const int margin = 10;
        for (int i = 0; i < maxDisplay; ++i) {
            int targetIdx = scrollIndex + i;
            if (targetIdx >= (int)items.size()) break;
            int y = startY + (i * (itemH + margin));
            if (mouseX >= startX && mouseX <= endX && mouseY >= y && mouseY <= y + itemH) {
                if (currentIndex != targetIdx) currentIndex = targetIdx;
                if (input.IsMouseDown(MOUSE_INPUT_LEFT)) goto DECIDE_LABEL;
            }
        }

        // --- 4. 決定判定 (Bボタン/Enter) ---
        if (input.IsKeyDown(KEY_INPUT_RETURN) || input.IsButtonDown(XINPUT_GAMEPAD_B)) {
        DECIDE_LABEL:
            if (items[currentIndex].item.get() == targetInventory->GetEquippedItem()) {
                state = State::Error;
            }
            else {
                state = State::Confirming;
                confirmIndex = 1; // デフォルト「いいえ」
            }
        }

        // --- 5. 終了判定 (Aボタン/Esc) ---
        if (input.IsKeyDown(KEY_INPUT_ESCAPE) || input.IsButtonDown(XINPUT_GAMEPAD_A)) {
            isActive = false;
            SetMouseDispFlag(FALSE);
        }

        // スクロール自動追従
        if (currentIndex < scrollIndex) {
            scrollIndex = currentIndex;
        }
        else if (currentIndex >= scrollIndex + maxDisplay) {
            scrollIndex = currentIndex - (maxDisplay - 1);
        }
    }
    else if (state == State::Confirming) {
        // --- 追加: マウスによる「はい/いいえ」の選択判定 ---
        int wx = (WINDOW_WIDTH - 400) / 2;
        int wy = (WINDOW_HEIGHT - 150) / 2;

        // 「はい」ボタンの範囲 (Renderの描画位置に合わせる)
        int btn1X = wx + 70;
        int btnY = wy + 90;
        int btnW = 110;
        int btnH = 40;

        // 「いいえ」ボタンの範囲
        int btn2X = wx + 220;

        // マウスホバー判定
        if (mouseY >= btnY && mouseY <= btnY + btnH) {
            if (mouseX >= btn1X && mouseX <= btn1X + btnW) {
                confirmIndex = 0; // はい
                if (input.IsMouseDown(MOUSE_INPUT_LEFT)) goto CONFIRM_DECIDE;
            }
            else if (mouseX >= btn2X && mouseX <= btn2X + btnW) {
                confirmIndex = 1; // いいえ
                if (input.IsMouseDown(MOUSE_INPUT_LEFT)) goto CONFIRM_DECIDE;
            }
        }

        // 既存のキーボード/スティック入力
        if (input.IsKeyDown(KEY_INPUT_LEFT) || input.IsButtonDown(XINPUT_GAMEPAD_DPAD_LEFT) || stickX < -0.5f) confirmIndex = 0;
        if (input.IsKeyDown(KEY_INPUT_RIGHT) || input.IsButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT) || stickX > 0.5f) confirmIndex = 1;

        // Bボタンまたはマウス左クリックで確定
        if (input.IsKeyDown(KEY_INPUT_RETURN) || input.IsButtonDown(XINPUT_GAMEPAD_B)) {
        CONFIRM_DECIDE: // マウスクリック時もここへ飛ばす
            if (confirmIndex == 0) ExecuteSale();
            state = State::Selecting;
        }

        // AボタンまたはEscでキャンセル
        if (input.IsKeyDown(KEY_INPUT_ESCAPE) || input.IsButtonDown(XINPUT_GAMEPAD_A)) {
            state = State::Selecting;
        }
    }
    else if (state == State::Error) {
        // A, B, Enter, 左クリックのいずれかでリストに戻る
        if (input.IsMouseDown(MOUSE_INPUT_LEFT) || input.IsKeyDown(KEY_INPUT_RETURN) ||
            input.IsButtonDown(XINPUT_GAMEPAD_A) || input.IsButtonDown(XINPUT_GAMEPAD_B)) {
            state = State::Selecting;
        }
    }
}

void SalesManager::ExecuteSale() {
    const auto& items = targetInventory->GetItems();
    if (currentIndex >= (int)items.size()) return;

    // 売却額を加算
    Player::GetInstance()->AddCoinValue(items[currentIndex].item->GetValue());
    // インベントリから削除
    targetInventory->RemoveItemAmount(currentIndex, 1);

    // 削除後のカーソル位置調整
    const auto& updatedItems = targetInventory->GetItems();
    if (currentIndex >= (int)updatedItems.size() && !updatedItems.empty()) {
        currentIndex = (int)updatedItems.size() - 1;
    }
}

void SalesManager::Render() {
    if (!isActive || !targetInventory) return;

    // 背景描画
    DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(10, 10, 10), TRUE);

    // --- 所持金表示 ---
    DrawFormatString(100, 45, GetColor(255, 255, 255), "--- 売却 ---    所持金: %d G", Player::GetInstance()->GetCoinValue());

    const auto& items = targetInventory->GetItems();
    const int startY = 110;
    const int itemH = 85;
    const int margin = 10;
    ItemBase* equippedPtr = targetInventory->GetEquippedItem();

    for (int i = 0; i < maxDisplay; ++i) {
        int targetIdx = scrollIndex + i;
        if (targetIdx >= (int)items.size()) break;

        int y = startY + (i * (itemH + margin));
        int frameColor = (targetIdx == currentIndex) ? GetColor(255, 255, 0) : GetColor(200, 200, 200);

        // アイテム枠
        DrawBox(90, y, 1100, y + itemH, frameColor, FALSE);
        if (targetIdx == currentIndex) {
            DrawBox(91, y + 1, 1099, y + itemH - 1, GetColor(40, 40, 10), TRUE);
        }

        int textColor = (targetIdx == currentIndex) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);

        // アイコン
        int iconHandle = targetInventory->GetIconHandleFromCache(items[targetIdx].item->GetItemIcon());
        int iconSize = 64;
        int iconY = y + (itemH - iconSize) / 2;
        if (iconHandle >= 0) {
            DrawExtendGraph(110, iconY, 110 + iconSize, iconY + iconSize, iconHandle, TRUE);
        }

        // テキスト (上下中央)
        int textY = y + (itemH / 2) - 8;

        std::string name = items[targetIdx].item->GetName();
        int attackVal = items[targetIdx].item->GetEffectValue();
        int quantity = items[targetIdx].quantity;
        int price = items[targetIdx].item->GetValue();

        if (items[targetIdx].item.get() == equippedPtr) {
            DrawString(185, textY, "E", GetColor(255, 100, 0));
        }

        if (name.find("ポーション") != std::string::npos || name.find("グレネード") != std::string::npos) {
            DrawFormatString(220, textY, textColor, "%-20s (x%d)  価格:%5d G", name.c_str(), quantity, price);
        }
        else {
            DrawFormatString(220, textY, textColor, "%-20s  攻撃力:%3d  価格:%5d G", name.c_str(), attackVal, price);
        }
    }

    // スクロールバー
    if ((int)items.size() > maxDisplay) {
        int barH = maxDisplay * (itemH + margin) - margin;
        int barX = 1115;
        DrawBox(barX, startY, barX + 6, startY + barH, GetColor(40, 40, 40), TRUE);
        float ratio = (float)maxDisplay / items.size();
        int sliderH = (int)(barH * ratio);
        int sliderY = (int)(barH * ((float)scrollIndex / items.size()));
        DrawBox(barX, startY + sliderY, barX + 6, startY + sliderY + sliderH, GetColor(180, 180, 180), TRUE);
    }

    // ガイド
    DrawString(90, 680, "B:決定  A:戻る  Wheel:スクロール  Stick/DPAD:選択", GetColor(150, 150, 150));

    // --- ダイアログ表示 (ここを修正) ---
    int wx = (WINDOW_WIDTH - 400) / 2;
    int wy = (WINDOW_HEIGHT - 150) / 2;

    if (state == State::Confirming) {
        DrawBox(wx, wy, wx + 400, wy + 150, GetColor(20, 20, 20), TRUE);
        DrawBox(wx, wy, wx + 400, wy + 150, GetColor(255, 255, 255), FALSE);

        DrawString(wx + 65, wy + 40, "このアイテムを売却しますか？", GetColor(255, 255, 255));

        int yColor = (confirmIndex == 0) ? GetColor(255, 255, 0) : GetColor(100, 100, 100);
        int nColor = (confirmIndex == 1) ? GetColor(255, 255, 0) : GetColor(100, 100, 100);

        // --- ボタン位置修正 ---
        // 「はい」ボタン
        int btn1X = wx + 70;
        DrawBox(btn1X, wy + 90, btn1X + 110, wy + 130, yColor, FALSE);
        DrawString(btn1X + 22, wy + 102, "はい(B)", yColor);

        // 「いいえ」ボタン
        int btn2X = wx + 220;
        DrawBox(btn2X, wy + 90, btn2X + 110, wy + 130, nColor, FALSE);
        DrawString(btn2X + 15, wy + 102, "いいえ(A)", nColor);
    }
    else if (state == State::Error) {
        DrawBox(wx, wy, wx + 400, wy + 150, GetColor(60, 20, 20), TRUE);
        DrawBox(wx, wy, wx + 400, wy + 150, GetColor(255, 255, 255), FALSE);

        DrawString(wx + 55, wy + 55, "装備中のため売却できません！", GetColor(255, 100, 100));
        DrawString(wx + 120, wy + 105, "- A or B で戻る -", GetColor(200, 200, 200));
    }
}
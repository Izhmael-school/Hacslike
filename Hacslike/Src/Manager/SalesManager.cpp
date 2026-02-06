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

    // --- 【1200x800基準のレスポンシブ座標計算】 ---
    const int startX = (int)(WINDOW_WIDTH * 0.075f);  // 1200で90
    const int endX = (int)(WINDOW_WIDTH * 0.916f);  // 1200で1100
    const int startY = (int)(WINDOW_HEIGHT * 0.125f); // 800で100
    const int itemH = (int)(WINDOW_HEIGHT * 0.106f); // 800で85
    const int margin = (int)(WINDOW_HEIGHT * 0.0125f); // 800で10

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
        // --- ダイアログのレスポンシブ座標 ---
        int dialogW = (int)(WINDOW_WIDTH * 0.333f);  // 1200で400
        int dialogH = (int)(WINDOW_HEIGHT * 0.187f); // 800で150
        int wx = (WINDOW_WIDTH - dialogW) / 2;
        int wy = (WINDOW_HEIGHT - dialogH) / 2;

        int btnW = (int)(WINDOW_WIDTH * 0.091f);  // 1200で110
        int btnH = (int)(WINDOW_HEIGHT * 0.05f);  // 800で40
        int btnY = wy + (int)(dialogH * 0.6f);    // 150の60%位置

        int btn1X = wx + (int)(dialogW * 0.175f); // 400の17.5%
        int btn2X = wx + (int)(dialogW * 0.55f);  // 400の55%

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

        if (input.IsKeyDown(KEY_INPUT_LEFT) || input.IsButtonDown(XINPUT_GAMEPAD_DPAD_LEFT) || stickX < -0.5f) confirmIndex = 0;
        if (input.IsKeyDown(KEY_INPUT_RIGHT) || input.IsButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT) || stickX > 0.5f) confirmIndex = 1;

        if (input.IsKeyDown(KEY_INPUT_RETURN) || input.IsButtonDown(XINPUT_GAMEPAD_B)) {
        CONFIRM_DECIDE:
            if (confirmIndex == 0) ExecuteSale();
            state = State::Selecting;
        }

        if (input.IsKeyDown(KEY_INPUT_ESCAPE) || input.IsButtonDown(XINPUT_GAMEPAD_A)) {
            state = State::Selecting;
        }
    }
    else if (state == State::Error) {
        if (input.IsMouseDown(MOUSE_INPUT_LEFT) || input.IsKeyDown(KEY_INPUT_RETURN) ||
            input.IsButtonDown(XINPUT_GAMEPAD_A) || input.IsButtonDown(XINPUT_GAMEPAD_B)) {
            state = State::Selecting;
        }
    }
}

void SalesManager::ExecuteSale() {
    const auto& items = targetInventory->GetItems();
    if (currentIndex >= (int)items.size()) return;

    Player::GetInstance()->AddCoinValue(items[currentIndex].item->GetValue());
    targetInventory->RemoveItemAmount(currentIndex, 1);
    AudioManager::GetInstance().PlayOneShot("Buy");

    const auto& updatedItems = targetInventory->GetItems();
    if (currentIndex >= (int)updatedItems.size() && !updatedItems.empty()) {
        currentIndex = (int)updatedItems.size() - 1;
    }
}

void SalesManager::Render() {
    if (!isActive || !targetInventory) return;

    // --- 【1200x800基準のレスポンシブ描画計算】 ---
    const int startX = (int)(WINDOW_WIDTH * 0.075f);  // 90
    const int endX = (int)(WINDOW_WIDTH * 0.916f);  // 1100
    const int startY = (int)(WINDOW_HEIGHT * 0.1375f); // 110
    const int itemH = (int)(WINDOW_HEIGHT * 0.106f);  // 85
    const int margin = (int)(WINDOW_HEIGHT * 0.0125f); // 10

    // 背景描画
    DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(10, 10, 10), TRUE);

    // --- 所持金表示 ---
    DrawFormatString((int)(WINDOW_WIDTH * 0.083f), (int)(WINDOW_HEIGHT * 0.056f), GetColor(255, 255, 255), "--- 売却 ---    所持金: %d G", Player::GetInstance()->GetCoinValue());

    const auto& items = targetInventory->GetItems();
    ItemBase* equippedPtr = targetInventory->GetEquippedItem();

    for (int i = 0; i < maxDisplay; ++i) {
        int targetIdx = scrollIndex + i;
        if (targetIdx >= (int)items.size()) break;

        int y = startY + (i * (itemH + margin));
        int frameColor = (targetIdx == currentIndex) ? GetColor(255, 255, 0) : GetColor(200, 200, 200);

        // アイテム枠
        DrawBox(startX, y, endX, y + itemH, frameColor, FALSE);
        if (targetIdx == currentIndex) {
            DrawBox(startX + 1, y + 1, endX - 1, y + itemH - 1, GetColor(40, 40, 10), TRUE);
        }

        int textColor = (targetIdx == currentIndex) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);

        // アイコン
        int iconHandle = targetInventory->GetIconHandleFromCache(items[targetIdx].item->GetItemIcon());
        int iconSize = (int)(itemH * 0.75f); // 枠の75%サイズ
        int iconX = startX + (int)(WINDOW_WIDTH * 0.016f);
        int iconY = y + (itemH - iconSize) / 2;
        if (iconHandle >= 0) {
            DrawExtendGraph(iconX, iconY, iconX + iconSize, iconY + iconSize, iconHandle, TRUE);
        }

        // テキスト位置
        int textY = y + (itemH / 2) - 8;
        int textX = iconX + iconSize + (int)(WINDOW_WIDTH * 0.01f);

        std::string name = items[targetIdx].item->GetName();
        int attackVal = items[targetIdx].item->GetEffectValue();
        int quantity = items[targetIdx].quantity;
        int price = items[targetIdx].item->GetValue();

        if (items[targetIdx].item.get() == equippedPtr) {
            DrawString(textX - (int)(WINDOW_WIDTH * 0.008f), textY, "E", GetColor(255, 100, 0));
        }

        if (name.find("ポーション") != std::string::npos || name.find("グレネード") != std::string::npos) {
            DrawFormatString(textX + 25, textY, textColor, "%-20s (x%d)  価格:%5d G", name.c_str(), quantity, price);
        }
        else {
            DrawFormatString(textX + 25, textY, textColor, "%-20s  攻撃力:%3d  価格:%5d G", name.c_str(), attackVal, price);
        }
    }

    // スクロールバー
    if ((int)items.size() > maxDisplay) {
        int barH = maxDisplay * (itemH + margin) - margin;
        int barX = endX + 15;
        DrawBox(barX, startY, barX + 6, startY + barH, GetColor(40, 40, 40), TRUE);
        float ratio = (float)maxDisplay / items.size();
        int sliderH = (int)(barH * ratio);
        int sliderY = (int)(barH * ((float)scrollIndex / items.size()));
        DrawBox(barX, startY + sliderY, barX + 6, startY + sliderY + sliderH, GetColor(180, 180, 180), TRUE);
    }

    // ダイアログ表示
    int dialogW = (int)(WINDOW_WIDTH * 0.333f);
    int dialogH = (int)(WINDOW_HEIGHT * 0.187f);
    int wx = (WINDOW_WIDTH - dialogW) / 2;
    int wy = (WINDOW_HEIGHT - dialogH) / 2;

    if (state == State::Confirming) {
        DrawBox(wx, wy, wx + dialogW, wy + dialogH, GetColor(20, 20, 20), TRUE);
        DrawBox(wx, wy, wx + dialogW, wy + dialogH, GetColor(255, 255, 255), FALSE);

        DrawString(wx + (int)(dialogW * 0.16f), wy + (int)(dialogH * 0.26f), "このアイテムを売却しますか？", GetColor(255, 255, 255));

        int yColor = (confirmIndex == 0) ? GetColor(255, 255, 0) : GetColor(100, 100, 100);
        int nColor = (confirmIndex == 1) ? GetColor(255, 255, 0) : GetColor(100, 100, 100);

        int btnW = (int)(WINDOW_WIDTH * 0.091f);
        int btnH = (int)(WINDOW_HEIGHT * 0.05f);
        int btnY = wy + (int)(dialogH * 0.6f);
        int btn1X = wx + (int)(dialogW * 0.175f);
        int btn2X = wx + (int)(dialogW * 0.55f);

        DrawBox(btn1X, btnY, btn1X + btnW, btnY + btnH, yColor, FALSE);
        DrawString(btn1X + 22, btnY + 12, "はい(B)", yColor);

        DrawBox(btn2X, btnY, btn2X + btnW, btnY + btnH, nColor, FALSE);
        DrawString(btn2X + 15, btnY + 12, "いいえ(A)", nColor);
    }
    else if (state == State::Error) {
        DrawBox(wx, wy, wx + dialogW, wy + dialogH, GetColor(60, 20, 20), TRUE);
        DrawBox(wx, wy, wx + dialogW, wy + dialogH, GetColor(255, 255, 255), FALSE);
        DrawString(wx + (int)(dialogW * 0.13f), wy + (int)(dialogH * 0.36f), "装備中のため売却できません！", GetColor(255, 100, 100));
    }
}
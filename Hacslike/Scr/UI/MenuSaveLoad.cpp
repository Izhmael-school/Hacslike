#include "MenuSaveLoad.h"
#include "../Manager/SaveManager.h"
#include "DxLib.h"
#include <ctime>
#include <cstdio>
#include <cstring>
#include"../Definition.h"


#include "../Manager/InputManager.h"
#include "../GameObject/Item/ItemFactory.h"

MenuSaveLoad::MenuSaveLoad(Mode m) : mode(m), selectedSlot(0), menuActive(false), menuChoice(0) {}

void MenuSaveLoad::Open() {
    selectedSlot = 0;
    menuActive = false;
    menuChoice = 0;
}

void MenuSaveLoad::Update() {
    InputManager* input = &InputManager::GetInstance();
    auto& slots = SaveManager::GetInstance().GetSlots();
    // 上下移動
    if (input->IsKeyDown(KEY_INPUT_UP) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_UP)) selectedSlot = (selectedSlot + 9) % 10;
    if (input->IsKeyDown(KEY_INPUT_DOWN) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN)) selectedSlot = (selectedSlot + 1) % 10;

    // Enter: スロット選択 -> ポップアップ開閉 or ポップアップ実行
    if (input->IsKeyUp(KEY_INPUT_RETURN) || input->IsButtonUp(XINPUT_GAMEPAD_B)) {
        if (!menuActive) {
            // ポップアップを開く
            menuActive = true;
            menuChoice = 0;
        }
        else {
            // ポップアップ内で選択を実行
            switch (menuChoice) {
            case 0: // セーブ
                SaveManager::GetInstance().Save(selectedSlot);
                break;
            case 1: // ロード
                // 存在チェックなどは SaveManager 内で行われる想定
                ItemFactory::Instance().InitializeDefaultItems();
                SaveManager::GetInstance().Load(selectedSlot);
                break;
            case 2: // 削除
            {
                SaveManager::GetInstance().Delete(selectedSlot);
               
#ifdef _DEBUG
                printfDx("Slot %d を削除しました (exists=false)\n", selectedSlot + 1);
#endif
            }
            break;
            default:
                break;
            }
            menuActive = false;
        }
    }

    // ポップアップが開いているときは、メニュー内の左右で選択を切り替える
    if (menuActive) {
        if (input->IsKeyDown(KEY_INPUT_LEFT) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)) {
            menuChoice = (menuChoice + 3 - 1) % 3;
        }
        if (input->IsKeyDown(KEY_INPUT_RIGHT) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) {
            menuChoice = (menuChoice + 1) % 3;
        }

        // Esc: キャンセル
        if (input->IsKeyDown(KEY_INPUT_ESCAPE) || input->IsButtonDown(XINPUT_GAMEPAD_A)) {
            menuActive = false;
        }
    }
}

void MenuSaveLoad::Render() {
    const auto& slots = SaveManager::GetInstance().GetSlots();
    int baseX = 250;
    int baseY = 16;
    int BaseX = baseX + 10;
    int BaseY = baseY + 30;
    int boxW = 430;
    int boxH = 280;
    int lineHeight = 24;
    int padding = 6;
    int titleHeight = 22;

    const int bgColor = black;
    const int borderColor = GetColor(200, 200, 200);

    DrawBox(baseX, baseY, baseX + boxW, baseY + boxH, bgColor, TRUE);
    DrawBox(baseX, baseY, baseX + boxW, baseY + titleHeight, borderColor, FALSE);

    // タイトル
    const char* title = "セーブ / ロード";
    DrawString(baseX + padding, baseY + 2, title, white);

    for (int i = 0; i < 10; ++i) {
        const auto& s = slots[i];

        // 時刻文字列作成
        char timestr[64] = "";
        if (s.exists) {
#ifdef _MSC_VER
            std::tm tmBuf;
            if (localtime_s(&tmBuf, &s.timestamp) == 0) {
                std::strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M", &tmBuf);
            }
#else
            std::tm tmBuf;
            if (localtime_r(&s.timestamp, &tmBuf) != nullptr) {
                std::strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M", &tmBuf);
            }
#endif
        }

        const char* stateText = s.exists ? "USED" : "EMPTY";

        char linebuf[256];
#ifdef _MSC_VER
        sprintf_s(linebuf, sizeof(linebuf),
            "Slot %02d [%s] Lv:%u Floor:%u %s",
            i + 1,
            stateText,
            static_cast<unsigned>(s.playerLevel),
            static_cast<unsigned>(s.floor),
            timestr);
#else
        std::snprintf(linebuf, sizeof(linebuf),
            "Slot %02d [%s] Lv:%u Floor:%u %s",
            i + 1,
            stateText,
            static_cast<unsigned>(s.playerLevel),
            static_cast<unsigned>(s.floor),
            timestr);
#endif

        int y = BaseY + i * lineHeight;
        unsigned int color = (i == selectedSlot) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);
        DrawString(BaseX, y, linebuf, color);
    }

    // ポップアップメニュー描画 (開いているとき)
    if (menuActive) {
        const int popupW = 280;
        const int popupH = 96;
        int px = baseX + 10;
        int py = BaseY + selectedSlot * lineHeight + 25; // 選択行の下あたりに表示

        DrawBox(px, py, px + popupW, py + popupH - 20, GetColor(40, 40, 40), TRUE);
        DrawBox(px, py, px + popupW, py + popupH - 20, GetColor(200, 200, 200), FALSE);

        // メニューオプション
        const char* opts[3] = { "セーブ", "ロード", "削除" };
        for (int i = 0; i < 3; ++i) {
            int col = (i == menuChoice) ? GetColor(255, 255, 0) : GetColor(220, 220, 220);
            DrawString(px + 12 + i * 100, py + 12, opts[i], col);
        }
        DrawString(px + 8, py + popupH - 50, "Enter: 決定  Esc: キャンセル", GetColor(180, 180, 180));
    }
}
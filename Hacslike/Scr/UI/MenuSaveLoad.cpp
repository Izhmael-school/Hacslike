#include "MenuSaveLoad.h"
#include "../Manager/SaveManager.h"
#include"../Manager/AudioManager.h"
#include "DxLib.h"
#include <ctime>
#include <cstdio>
#include <cstring>
#include"../Definition.h"


#include "../Manager/InputManager.h"
#include "../GameObject/Item/ItemFactory.h"
#include"../Manager/StageManager.h"
#include"../GameObject/Character/Player/Player.h"


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
    if (input->IsKeyDown(KEY_INPUT_UP) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_UP)) { 
        selectedSlot = (selectedSlot + 9) % 10; 
        AudioManager::GetInstance().PlayOneShot("Select");
    }
    if (input->IsKeyDown(KEY_INPUT_DOWN) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN)) { 
        selectedSlot = (selectedSlot + 1) % 10; 
        AudioManager::GetInstance().PlayOneShot("Select");
    }

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
                AudioManager::GetInstance().PlayOneShot("Decision");
                SaveManager::GetInstance().Save(selectedSlot);
                break;
            case 1: // 削除
                AudioManager::GetInstance().PlayOneShot("Decision");
                SaveManager::GetInstance().Delete(selectedSlot);
#ifdef _DEBUG
                printfDx("Slot %d を削除しました (exists=false)\n", selectedSlot + 1);
#endif
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
            menuChoice = (menuChoice + 2 - 1) % 2;
        }
        if (input->IsKeyDown(KEY_INPUT_RIGHT) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) {
            menuChoice = (menuChoice + 1) % 2;
        }

        // Esc: キャンセル
        if (input->IsKeyDown(KEY_INPUT_ESCAPE) || input->IsButtonDown(XINPUT_GAMEPAD_A)) {
            menuActive = false;
        }
    }
}

void MenuSaveLoad::Render() {
    const auto& slots = SaveManager::GetInstance().GetSlots();

    // ---- 全画面黒背景 ----
    const int bgColor = black;
    DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, bgColor, TRUE); // 全画面を黒で塗りつぶし

    // ---- スロット描画の中央配置設定 ----
    int lineHeight = 40; // 行の高さ
    int slotWidth = 600; // スロット描画幅
    int slotHeight = lineHeight * 10; // スロット全体の高さ（最大10個分）
    int baseX = (WINDOW_WIDTH - slotWidth) / 2; // 中央のX座標
    int baseY = (WINDOW_HEIGHT - slotHeight) / 2; // スロットの中央Y座標
    

    const int slotBaseX = baseX; // スロットのベースX座標
    const int slotBaseY = baseY;
    // タイトル
    const char* title = "セーブ / ロード";
    DrawStringToHandle(baseX , baseY - lineHeight, title, white, MainFont);
    // スロットリストを描画
    for (int i = 0; i < 10; ++i) {
        const auto& s = slots[i];
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
            "Slot %02d [%s] Lv:%d Floor:%d %s",
            i + 1,
            stateText,
            s.playerLevel,
            s.floor,
            timestr);
#else
        std::snprintf(linebuf, sizeof(linebuf),
            "Slot %02d [%s] Lv:%d Floor:%d %s",
            i + 1,
            stateText,
            Player::GetInstance()->GetPlayerLevel(),
            StageManager::GetInstance().GetFloorCount(),
            timestr);
#endif

        // スロットの描画
        int y = slotBaseY + i * lineHeight; // スロットのY座標
        unsigned int color = (i == selectedSlot) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);
        DrawStringToHandle(slotBaseX, y, linebuf, color, MainFont);
    }

    // ---- セーブ/ロード/削除のコマンド表示 ----
    if (menuActive) {
        const int commandWidth = 400; // コマンド領域の幅
        const int commandHeight = 120; // コマンド領域の高さ
        const int commandX = baseX; // 中央に配置
        const int commandY = baseY + slotHeight + 20; // スロットリストの下部

        // コマンドの背景ボックスを描画
        DrawBox(commandX, commandY, commandX + commandWidth, commandY + commandHeight, GetColor(40, 40, 40), TRUE);
        DrawBox(commandX, commandY, commandX + commandWidth, commandY + commandHeight, GetColor(200, 200, 200), FALSE);

        // コマンド文字列
        const char* opts[2] = { "セーブ", "削除" };
        for (int i = 0; i < 2; ++i) {
            int col = (i == menuChoice) ? GetColor(255, 255, 0) : GetColor(220, 220, 220);
            DrawStringToHandle(commandX + 20 + i * 120, commandY + 50, opts[i], col, MainFont);
        }

        // ヒントを追加表示
        DrawStringToHandle(commandX + 20, commandY + 90, "Enter/Bボタン: 決定 Esc:キャンセル", GetColor(180, 180, 180), MainFont);
    }
    // Zバッファ（奥行き）をチェックせずに描画する
    SetUseZBuffer3D(FALSE);
    // Zバッファに書き込みもしない（後の描画に影響を与えない）
    SetWriteZBuffer3D(FALSE);

    int StartX = (WINDOW_WIDTH / 2) - 200;
    int StartY = (WINDOW_HEIGHT)-200;
    int GoalX = (WINDOW_WIDTH / 2) + 200;
    int GoalY = (WINDOW_HEIGHT)-150;
    int textX = StartX + 80;
    int textY = StartY + 17;

    DrawBox(StartX, StartY, GoalX, GoalY, gray, TRUE);
    DrawBox(StartX + 2, StartY + 2, GoalX - 2, GoalY - 2, white, FALSE);
    DrawFormatStringToHandle(textX + 40, textY, black, MainFont, "キー/  ボタン:閉じる");
    DrawFormatStringToHandle(textX, textY, white, MainFont, "ESC");
    DrawFormatStringToHandle(textX + 80, textY, white, MainFont, "A");

    // Zバッファ（奥行き）をチェックせずに描画する
    SetUseZBuffer3D(TRUE);
    // Zバッファに書き込みもしない（後の描画に影響を与えない）
    SetWriteZBuffer3D(TRUE);
}
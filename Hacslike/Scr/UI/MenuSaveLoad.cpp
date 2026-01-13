#include "MenuSaveLoad.h"
#include "../Manager/SaveManager.h"
#include "DxLib.h"
#include <ctime>
#include <cstdio>
#include <cstring>
#include"../Definition.h"


#include "../Manager/InputManager.h"
MenuSaveLoad::MenuSaveLoad(Mode m) : mode(m), selectedSlot(0) {}

void MenuSaveLoad::Open() {
    selectedSlot = 0;
}

void MenuSaveLoad::Update() {
    InputManager* input = &InputManager::GetInstance();
    auto& slots = SaveManager::GetInstance().GetSlots();
    // 簡易入力処理（上/下/決定）
    if (input->IsKeyDown(KEY_INPUT_UP) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_UP)) selectedSlot = (selectedSlot + 9) % 10;
    if (input->IsKeyDown(KEY_INPUT_DOWN) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN)) selectedSlot = (selectedSlot + 1) % 10;
    if (input->IsKeyUp(KEY_INPUT_RETURN) || input->IsButtonUp(XINPUT_GAMEPAD_B)) {
        if (mode == SaveMode) {
            // セーブ前に Player などからメタ情報をセットする
            // 例: SaveManager::GetInstance().GetSlots()[selectedSlot].playerLevel = player->GetLevelForSave();
            SaveManager::GetInstance().Save(selectedSlot);
        }
        else {
            SaveManager::GetInstance().Load(selectedSlot);
        }
    }
    
}

void MenuSaveLoad::Render() {
    const auto& slots = SaveManager::GetInstance().GetSlots();
    int baseX = 250;
    int baseY = 16;
    int BaseY = baseY + 20;
    int boxW = 400;
    int boxH = 250;
    int lineHeight = 24;
    int padding = 6;
    int titleHeight = 22;

    const int bgColor = black;
    const int borderColor = GetColor(200, 200, 200);

    DrawBox(baseX, baseY, baseX + boxW, baseY + boxH, bgColor, TRUE);
    DrawBox(baseX, baseY, baseX + boxW, baseY + titleHeight, borderColor, FALSE);

    // タイトル
    const char* title = "セーブ";
    DrawString(baseX + padding, baseY + 2, title, white);



    for (int i = 0; i < 10; ++i) {
        const auto& s = slots[i];

        // 時刻文字列の作成（スレッド安全な localtime を使う）
        char timestr[64] = "";
        if (s.exists) {
#ifdef _MSC_VER
            std::tm tmBuf;
            // localtime_s(出力先, 入力元)
            if (localtime_s(&tmBuf, &s.timestamp) == 0) {
                std::strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M", &tmBuf);
            }
#else
            std::tm tmBuf;
            // localtime_r(入力元, 出力先)
            if (localtime_r(&s.timestamp, &tmBuf) != nullptr) {
                std::strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M", &tmBuf);
            }
#endif
        }

        const char* stateText = s.exists ? "USED" : "EMPTY";

        // バッファはここで一度だけ宣言（再定義を避ける）
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
        // 選択中スロットは色を変える
        unsigned int color = (i == selectedSlot) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);
        DrawString(baseX, y, linebuf, color);
    }

    // 選択カーソルなど追加描画があればここに
}
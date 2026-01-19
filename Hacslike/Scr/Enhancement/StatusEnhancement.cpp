#include "StatusEnhancement.h"
#include "DxLib.h"
#include <cmath>
#include "../Manager/InputManager.h"
#include "../GameObject/Character/Player/Player.h"

static int oldMouse = 0;

StatusEnhancement::StatusEnhancement()
    : timer(0.0f)
    , allMax(false)
    , playerCoins(0)
    , selectedIndex(0) {
}

StatusEnhancement::~StatusEnhancement() {
}

void StatusEnhancement::Start() {
    if (!stats.empty()) return;

    allMax = false;
    selectedIndex = 0;

    unsigned int defaultColor = GetColor(200, 200, 200);

    // 各ステータスの初期化（名前, レベル, 累積加算値, 色）
    stats.push_back({ "H P",      0, 0, defaultColor });
    stats.push_back({ "攻撃力",   0, 0, defaultColor });
    stats.push_back({ "防御力",   0, 0, defaultColor });
    stats.push_back({ "会心率",   0, 0, defaultColor });
    stats.push_back({ "会心ダメ", 0, 0, defaultColor });
}

bool StatusEnhancement::Update() {
    timer += 0.05f;
    SetMouseDispFlag(TRUE);

    Player* player = Player::GetInstance();
    if (player) playerCoins = player->GetCoinValue();

    InputManager* input = &InputManager::GetInstance();
    if (!input) return false;

    // パッド（十字キー）による選択移動
    if (input->IsButtonDown(XINPUT_GAMEPAD_DPAD_UP)) {
        selectedIndex--;
        if (selectedIndex < 0) selectedIndex = (int)stats.size() - 1;
    }
    if (input->IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN)) {
        selectedIndex++;
        if (selectedIndex >= (int)stats.size()) selectedIndex = 0;
    }

    // マウスによる選択更新
    int mouseX, mouseY;
    GetMousePoint(&mouseX, &mouseY);
    for (int i = 0; i < (int)stats.size(); i++) {
        int x = 380;
        int y = 200 + i * 100;
        if (mouseX >= x && mouseX <= x + 625 && mouseY >= y && mouseY <= y + 35) {
            selectedIndex = i;
        }
    }

    // --- 強化実行処理 ---
    if (input->IsMouseDown(MOUSE_INPUT_LEFT) || input->IsButton(XINPUT_GAMEPAD_A)) {
        if (!stats.empty() && stats[selectedIndex].level < 50) {

            // コスト計算：1フロア100枚制限のため、初期コスト3から緩やかに上昇
            // Lv.0->Lv.1で 3枚、Lv.20で 43枚、Lv.40で 83枚
            int cost = 3 + (stats[selectedIndex].level * 2);

            // コインが足りるかチェック
            if (playerCoins >= cost) {
                // コイン消費
                playerCoins -= cost;
                if (player) player->SetCoinValue(playerCoins);

                // 各ステータスの上昇幅（totalBonusに加算）
                int boostValue = 0;
                switch (selectedIndex) {
                case 0: boostValue = 10; break; // HP +10
                case 1: boostValue = 2;  break; // 攻撃 +2
                case 2: boostValue = 1;  break; // 防御 +1
                case 3: boostValue = 1;  break; // 会心率 +1
                case 4: boostValue = 5;  break; // 会心ダメ +5 (0.05倍相当)
                }

                stats[selectedIndex].totalBonus += boostValue;
                stats[selectedIndex].level++;

                // --- 重要：Playerクラスの実際のステータスに反映 ---
                if (player) {
                    switch (selectedIndex) {
                    case 0: // HP
                        player->SetMaxHp(100 + stats[0].totalBonus);
                        player->SetHp(player->GetMaxHp()); // 最大HP上昇時に全回復
                        break;
                    case 1: // 攻撃力
                        player->SetAtk(10 + stats[1].totalBonus);
                        break;
                    case 2: // 防御力
                        player->SetDef(stats[2].totalBonus);
                        break;
                    case 3: // 会心率
                        player->SetCriticalHitRate((float)(5 + stats[3].totalBonus));
                        break;
                    case 4: // 会心ダメ
                        player->SetCriticalDamage(1.5f + (stats[4].totalBonus / 100.0f));
                        break;
                    }
                }
            }
        }
    }

    // 全ステータスMax判定
    bool check = true;
    if (stats.empty()) {
        check = false;
    }
    else {
        for (const auto& s : stats) {
            if (s.level < 50) { check = false; break; }
        }
    }
    allMax = check;

    // --- ここに追加：閉じる処理 ---
   // Bボタン または ESCキー または 強化画面以外の場所クリック など
    if (input->IsButtonDown(XINPUT_GAMEPAD_B) || input->IsKeyDown(KEY_INPUT_ESCAPE)) {
        return true; // 「画面を閉じる」という合図を送る
    }

    return false;
}

void StatusEnhancement::Render() {
    if (stats.empty()) {
        Start();
        return;
    }

    // --- ここを強化 ---
     // 1. 3Dモデルに隠されないように、深度テストを完全にオフにする
    SetUseZBuffer3D(FALSE);
    SetWriteZBuffer3D(FALSE);

    // 2. もし3D空間の中にメニューが埋まってしまう場合は、ここで一度リセット
    // これにより、石のモデルより物理的に後ろにメニューがあっても描画されます
    ClearDrawScreenZBuffer();

    // 3. ライティングの影響でメニューが暗くならないようにする
    SetUseLighting(FALSE);

    int mouseX, mouseY;
    GetMousePoint(&mouseX, &mouseY);
    

    // 背景パネル
    DrawBox(150, 50, 1150, 750, GetColor(5, 5, 10), TRUE);
    DrawBox(150, 50, 1150, 750, GetColor(40, 40, 50), FALSE);

    // タイトル
    unsigned int titleColor = allMax ? GetColor(255, 255, 255) : GetColor(200, 200, 200);
    DrawString(480, 80, "STATUS ENHANCEMENT", titleColor);

    // --- 所持コイン表示 (右上) ---
    unsigned int coinColor = GetColor(255, 215, 0);
    DrawFormatString(900, 80, coinColor, "COINS: %d", playerCoins);

    for (int i = 0; i < (int)stats.size(); i++) {
        int x = 380;
        int y = 200 + i * 100;

        // 選択カーソル
        if (i == selectedIndex) {
            int animX = (int)(sin(GetNowCount() / 150.0f) * 5.0f);
            DrawString(x - 175 + animX, y + 8, "->", GetColor(255, 255, 255));
        }

        // 項目名
        DrawString(x - 140, y + 8, stats[i].name.c_str(), GetColor(255, 255, 255));

        // 現在のボーナス値 (+○○)
        DrawFormatString(x - 140, y + 55, GetColor(0, 255, 150), "+%d", stats[i].totalBonus);

        // レベル表示
        unsigned int lvColor = (stats[i].level >= 50) ? GetColor(255, 215, 0) : GetColor(150, 150, 150);
        DrawFormatString(x - 140, y + 30, lvColor, "Lv.%d", stats[i].level);

        // ゲージ描画
        DrawParallelGauge(x, y, stats[i].level, stats[i].color);

        // --- コスト表示 ---
        if (stats[i].level < 50) {
            int cost = 3 + (stats[i].level * 2);
            unsigned int cCol = (playerCoins >= cost) ? GetColor(255, 255, 255) : GetColor(255, 50, 50);
            DrawFormatString(x + 640, y + 8, cCol, "COST: %d", cost);
        }
        else {
            DrawString(x + 640, y + 8, "MAXED", GetColor(255, 215, 0));
        }
    }

    if (allMax) {
        DrawString(510, 700, "ULTIMATE RAINBOW ACHIEVED", GetColor(255, 255, 255));
    }

    SetUseLighting(TRUE);
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
}

void StatusEnhancement::DrawParallelGauge(int x, int y, int level, unsigned int baseColor) {
    int w = 55; int h = 35; int skew = 15; int gap = 6;
    float systemTime = GetNowCount() / 1000.0f;

    int loopCount = (level - 1) / 10;
    if (level <= 0) loopCount = 0;
    int currentLoopLevel = level % 10;
    if (currentLoopLevel == 0 && level > 0) currentLoopLevel = 10;

    unsigned int loopColors[] = {
        GetColor(180, 180, 180), // 1周目：シルバー
        GetColor(0, 110, 220),   // 2周目：ブルー
        GetColor(220, 180, 0),   // 3周目：イエロー
        GetColor(140, 0, 230),   // 4周目：パープル
        GetColor(200, 0, 50)     // 5周目：レッド
    };

    for (int i = 0; i < 10; i++) {
        float offsetY = allMax ? sin(systemTime * 4.0f - (i * 0.5f)) * 12.0f : 0.0f;
        int curY = y + (int)offsetY;

        unsigned int contentCol;
        unsigned int outerBorder;
        unsigned int innerLight;

        bool isFilled = (i < currentLoopLevel);

        if (allMax) {
            float h_val = systemTime * 3.0f - (i * 0.4f);
            contentCol = GetColor(
                (int)(128 + 127 * sin(h_val)),
                (int)(128 + 127 * sin(h_val + 2.09f)),
                (int)(128 + 127 * sin(h_val + 4.18f))
            );
            outerBorder = GetColor(255, 215, 0);
            innerLight = GetColor(220, 220, 180);
        }
        else {
            if (loopCount >= 1) {
                contentCol = isFilled ? loopColors[loopCount] : loopColors[loopCount - 1];
            }
            else {
                contentCol = isFilled ? loopColors[0] : GetColor(30, 30, 35);
            }

            int effectiveLoop = isFilled ? loopCount : (loopCount - 1);
            if (effectiveLoop < 0) effectiveLoop = -1;

            if (effectiveLoop >= 4) { outerBorder = GetColor(255, 215, 0); innerLight = GetColor(255, 255, 180); }
            else if (effectiveLoop >= 2) { outerBorder = GetColor(180, 180, 190); innerLight = GetColor(255, 255, 255); }
            else if (effectiveLoop >= 0) { outerBorder = GetColor(100, 100, 100); innerLight = GetColor(140, 140, 140); }
            else { outerBorder = GetColor(50, 50, 50); innerLight = GetColor(70, 70, 70); }

            bool isNextTarget = (i == (level % 10)) && (level < 50);
            if (isNextTarget) {
                float pulse = (sin(systemTime * 6.0f) + 1.0f) / 2.0f;
                outerBorder = GetColor((int)(100 + 155 * pulse), (int)(100 + 155 * pulse), (int)(100 + 155 * pulse));
                innerLight = GetColor(255, 255, 255);
            }
        }

        int x1 = x + i * (w + gap) + skew; int y1 = curY;
        int x2 = x1 + w;                   int y2 = curY;
        int x3 = x + i * (w + gap) + w;    int y3 = curY + h;
        int x4 = x + i * (w + gap);        int y4 = curY + h;

        DrawQuadrangle(x1, y1, x2, y2, x3, y3, x4, y4, contentCol, TRUE);
        DrawQuadrangle(x1, y1, x2, y2, x3, y3, x4, y4, outerBorder, FALSE);

        if (allMax || loopCount > 0 || isFilled || (i == (level % 10) && level < 50)) {
            float shineIdx = sin(systemTime * 3.5f - (i * 0.4f));
            unsigned int finalLight = innerLight;
            if (shineIdx > 0.85f) finalLight = GetColor(255, 255, 255);
            DrawQuadrangle(x1 + 2, y1 + 2, x2 - 1, y2 + 2, x3 - 2, y3 - 2, x4 + 1, y4 - 2, finalLight, FALSE);
        }
    }
}
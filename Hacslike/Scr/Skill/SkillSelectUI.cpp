#include "SkillSelectUI.h"
#include"DxLib.h"
#include"../Definition.h"
#include"../Manager/InputManager.h"


void SkillSelectUI::StartSelection()
{
    selectedIndex = 1; // 真ん中から開始
    isActive = true;


    // 出現演出初期化
    animTimer = 0.0f;
    isAppearing = true;


    // 花火初期化
    fireworks.clear();
    for (int i = 0; i < 5; ++i) {
        FireworkParticle fw;
        fw.x = 300 + i * 250; // スキルカードの下あたり
        fw.y = 600;           // 画面下
        fw.vx = 0;
        fw.vy = -6.0f - i;    // 上方向へ打ち上げ
        fw.color = GetColor(255, 200 - i * 30, 100 + i * 20);
        fw.life = 0;
        fw.maxLife = 60 + i * 10;
        fw.exploded = false;
        fireworks.push_back(fw);
    }
}

int SkillSelectUI::UpdateSelection()
{
    InputManager* input = &InputManager::GetInstance();
    if (!isActive) return -1;

    // 出現演出中は入力無効
    if (isAppearing)
    {
        animTimer++;
        if (animTimer >= animDuration)
        {
            animTimer = animDuration;
            isAppearing = false;
        }
        return -1;
    }

    if (input->IsKeyDown(KEY_INPUT_LEFT) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_LEFT))
    {
        selectedIndex--;
        if (selectedIndex < 0) selectedIndex = 2;
        WaitTimer(150);
    }
    else if (input->IsKeyDown(KEY_INPUT_RIGHT) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT))
    {
        selectedIndex++;
        if (selectedIndex > 2) selectedIndex = 0;
        WaitTimer(150);
    }
    else if (input->IsKeyDown(KEY_INPUT_RETURN) || input->IsButtonDown(XINPUT_GAMEPAD_B))
    {
        isActive = false;
        return selectedIndex;
    }



    return -1;
}

void SkillSelectUI::Render(const std::vector<std::shared_ptr<Skill>>& skills)
{
    if (!isActive) return;
    if (skills.empty()) return;

    const int startX = 200;
    const int startY = 180;
    const int gap = 300;

    // 出現アニメ用の補間値（0.0?1.0）
    float t = animTimer / animDuration;
    if (t > 1.0f) t = 1.0f;

    // イージング（ふわっと出る動き）
    float easeOut = 1 - pow(1 - t, 3); // easeOutCubic

    // 拡大率、Y位置、透明度を補間
    float scale = 0.5f + easeOut * 0.5f;   // 0.5 → 1.0
    float offsetY = -200 + easeOut * 200;  // 上から降りてくる
    int alpha = (int)(easeOut * 255);      // フェードイン

    for (int i = 0; i < (int)skills.size(); ++i)
    {
        int x = startX + i * gap;
        int y = startY + (int)offsetY;

        int color = (i == selectedIndex) ? yellow : GetColor(50, 50, 50);
        int backColor = GetColor(100, 100, 100);

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

        // 背景・枠をスケーリングして描画
        int cx = x + cardWidth / 2;
        int cy = y + cardHeight / 2;
        int halfW = (int)(cardWidth * 0.5f * scale);
        int halfH = (int)(cardHeight * 0.5f * scale);

        DrawBox(cx - halfW, cy - halfH, cx + halfW, cy + halfH, gray, TRUE);
        DrawBox(cx - halfW, cy - halfH, cx + halfW, cy + halfH, color, FALSE);

        // アイコン
        int iconHandle = LoadGraph(skills[i]->GetIconPath().c_str());
        int iconSize = (int)(160 * scale);
        DrawExtendGraph(cx - iconSize / 2, cy - iconSize / 2, cx + iconSize / 2, cy + iconSize / 2, iconHandle, TRUE);
        DeleteGraph(iconHandle);

        // テキスト
        DrawFormatString(cx - (int)(cardWidth * 0.4f * scale), cy - (int)(cardHeight * 0.4f * scale), white, skills[i]->GetName().c_str());
        DrawFormatString(cx - (int)(cardWidth * 0.4f * scale), cy + (int)(cardHeight * 0.3f * scale), white, skills[i]->GetDescription().c_str());
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // 花火描画
    for (auto& fw : fireworks)
    {
        if (!fw.exploded) {
            // 打ち上げ中
            fw.y += fw.vy;
            fw.vy += 0.15f; // 重力

            DrawCircle((int)fw.x, (int)fw.y, 4, fw.color, TRUE);

            fw.life++;
            if (fw.life > fw.maxLife / 2) {
                // 爆発！
                fw.exploded = true;
                for (int j = 0; j < 40; ++j) {
                    FireworkParticle p;
                    p.x = fw.x;
                    p.y = fw.y;
                    float angle = (float)(DX_PI * 2 * j / 40);
                    float speed = 2.0f + GetRand(40) / 20.0f;
                    p.vx = cosf(angle) * speed;
                    p.vy = sinf(angle) * speed;
                    p.color = GetColor(GetRand(255), GetRand(255), GetRand(255));
                    p.life = 0;
                    p.maxLife = 40 + GetRand(20);
                    p.exploded = true;
                    fireworks.push_back(p);
                }
            }
        }
        else {
            // 爆発後の粒
            fw.x += fw.vx;
            fw.y += fw.vy;
            fw.vy += 0.05f; // 重力
            fw.life++;
            int alpha = (int)(255 * (1.0f - fw.life / fw.maxLife));
            if (alpha < 0) alpha = 0;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
            DrawCircle((int)fw.x, (int)fw.y, 2, fw.color, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
    }

    // 寿命切れ削除
    fireworks.erase(
        std::remove_if(fireworks.begin(), fireworks.end(), [](const FireworkParticle& p) {
            return p.life > p.maxLife;
            }),
        fireworks.end());

}
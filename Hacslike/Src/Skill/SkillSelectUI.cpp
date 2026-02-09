#include "SkillSelectUI.h"
#include"DxLib.h"
#include"../Definition.h"
#include"../Manager/InputManager.h"
#include "../Manager/AudioManager.h"



void SkillSelectUI::StartSelection()
{
    selectedIndex = 0; // 真ん中から開始
    isActive = true;


    // 出現演出初期化
    animTimer = 0.0f;
    isAppearing = true;


    // 花火初期化
    fireworks.clear();
    for (int i = 0; i < 5; ++i) {
        FireworkParticle fw;
        fw.x = 500 + i * 350; // スキルカードの横に散らす
        fw.y = 900;           // 画面下
        fw.vx = 0;
        fw.vy = -6.0f - i;    // 上方向に打ち上げ
        fw.color = GetColor(255, 200 - i * 30, 100 + i * 20);
        fw.life = 0;
        fw.maxLife = 60 + i * 10;
        fw.exploded = false;
        fireworks.push_back(fw);
    }
    AudioManager::GetInstance().Load("Res/SE/決定ボタンを押す2.mp3", "SelectSkill", false);
    AudioManager::GetInstance().Load("Res/SE/決定ボタンを押す38.mp3", "DecisionSkill", false);
}

int SkillSelectUI::UpdateSelection(const std::vector<std::shared_ptr<Skill>>& skills)
{
    InputManager* input = &InputManager::GetInstance();
    const int skillCount = (int)skills.size();
    if (!isActive) return -1;

    // 出現アニメ中は無効
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

    //-------------------------------------
    // ▼ マウスによるカード選択（NEW）
    //-------------------------------------
    int mx, my;
    GetMousePoint(&mx, &my);

    hoverIndex = -1;

    const int startX = 200;
    const int startY = 180;
    const int gap = 300;

    // 出現アニメ補間（Render と同じ計算）
    float t = animTimer / animDuration;
    if (t > 1.0f) t = 1.0f;
    float easeOut = 1 - pow(1 - t, 3);
    float scale = 0.5f + easeOut * 0.5f;
    float offsetY = -200 + easeOut * 200;

    for (int i = 0; i < 3; i++)
    {
        const int centerX = 960; // 画面中央（解像度1280x720の場合）
        const int centerY = 270;
        // index = 0 を中央、1を左、2を右にする
        int offsetX = 0;
        if (i == 0) offsetX = 0;
        else if (i == 1) offsetX = -gap;
        else if (i == 2) offsetX = gap;

        int x = centerX + offsetX - cardWidth / 2;
        int y = centerY;

        int cx = x + cardWidth / 2;
        int cy = y + cardHeight / 2;
        int halfW = (int)(cardWidth * 0.5f * scale);
        int halfH = (int)(cardHeight * 0.5f * scale);

        int left = cx - halfW;
        int right = cx + halfW;
        int top = cy - halfH;
        int bottom = cy + halfH;

        if (mx >= left && mx <= right && my >= top && my <= bottom)
        {
            hoverIndex = i;
        }
    }

    // ★ マウスがカードの上にある場合、選択カーソルを移動
    if (hoverIndex != -1)
    {
        if (selectedIndex != hoverIndex)
        {
            selectedIndex = hoverIndex;
            AudioManager::GetInstance().PlayOneShot("SelectSkill");
        }

        // ★ 左クリックで決定
        if (input->IsMouseDown(MOUSE_INPUT_LEFT))
        {
            isActive = false;
            AudioManager::GetInstance().PlayOneShot("DecisionSkill");
            SetMouseDispFlag(FALSE);
            return selectedIndex;
        }
    }
    
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

    if (inputCooldown > 0) {
        inputCooldown--;
    }
    else {
        if (input->IsKeyDown(KEY_INPUT_LEFT) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)) {
            selectedIndex = (selectedIndex + 1)% skillCount;
            inputCooldown = 10;  // 10F = 約0.16秒
            
            AudioManager::GetInstance().PlayOneShot("SelectSkill");
        }
        else if (input->IsKeyDown(KEY_INPUT_RIGHT) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) {
            selectedIndex = (selectedIndex + skillCount - 1) % skillCount;

            inputCooldown = 10;
            AudioManager::GetInstance().PlayOneShot("SelectSkill");
        }
        else if (input->IsKeyDown(KEY_INPUT_RETURN) || input->IsButtonDown(XINPUT_GAMEPAD_B))
        {
            isActive = false;
            AudioManager::GetInstance().PlayOneShot("DecisionSkill");
            SetMouseDispFlag(FALSE);

            return selectedIndex;
        }
    }




    return -1;
}

void SkillSelectUI::Render(const std::vector<std::shared_ptr<Skill>>& skills)
{
    if (!isActive) return;
    if (skills.empty()) return;

    const int centerX = 960; // 画面中央（解像度1280x720の場合）
    const int centerY = 270;
    const int gap = 300;

    // 出現アニメ補間
    float t = animTimer / animDuration;
    if (t > 1.0f) t = 1.0f;
    float easeOut = 1 - pow(1 - t, 3);
    float scale = 0.5f + easeOut * 0.5f;
    float offsetY = -200 + easeOut * 200;
    int alpha = (int)(easeOut * 255);

    for (int i = 0; i < (int)skills.size(); ++i)
    {
        // index = 0 を中央、1を左、2を右にする
        int offsetX = 0;
        if (i == 0) offsetX = 0;
        else if (i == 1) offsetX = -gap;
        else if (i == 2) offsetX = gap;

        int x = centerX + offsetX - cardWidth / 2;
        int y = centerY;

        int color = (i == selectedIndex) ? yellow : GetColor(50, 50, 50);

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

        int cx = x + cardWidth / 2;
        int cy = y + cardHeight / 2;
        int halfW = (int)(cardWidth * 0.5f * scale);
        int halfH = (int)(cardHeight * 0.5f * scale);

        DrawBox(cx - halfW, cy - halfH, cx + halfW, cy + halfH, gray, TRUE);
        DrawBox(cx - halfW, cy - halfH, cx + halfW, cy + halfH, color, FALSE);

        int iconHandle = LoadGraph(skills[i]->GetIconPath().c_str());
        int iconSize = (int)(160 * scale);
        DrawExtendGraph(cx - iconSize / 2, cy - iconSize / 2, cx + iconSize / 2, cy + iconSize / 2, iconHandle, TRUE);
        DeleteGraph(iconHandle);

        DrawFormatStringToHandle(cx - (int)(cardWidth * 0.4f * scale), cy - (int)(cardHeight * 0.4f * scale), white,MainFont, skills[i]->GetName().c_str());
        // ★レベル表示追加（ここ！）
        std::string lvText = "Lv: " + std::to_string(skills[i]->GetLevel()) +
            " / " + std::to_string(skills[i]->GetMaxLevel());

        DrawFormatStringToHandle(cx - (int)(cardWidth * 0.4f * scale),cy - (int)(cardHeight * 0.35f * scale),white,MainFont,lvText.c_str());
        DrawFormatStringToHandle(cx - (int)(cardWidth * 0.4f * scale), cy + (int)(cardHeight * 0.3f * scale), white,MainFont, skills[i]->GetDescription().c_str());
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

 //-------------------------------------
 //  花火の上限チェック
 //-------------------------------------
     // 花火の継続的生成
    static int fireworkTimer = 0;
    fireworkTimer++;
    const int MAX_FIREWORK = 250;
    if (fireworks.size() < MAX_FIREWORK) {
        if (fireworkTimer > 40) {
            fireworkTimer = 0;
            FireworkParticle fw;
            fw.x = 300 + GetRand(1320); // 1920の範囲に調整
            fw.y = 900;
            fw.vx = 0;
            fw.vy = -6.0f - GetRand(4);
            fw.color = GetColor(255, 200 - GetRand(50), 100 + GetRand(100));
            fw.life = 0;
            fw.maxLife = 60 + GetRand(30);
            fw.exploded = false;
            fireworks.push_back(fw);
        }
    }


    //-------------------------------------
    //  描画（BlendMode はまとめて1回）
    //-------------------------------------
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

    for (auto& fw : fireworks)
    {
        if (!fw.exploded) {
            // 上へ
            fw.y += fw.vy;
            fw.vy += 0.15f;
            DrawCircle((int)fw.x, (int)fw.y, 3, fw.color, TRUE);

            fw.life++;

            // 爆発
            if (fw.life > fw.maxLife / 2 && fireworks.size() < MAX_FIREWORK) {
                fw.exploded = true;

                for (int j = 0; j < 16; ++j) { // 40 → 16 に減らす
                    FireworkParticle p;
                    p.x = fw.x;
                    p.y = fw.y;
                    float angle = (float)(DX_PI * 2 * j / 16);
                    float s = 2.0f + GetRand(20) / 20.0f;
                    p.vx = cosf(angle) * s;
                    p.vy = sinf(angle) * s;
                    p.color = fw.color;
                    p.life = 0;
                    p.maxLife = fw.maxLife / 2;
                    p.exploded = true;
                    fireworks.push_back(p);
                }
            }
        }
        else {
            fw.x += fw.vx;
            fw.y += fw.vy;
            fw.vy += 0.05f;
            fw.life++;

            float a = 1.0f - (float)fw.life / fw.maxLife;
            if (a < 0) a = 0;

            DrawCircle((int)fw.x, (int)fw.y, 2, fw.color, TRUE);
        }
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
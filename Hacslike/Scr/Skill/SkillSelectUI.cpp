#include "SkillSelectUI.h"
#include"DxLib.h"
#include"../Definition.h"
#include"../Manager/InputManager.h"


void SkillSelectUI::StartSelection()
{
    selectedIndex = 1; // 真ん中から開始
    isActive = true;
}

int SkillSelectUI::UpdateSelection()
{
    InputManager* input = InputManager::GetInstance();
    if (!isActive) return -1;

    if (input->IsKeyDown(KEY_INPUT_LEFT) || input->IsButtonDown(XINPUT_BUTTON_DPAD_LEFT))
    {
        selectedIndex--;
        if (selectedIndex < 0) selectedIndex = 2;
        WaitTimer(150);
    }
    else if (input->IsKeyDown(KEY_INPUT_RIGHT) || input->IsButtonDown(XINPUT_BUTTON_DPAD_RIGHT))
    {
        selectedIndex++;
        if (selectedIndex > 2) selectedIndex = 0;
        WaitTimer(150);
    }
    else if (input->IsKeyDown(KEY_INPUT_RETURN) || input->IsButtonDown(XINPUT_BUTTON_B))
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

    // 背景の黒幕
   /* DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);*/

    const int startX = 200;
    const int startY = 180;
    const int gap = 300;

    for (int i = 0; i < (int)skills.size(); ++i)
    {
        int x = startX + i * gap;
        int y = startY;

        int color = (i == selectedIndex) ? yellow : GetColor(50, 50, 50);
        int backColor = GetColor(100, 100, 100);

        // カード背景
        DrawBox(x, y, x + cardWidth, y + cardHeight, gray, TRUE);

        // 枠
        DrawBox(x, y, x + cardWidth, y + cardHeight, color, FALSE);

        // アイコン
        int iconHandle = LoadGraph(skills[i]->GetIconPath().c_str());
        DrawExtendGraph(x + 50, y + 50, x + 210, y + 210, iconHandle, TRUE);
        DeleteGraph(iconHandle);

        // テキスト
        DrawFormatString(x + 20, y + 20, white, skills[i]->GetName().c_str());
        DrawFormatString(x + 20, y + 240, white, skills[i]->GetDescription().c_str());
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
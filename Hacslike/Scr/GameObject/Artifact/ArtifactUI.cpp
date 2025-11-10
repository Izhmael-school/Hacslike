#define NOMINMAX
#include "ArtifactUI.h"
#include <algorithm>
#include "../../Manager/InputManager.h"

void ArtifactUI::Update()
{
    InputManager* input = &InputManager::GetInstance();

    int total = (int)ArtifactManager::GetInstance()->GetObtainedArtifacts().size();

    // 下キー：次のページ
    if (input->IsKeyDown(KEY_INPUT_DOWN) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN))
    {
        if (displayStartIndex + displayCount < total)
            displayStartIndex++;
    }

    // 上キー：前のページ
    if (input->IsKeyDown(KEY_INPUT_UP) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_UP))
    {
        if (displayStartIndex > 0)
            displayStartIndex--;
    }
}

void ArtifactUI::Render()
{
    const auto& obtained = ArtifactManager::GetInstance()->GetObtainedArtifacts();
    if (obtained.empty()) return;

    const int iconSize = 48;
    const int padding = 8;
    const int boxMargin = 6;
    const int frameHeight = iconSize + padding * 3;

    const int startX = baseX + padding;
    const int startY = baseY + titleHeight + padding + 3;
    const int visibleTop = baseY + titleHeight;
    const int visibleBottom = baseY + boxH;

    // 背景
    DrawBox(baseX, baseY, baseX + boxW, baseY + boxH, GetColor(0, 0, 0), TRUE);
    DrawBox(baseX, baseY, baseX + boxW, baseY + boxH, GetColor(200, 200, 200), FALSE);

    // タイトル
    DrawBox(baseX, baseY, baseX + boxW, baseY + titleHeight, GetColor(40, 40, 40), TRUE);
    DrawString(baseX + padding, baseY + 4, "アーティファクト", GetColor(255, 255, 255));

    // --- 表示可能範囲だけ描画 ---
    int total = (int)obtained.size();
    int endIndex = std::min(displayStartIndex + displayCount, total);

    for (int i = displayStartIndex; i < endIndex; ++i)
    {
        auto& art = obtained[i];
        int frameY = startY + (i - displayStartIndex) * (frameHeight + boxMargin);
        int frameX = startX;
        int frameW = boxW - padding * 2;
        int frameH = frameHeight;

        DrawBox(frameX, frameY, frameX + frameW, frameY + frameH, GetColor(20, 20, 20), TRUE);
        DrawBox(frameX, frameY, frameX + frameW, frameY + frameH, GetColor(255, 255, 255), FALSE);

        int icon = art->GetIconHandle();
        if (icon > 0)
        {
            int iconX = frameX + padding;
            int iconY = frameY + (frameH - iconSize) / 2;
            DrawExtendGraph(iconX, iconY, iconX + iconSize, iconY + iconSize, icon, TRUE);
        }

        int textX = frameX + iconSize + padding * 2;
        DrawFormatString(textX, frameY + 6, GetColor(255, 255, 255), "%s", art->GetName().c_str());
        DrawFormatString(textX, frameY + 28, GetColor(200, 200, 200), "%s", art->GetDescription().c_str());
    }

    // --- 矢印アイコンを描画（ページありなら） ---
    if (displayStartIndex > 0)
        DrawTriangle(baseX + boxW / 2 - 10, baseY + titleHeight - 8,
            baseX + boxW / 2 + 10, baseY + titleHeight - 8,
            baseX + boxW / 2, baseY + titleHeight - 20, GetColor(255, 255, 255), TRUE);

    if (displayStartIndex + displayCount < total)
        DrawTriangle(baseX + boxW / 2 - 10, visibleBottom + 4,
            baseX + boxW / 2 + 10, visibleBottom + 4,
            baseX + boxW / 2, visibleBottom + 16, GetColor(255, 255, 255), TRUE);
}

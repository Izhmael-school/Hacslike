#include "SaveSlotUI.h"
#include "../Manager/SaveManager.h"
#include "DxLib.h"
#include"../Definition.h"

SaveSlotUI::SaveSlotUI(int slot, int px, int py, int w, int h)
    : slotIndex(slot), x(px), y(py), w(w), h(h)
{
    SaveManager::LoadMeta(slot, meta);
}

void SaveSlotUI::Draw()
{
    DrawBox(x, y, x + w, y + h, white, FALSE);

    if (!meta.isValid)
    {
        DrawString(x + 10, y + 10, "セーブデータがありません", white);
        return;
    }

    char buf[128];
    sprintf_s(buf, "階層 %d\nレベル %d", meta.floor, meta.level);

    DrawString(x + 10, y + 10, buf, white);
}

bool SaveSlotUI::Hit(int mx, int my)
{
    return (mx >= x && mx <= x + w && my >= y && my <= y + h);
}
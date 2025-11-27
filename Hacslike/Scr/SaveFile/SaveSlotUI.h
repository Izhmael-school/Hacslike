#pragma once
#include"SaveMeta.h"

class SaveSlotUI
{
public:
    int x, y, w, h;
    int slotIndex;
    SaveMeta meta;

    SaveSlotUI(int slot, int px, int py, int w, int h);

    void Draw();
    bool Hit(int mx, int my);
};


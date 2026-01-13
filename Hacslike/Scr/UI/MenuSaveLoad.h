#pragma once
class MenuSaveLoad
{
public:
    enum Mode { SaveMode, LoadMode };
    MenuSaveLoad(Mode m);
    void Open();
    void Update();
    void Render();
public:
    Mode mode;
    int selectedSlot;
};


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
    bool menuActive = false;
    int menuChoice = 0; // 0=セーブ,1=ロード,2=削除
};


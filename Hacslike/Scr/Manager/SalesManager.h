#pragma once
#include "../Definition.h"
#include <string>

class Inventory; // 前方宣言

class SalesManager : public Singleton<SalesManager> {

private:
    enum class State {
        Selecting,  // アイテム選択中
        Confirming  // 「本当に売る？」の確認中
    };

    SalesManager();
    ~SalesManager() = default;

    bool isActive = false;
    State state = State::Selecting;
    Inventory* targetInventory = nullptr;
    int currentIndex = 0;   // アイテムリストのカーソル
    int confirmIndex = 1;   // はい(0) / いいえ(1) のカーソル

public:
    // 売却シーンの開始
    void StartSellScene(Inventory* inv);
    // 更新処理（入力）
    void Update();
    // 描画処理（DxLib）
    void Render();

    bool IsActive() const { return isActive; }

private:
    // 売却の内部実行
    void ExecuteSale();
};
#include "SalesManager.h"
#include "../GameObject/Item/Inventory.h" 
#include "../GameObject/Character/Player/Player.h"

SalesManager::SalesManager() : isActive(false), state(State::Selecting), targetInventory(nullptr) {}

void SalesManager::StartSellScene(Inventory* inv) {
    if (!inv) return;
    targetInventory = inv;
    isActive = true;
    state = State::Selecting;
    currentIndex = 0;
    confirmIndex = 1; // デフォルトは「いいえ」
}

void SalesManager::Update() {
    if (!isActive || !targetInventory) return;

    const auto& items = targetInventory->GetItems();

    // アイテムが空ならEscで閉じるのみ
    if (items.empty()) {
        if (CheckHitKey(KEY_INPUT_ESCAPE)) isActive = false;
        return;
    }

    if (state == State::Selecting) {
        // リストの移動
        if (CheckHitKey(KEY_INPUT_UP)) {
            currentIndex = (currentIndex <= 0) ? (int)items.size() - 1 : currentIndex - 1;
            AudioManager::GetInstance().PlayOneShot("Select");
        }
        if (CheckHitKey(KEY_INPUT_DOWN)) {
            currentIndex = (currentIndex >= (int)items.size() - 1) ? 0 : currentIndex + 1;
            AudioManager::GetInstance().PlayOneShot("Select");
        }

        // 決定：確認ダイアログへ
        if (CheckHitKey(KEY_INPUT_RETURN)) {
            state = State::Confirming;
            confirmIndex = 1;
            AudioManager::GetInstance().PlayOneShot("Decision");
        }

        // 閉じる
        if (CheckHitKey(KEY_INPUT_ESCAPE)) {
            isActive = false;
        }
    }
    else if (state == State::Confirming) {
        // はい・いいえの選択
        if (CheckHitKey(KEY_INPUT_LEFT) || CheckHitKey(KEY_INPUT_RIGHT)) {
            confirmIndex = 1 - confirmIndex;
            AudioManager::GetInstance().PlayOneShot("Select");
        }

        // 最終決定
        if (CheckHitKey(KEY_INPUT_RETURN)) {
            if (confirmIndex == 0) {
                ExecuteSale();
            }
            state = State::Selecting;
        }

        // キャンセル
        if (CheckHitKey(KEY_INPUT_ESCAPE)) {
            state = State::Selecting;
            AudioManager::GetInstance().PlayOneShot("Decision");
        }
    }
}

void SalesManager::ExecuteSale() {
    const auto& items = targetInventory->GetItems();
    auto& target = items[currentIndex];

    // 装備中チェック（IDで比較）
    if (target.item->GetID() == targetInventory->GetEquippedItemID()) {
        AudioManager::GetInstance().PlayOneShot("NotRemoveItem");
        return;
    }

    // お金を増やす
    int price = target.item->GetValue();
    Player::GetInstance()->AddCoinValue(price);

    // インベントリから1つ減らす（独自に作った共通関数）
    targetInventory->RemoveItemAmount(currentIndex, 1);

    AudioManager::GetInstance().PlayOneShot("DecisionSkill");
}

void SalesManager::Render() {
    if (!isActive || !targetInventory) return;

    // 1. 全面黒背景
    DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);

    // 2. アイテムリスト描画
    const auto& items = targetInventory->GetItems();
    for (int i = 0; i < (int)items.size(); ++i) {
        int y = 120 + (i * 60);
        int color = (i == currentIndex) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);

        // --- あなたの Inventory.cpp と同じアイコン描画ロジック ---
        int iconHandle = -1;
        std::string iconPath = items[i].item->GetItemIcon();

        if (!iconPath.empty()) {
            // Inventory クラスが持っている iconCache にアクセス（public または friend 設定が必要）
            // もしアクセスできない場合は、SalesManager 側にも std::unordered_map<std::string, int> iconCache を持たせます
            iconHandle = targetInventory->GetIconHandleFromCache(iconPath);
        }

        if (iconHandle >= 0) {
            // 比率を維持して描画（Inventory::Render のロジックを拝借）
            int targetH = 48; // アイコンの表示サイズ
            int gw = 0, gh = 0;
            GetGraphSize(iconHandle, &gw, &gh);
            if (gw > 0 && gh > 0) {
                int destW = static_cast<int>((float)gw * ((float)targetH / (float)gh) + 0.5f);
                DrawExtendGraph(100, y, 100 + destW, y + targetH, iconHandle, TRUE);
            }
        }
        else {
            // アイコンがない場合の仮ボックス
            DrawBox(100, y, 148, y + 48, GetColor(100, 100, 100), TRUE);
        }

        // 名前と価格の描画
        DrawFormatString(180, y + 15, color, "%s (x%d)  価格: %d G",
            items[i].item->GetName().c_str(), items[i].quantity, items[i].item->GetValue());
    }

    // 4. 確認ダイアログ
    if (state == State::Confirming) {
        int wx = 440, wy = 300;
        DrawBox(wx, wy, wx + 400, wy + 150, GetColor(30, 30, 30), TRUE);
        DrawBox(wx, wy, wx + 400, wy + 150, GetColor(255, 255, 255), FALSE);

        DrawString(wx + 80, wy + 30, "このアイテムを売りますか？", GetColor(255, 255, 255));

        int yColor = (confirmIndex == 0) ? GetColor(255, 255, 0) : GetColor(100, 100, 100);
        int nColor = (confirmIndex == 1) ? GetColor(255, 255, 0) : GetColor(100, 100, 100);

        DrawString(wx + 100, wy + 90, "【 はい 】", yColor);
        DrawString(wx + 220, wy + 90, "【 いいえ 】", nColor);
    }
}
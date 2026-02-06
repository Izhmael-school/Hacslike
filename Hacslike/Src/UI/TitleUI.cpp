#include "TitleUI.h"
#include "../Manager/SceneManager.h" 
#include "../Manager/InputManager.h"


TitleUI::TitleUI() : isConfirmingExit(false), isConfirmedExit(false) {}

TitleUI::~TitleUI() {}

void TitleUI::Open() {
    isConfirmingExit = true;
    isConfirmedExit = false;
}

void TitleUI::ResetConfirmation() {
    isConfirmingExit = false;
    isConfirmedExit = false;
}

bool TitleUI::GetIsConfirmedExit() {
    return isConfirmedExit;
}

// 遷移確認の状態を更新
void TitleUI::Update() {
    InputManager* input = &InputManager::GetInstance();
    // 確認中でなければ何もしない
    if (!isConfirmingExit) return;

    if (input->IsKeyDown(KEY_INPUT_Y) || input->IsButtonDown(XINPUT_GAMEPAD_B)) {
        isConfirmedExit = true; // 確認完了
        isConfirmingExit = false;

        // **シーン変更実行**
        SceneManager::GetInstance().ChangeScene(SceneType::Title);
    }
    else if (input->IsKeyDown(KEY_INPUT_N) || input->IsButtonDown(XINPUT_GAMEPAD_A)) {
        isConfirmingExit = false; // キャンセル
    }
}

void TitleUI::Render() {
    if (!isConfirmingExit) return;

    // 確認ダイアログの描画
    const int dialogX = 250;
    const int dialogY = 150;
    const int dialogWidth = 600;
    const int dialogHeight = 100;

    DrawBox(dialogX, dialogY, dialogX + dialogWidth, dialogY + dialogHeight, GetColor(30, 30, 30), TRUE);
    DrawBox(dialogX - 2, dialogY - 2, dialogX + dialogWidth + 2, dialogY + dialogHeight + 2, GetColor(255, 255, 255), FALSE);

    DrawString(dialogX + 20, dialogY , "タイトルに戻りますか？", GetColor(255, 255, 255));
    DrawString(dialogX + 20, dialogY + 40, "Yキー/Bボタン: はい  Nキー/Aボタン: いいえ", GetColor(200, 200, 200));
}

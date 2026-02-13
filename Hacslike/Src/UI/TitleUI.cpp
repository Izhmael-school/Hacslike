#include "TitleUI.h"
#include "../Manager/SceneManager.h" 
#include "../Manager/InputManager.h"


TitleUI::TitleUI() : isConfirmingExit(false), isConfirmedExit(false), isSecondConfirmation(false) {}

TitleUI::~TitleUI() {}

void TitleUI::Open() {
    isConfirmingExit = true;
    isConfirmedExit = false;
    //isSecondConfirmation = false;
}

void TitleUI::ResetConfirmation() {
    isConfirmingExit = false;
    isConfirmedExit = false;
    isSecondConfirmation = false;
}

bool TitleUI::GetIsConfirmedExit() {
    return isConfirmedExit;
}

// 画面遷移確認の状態を更新
void TitleUI::Update() {
    InputManager* input = &InputManager::GetInstance();
    // 確認中でないなら何も処理しない
    if (!isConfirmingExit) return;

    if (input->IsKeyDown(KEY_INPUT_Y) || input->IsButtonDown(XINPUT_GAMEPAD_B)) {
        if (!isSecondConfirmation) {
            // 一段階目の確認 → 二段階目へ (一度trueになったら維持)
            isSecondConfirmation = true;
        }
        // isSecondConfirmationがtrueの状態では何もしない（キーを離すまで待つ）
    }
    else if (input->IsKey(KEY_INPUT_N) || input->IsButton(XINPUT_GAMEPAD_A)) {
        if (isSecondConfirmation) {
            // 二段階目でキャンセル → 一段階目に戻る
            isSecondConfirmation = false;
        }
        else {
            // 一段階目でキャンセル → 確認を閉じる
            isConfirmingExit = false;
        }
    }
    else {
        // キーが離されている状態
        if (isSecondConfirmation) {
            // 二段階目の状態でYキー/Bボタンが再度押されたらシーン変更
            // この時点ではキーが離されているので、次のフレームで押されるのを待つ
            static bool wasKeyReleased = false;

            // 前回のフレームでキーが離されていて、今回押されたら実行
            if (wasKeyReleased && (input->IsKey(KEY_INPUT_Y) || input->IsKey(XINPUT_GAMEPAD_B))) {
                isConfirmedExit = true;
                isConfirmingExit = false;
                SceneManager::GetInstance().ChangeScene(SceneType::Title);
                wasKeyReleased = false;
            }
            else if (!input->IsKey(KEY_INPUT_Y) && !input->IsKey(XINPUT_GAMEPAD_B)) {
                wasKeyReleased = true;
            }
        }
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

    if (!isSecondConfirmation) {
        // 一段階目のメッセージ
        DrawString(dialogX + 20, dialogY, "タイトルに戻りますか?", GetColor(255, 255, 255));
        DrawString(dialogX + 20, dialogY + 40, "Yキー/Bボタン: はい  Nキー/Aボタン: いいえ", GetColor(200, 200, 200));
    }
    else {
        // 二段階目のメッセージ
        DrawString(dialogX + 20, dialogY, "本当にタイトルに戻りますか? (進行状況は失われます)", GetColor(255, 100, 100));
        DrawString(dialogX + 20, dialogY + 40, "Yキー/Bボタン: はい  Nキー/Aボタン: いいえ", GetColor(200, 200, 200));
    }
}
#pragma once
class TitleUI
{
private:
    bool isConfirmingExit; // タイトルに戻る確認を表示するフラグ
    bool isConfirmedExit;  // 本当にタイトルに戻る処理フラグ
    bool isSecondConfirmation; // 二段階目の確認フラグ
    bool prevYesInput; // 前フレームのYes入力状態

public:
    TitleUI();
    ~TitleUI();

    void Open();
    void Update();
    void Render();

    // 追加：フラグのリセット用メソッド
    void ResetConfirmation();
    bool GetIsConfirmedExit(); // 確認結果の取得
};
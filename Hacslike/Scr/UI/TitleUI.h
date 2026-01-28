#pragma once
class TitleUI
{
private:
    bool isConfirmingExit; // タイトルに戻る確認を表示するフラグ
    bool isConfirmedExit;  // 本当にタイトルに戻る決定フラグ

public:
    TitleUI();
    ~TitleUI();

    void Open();
    void Update();
    void Render();

    // 追加：フラグのリセットメソッド
    void ResetConfirmation();
    bool GetIsConfirmedExit(); // 確認結果の取得
};


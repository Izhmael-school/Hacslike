#pragma once
#include "DxLib.h"
#include "../Definition.h"

struct GameEndUI {
private:
    // コンストラクタをprivateにして、勝手にインスタンス化できないようにする
    GameEndUI() = default;

public:
    // 唯一のインスタンスを取得する関数
    static GameEndUI* GetInstance() {
        static GameEndUI instance; // staticなローカル変数として実体を保持
        return &instance;
    }

    // コピー禁止（シングルトンの安全性を高める）
    GameEndUI(const GameEndUI&) = delete;
    void operator=(const GameEndUI&) = delete;

    int alpha = 0;
    int step = 0;  // 0:待機, 1:上昇, 2:下降

    void Start() {
        if (step == 0) step = 1;
    }

    inline void Update() {
        if (step == 1) { // 上昇
            alpha += 2;
            if (alpha >= 255) {
                alpha = 255;
                step = 2;
            }
        }
        else if (step == 2) { // 下降
            alpha -= 2;
            if (alpha <= 0) {
                alpha = 0;
                step = 0;
            }
        }
    }

    inline void Draw() {
        if (step == 0 && alpha == 0) return;

        // 画面の中心を計算
        int centerX = WINDOW_WIDTH / 2;
        int centerY = WINDOW_HEIGHT / 2;

        // 描画設定
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

        //  背景帯
        int halfRectHeight = 75;
        DrawBox(0, centerY - halfRectHeight, WINDOW_WIDTH, centerY + halfRectHeight, GetColor(5, 0, 0), TRUE);

        DrawExtendString(centerX - 200, centerY - 40, 5, 5, "YOU DIED", GetColor(255, 30, 30));

        // 4. 設定を戻す
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
};
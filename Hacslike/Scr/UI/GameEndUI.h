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

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawBox(0, 200, WINDOW_WIDTH, 350, GetColor(5, 0, 0), TRUE);
        DrawExtendString(400, 260, 5, 5, "YOU DIED", GetColor(255, 30, 30));
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
};
#pragma once
#include "DxLib.h"
#include "../Definition.h"
#include "../GameObject/Stage/StageGenerator.h"
#include <vector>
#include <cmath>

struct BossSlainUI {
    struct FireworkParticle {
        float x, y;
        float vx, vy;
        int color;
        float life;
        float maxLife;
        bool exploded;
    };

private:
    BossSlainUI() = default;
    int fireworkTimer = 0;
    int stayTimer = 0; // 表示を維持するためのタイマー

public:
    static BossSlainUI* GetInstance() {
        static BossSlainUI instance;
        return &instance;
    }

    BossSlainUI(const BossSlainUI&) = delete;
    void operator=(const BossSlainUI&) = delete;

    int alpha = 0;
    int step = 0;  // 0:待機, 1:フェードイン, 2:維持, 3:フェードアウト
    std::vector<FireworkParticle> fireworks;
    const int MAX_FIREWORK = 500; // 爆発分を含めて少し余裕を持たせる

    void Start() {
        step = 1;
        alpha = 0;
        fireworkTimer = 0;
        stayTimer = 0;
        fireworks.clear();

        // 初発の打ち上げ
        for (int i = 0; i < 5; ++i) {
            AddFirework(300 + i * 250, 600, -6.0f - (float)i);
        }
    }

    void AddFirework(float x, float y, float vy) {
        if (fireworks.size() >= MAX_FIREWORK) return;
        FireworkParticle fw;
        fw.x = x;
        fw.y = y;
        fw.vx = 0;
        fw.vy = vy;
        fw.color = GetColor(255, 150 + GetRand(100), 100 + GetRand(100));
        fw.life = 0;
        fw.maxLife = 60 + GetRand(30);
        fw.exploded = false;
        fireworks.push_back(fw);
    }

    inline void Update() {
        // ゲーム停止中、または待機状態なら何もしない
        if (GameSystem::GetInstance()->GetGameStatus() == GameStatus::Stop || step == 0) {
            return;
        }

        // --- 状態遷移ロジック ---
        if (step == 1) { // フェードイン
            alpha += 10; // 少し速める
            if (alpha >= 255) {
                alpha = 255;
                step = 2;
                stayTimer = 0;
            }
        }
        else if (step == 2) { // 維持（ここが長いと止まって見える）
            stayTimer++;
            // 120は長いかもしれないので、60（1秒）程度に短縮
            if (stayTimer > 60) {
                step = 3;
            }
        }
        else if (step == 3) { // フェードアウト
            alpha -= 5;
            if (alpha <= 0) {
                alpha = 0;
                step = 0; // ここで完全に終了
                fireworks.clear();
            }
        }

        // --- 花火の生成ロジック ---
        if (StageManager::GetInstance().floorCount > 50) {
            fireworkTimer++;
            if (fireworks.size() < 100 && fireworkTimer > 30) {
                fireworkTimer = 0;
                AddFirework(200 + GetRand(800), 700, -7.0f - GetRand(3));
            }
        }

        // --- パーティクルの更新 (追加用のテンポラリバッファを用意して安全にループ) ---
        std::vector<FireworkParticle> nextParticles;
        for (auto it = fireworks.begin(); it != fireworks.end(); ) {
            it->life++;

            if (!it->exploded) {
                it->y += it->vy;
                it->vy += 0.15f; // 重力

                // 頂点付近で爆発
                if (it->vy >= -1.0f || it->life > it->maxLife) {
                    it->exploded = true;
                    // 爆発火花の追加
                    for (int j = 0; j < 12; ++j) {
                        FireworkParticle p;
                        p.x = it->x; p.y = it->y;
                        float angle = (float)(DX_PI * 2 * j / 12);
                        float s = 2.0f + GetRand(20) / 10.0f;
                        p.vx = cosf(angle) * s;
                        p.vy = sinf(angle) * s;
                        p.color = it->color;
                        p.life = 0;
                        p.maxLife = 40 + GetRand(20);
                        p.exploded = true;
                        nextParticles.push_back(p);
                    }
                }
            }
            else {
                it->x += it->vx;
                it->y += it->vy;
                it->vy += 0.05f; // 重力
                it->vx *= 0.98f; // 空気抵抗
            }

            // 寿命が来たパーティクルを削除
            if (it->life > it->maxLife) {
                it = fireworks.erase(it);
            }
            else {
                ++it;
            }
        }
        // 新しく生成された爆発パーティクルを合流させる
        for (const auto& p : nextParticles) {
            if (fireworks.size() < MAX_FIREWORK) fireworks.push_back(p);
        }
    }

    inline void Draw() {
        if (step == 0 && alpha == 0) return;

        // 描画設定（停止中もここまでは通る）
        SetUseZBuffer3D(FALSE);
        SetWriteZBuffer3D(FALSE);
        SetUseLighting(FALSE);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

        // 背景帯
        DrawBox(0, 240, WINDOW_WIDTH, 360, GetColor(0, 0, 0), TRUE);

        // テキスト表示
        if (StageManager::GetInstance().floorCount > 50) {
            DrawExtendString(360, 270, 5, 5, "GOD SLAIN", yellow);
        }
        else {
            DrawExtendString(380, 270, 5, 5, "BOSS SLAIN", yellow);
        }

        // 花火の描画
        for (const auto& fw : fireworks) {
            int drawAlpha = alpha;
            if (fw.exploded) {
                // 爆発後は寿命に応じてさらにフェードアウト
                drawAlpha = (int)(alpha * (1.0f - (fw.life / fw.maxLife)));
            }
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, drawAlpha);
            DrawCircle((int)fw.x, (int)fw.y, fw.exploded ? 2 : 3, fw.color, TRUE);
        }

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        SetUseLighting(TRUE);
        SetUseZBuffer3D(TRUE);
        SetWriteZBuffer3D(TRUE);
    }
};
#pragma once
#include "DxLib.h"
#include <vector>
#include <string>


// ステータス個別のデータ構造
struct StatData {
    std::string name;   // 項目名
    int level;          // 現在のレベル (0 ~ 50)
    int totalBonus;     // 累積の上昇値 (素数を使わない加算値)
    unsigned int color; // ゲージの基本色
};

class StatusEnhancement {
private:
    std::vector<StatData> stats;
    int playerCoins;    // プレイヤーの所持金同期用
    float timer;        // 演出用タイマー
    bool allMax;        // 全MAXフラグ
    int selectedIndex;  // 現在選択中のインデックス

public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    StatusEnhancement();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~StatusEnhancement();

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Start();

    /// <summary>
    /// 更新処理
    /// </summary>
    bool Update();

    /// <summary>
    /// 描画処理
    /// </summary>
    void Render();

    /// <summary>
    /// 並行四辺形ゲージの描画
    /// </summary>
    /// <param name="x">描画基準X</param>
    /// <param name="y">描画基準Y</param>
    /// <param name="level">現在のレベル</param>
    /// <param name="baseColor">基本色</param>
    void DrawParallelGauge(int x, int y, int level, unsigned int baseColor);
};
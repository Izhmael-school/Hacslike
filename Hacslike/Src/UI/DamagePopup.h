#pragma once
#include <list>
#include <string>

// 前方宣言：Characterクラスが別の場所にあることを伝える
class Character;

class DamagePopup {
private:
    // メンバ変数
    float x, y, z;      // 3D空間上の位置
    int damage;         // 表示するダメージ数値
    bool isCritical;    // クリティカルかどうか
    int timer;          // 表示期間カウントダウン
    int alpha;          // 透明度 (0-255)
    std::string tag;

    // 全インスタンスを管理するリスト
    static std::list<DamagePopup*> instances;

    // コンストラクタ（外部からはCreate経由で呼ぶためprivate）
    DamagePopup(float x, float y, float z, int damage, bool isCritical, std::string tag);

public:
    // 初期化（必要に応じて）
    static void Init();

    /**
     * @brief ダメージポップアップを生成する
     * @param ch ダメージを受けたキャラクター（座標とGetCriticalを内部で取得）
     * @param damage ダメージ量
     */
    static void Create(Character* ch, int damage, bool isCritical);

    // 全インスタンスの更新（毎フレーム呼ぶ）
    static void UpdateAll();

    // 全インスタンスの描画（毎フレーム呼ぶ）
    static void RenderAll();

    // デストラクタ
    ~DamagePopup() {}
};
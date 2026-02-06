#include "DamagePopup.h"
#include "DxLib.h"
#include "../Definition.h"
#include "../GameObject/Character/Character.h" // Characterクラスの定義を読み込む

// 静的メンバ変数の実体定義
std::list<DamagePopup*> DamagePopup::instances;

void DamagePopup::Init() {
    // 必要に応じて初期化処理を記述（現在は空）
}

// コンストラクタ
DamagePopup::DamagePopup(float x, float y, float z, int damage, bool isCritical, std::string tag)
    : x(x), y(y), z(z), damage(damage), isCritical(isCritical), timer(60), alpha(255), tag(tag) {
}

// キャラクターを引数に取る生成関数
// 引数に bool isCritical を追加
void DamagePopup::Create(Character* ch, int damage, bool isCritical) {
    if (ch == nullptr) return;

    VECTOR pos = ch->GetPosition();
    float headOffset = 25.0f;

    std::string tag = ch->GetTag();

    // キャラクターのフラグ(ch->IsCritical())は見ず、
    // 引数で届いた「攻撃の結果としてのフラグ」を保存する
    instances.push_back(new DamagePopup(pos.x, pos.y + headOffset, pos.z, damage, isCritical, tag));
}

// 更新処理
void DamagePopup::UpdateAll() {
    auto it = instances.begin();
    while (it != instances.end()) {
        // 上に浮かせる動き
        (*it)->y += 0.5f;

        // タイマーを減らす
        (*it)->timer--;

        // 残り20フレームから徐々に透明にする
        if ((*it)->timer < 20) {
            (*it)->alpha -= 12;
            if ((*it)->alpha < 0) (*it)->alpha = 0;
        }

        // 削除判定
        if ((*it)->timer <= 0 || (*it)->alpha <= 0) {
            delete* it;
            it = instances.erase(it);
        }
        else {
            ++it;
        }
    }
}

// 描画処理
void DamagePopup::RenderAll() {
    for (auto p : instances) {
        // 3D座標をスクリーン座標に変換
        VECTOR worldPos = VGet(p->x, p->y, p->z);
        VECTOR screenPos = ConvWorldPosToScreenPos(worldPos);

       

        // カメラの視錐台の範囲内（画面内）にあるかチェック
        if (screenPos.z >= 0.0f && screenPos.z <= 1.0f) {

            unsigned int color;
            if (p->isCritical && p->tag == "Enemy") {
                color = yellow; // クリティカル：黄色
            }
            else if(p->tag == "Enemy"){
                color = white; // 通常：白
            }
            else if (p->isCritical && p->tag == "Player") {
                color = magenta; // 通常：白
            }
            else {
                color = red;
            }

            // 透明度の設定
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, p->alpha);

            // フォントの描画
            if (MainFont != -1 ) {
                // クリティカル時は少し太字にするなどの工夫も可能
                DrawFormatStringToHandle((int)screenPos.x, (int)screenPos.y, color, MainFont, "%d", p->damage);
            }
            else {
                DrawFormatString((int)screenPos.x, (int)screenPos.y, color, "%d", p->damage);
            }

            // ブレンドモードをリセット
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
    }
}
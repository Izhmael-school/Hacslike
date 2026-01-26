#pragma once
#include"Skill.h"
#include"DxLib.h"

class SkillSelectUI{
struct FireworkParticle {
    float x, y;
    float vx, vy;
    int color;
    float life;
    float maxLife;
    bool exploded;
};



private:
    std::vector<FireworkParticle> fireworks;
    int selectedIndex = 0;  //選択用のインデックス
    bool isActive = false;  //表示非表示
    int cardWidth = 260;    //横
    int cardHeight = 360;   //縦
    int inputCooldown = 0;   // 矢印キー連打防止クールタイム
    int hoverIndex = -1;

    // --- 出現アニメーション関連 ---
    bool isAppearing = false;  // アニメーション中かどうか
    float animTimer = 0.0f;    // 経過フレーム
    float animDuration = 30.0f; // 演出の総フレーム数（30で約0.5秒）

public:
    /// <summary>
    /// スタート
    /// </summary>
    void StartSelection();
    /// <summary>
    /// 更新
    /// </summary>
    /// <returns></returns>
    int UpdateSelection(const std::vector<std::shared_ptr<Skill>>& skills);
    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="skills"></param>
    void Render(const std::vector<std::shared_ptr<Skill>>& skills);
    bool IsActive() const { return isActive; }

    
};


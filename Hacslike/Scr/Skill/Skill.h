#pragma once
#include <string>
#include <memory>
#include <vector>
#include <iostream>

class Player;

class Skill{
protected: //メンバ変数
    std::string name;			//名前
    std::string description;	//説明
    std::string iconPath; // アイコン画像のパス

public:
    /// <summary>
    /// コンストラク
    /// </summary>
    /// <param name="name"></param>
    /// <param name="desc"></param>
    /// <param name="icon"></param>
    Skill(const std::string& name, const std::string& desc, const std::string& icon);

    /// <summary>
    /// デストラクタ
    /// </summary>
    virtual ~Skill() = default;

public://オーバーライドされる関数達
    virtual void Apply(Player* player) = 0;
    virtual void Remove(Player* player) = 0;

public://ゲッター
    const std::string& GetName() const { return name; }
    const std::string& GetDescription() const { return description; }
    const std::string& GetIconPath() const { return iconPath; }


};

//派生スキル

/// <summary>
/// 攻撃力増加スキル
/// </summary>
class AttackUpSkill : public Skill
{
    float attackBoost;
public:
    AttackUpSkill(float boost = 1.2f);
    void Apply(Player* player) override;
    void Remove(Player* player) override;
};

/// <summary>
/// HP上限増加スキル
/// </summary>
class HPUpSkill : public Skill
{
    int hpIncrease;
public:
    HPUpSkill(int hp = 50);
    void Apply(Player* player) override;
    void Remove(Player* player) override;
};


/// <summary>
/// 速度増加スキル
/// </summary>
class SpeedUpSkill : public Skill
{
    float speedBoost;
public:
    SpeedUpSkill(float boost = 0.3f);
    void Apply(Player* player) override;
    void Remove(Player* player) override;
};

/// <summary>
/// 防御力増加スキル
/// </summary>
class DefenseUpSkill : public Skill {
    int defenseBoost;
public:
    DefenseUpSkill(int boost = 5);
    void Apply(Player* player) override;
    void Remove(Player* player) override;

};


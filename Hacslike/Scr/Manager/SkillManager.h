#pragma once
#include"../Skill/Skill.h"
#include "../Component/Singleton.h"
#include <vector>
#include <memory>
#include <random>

class Player;

class SkillManager : public Singleton<SkillManager>{
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    SkillManager();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~SkillManager();


    // ======== スキル関連処理 ========
private:
    std::vector<std::shared_ptr<Skill>> activeSkills; // 現在有効なスキル
    std::vector<std::shared_ptr<Skill>> skillPool;    // 全スキルの候補

public:
    // 候補スキルを3つ生成して返す（重複なし）
    std::vector<std::shared_ptr<Skill>> GenerateSkillChoices();

    // 選択されたスキルを適用
    void ApplySelectedSkill(Player* player, std::shared_ptr<Skill> skill);

    // 全スキルをクリア（ダンジョン終了時など）
    void ClearSkills(Player* player);

    const std::vector<std::shared_ptr<Skill>>& GetActiveSkills() const { return activeSkills; }
};


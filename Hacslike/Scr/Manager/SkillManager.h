#pragma once
#include"../Skill/Skill.h"
#include <vector>
#include <memory>
#include <random>

class Player;

class SkillManager{
private:
    // ======== シングルトン関連 ========
    static SkillManager* pInstance;  // 自身のインスタンスのアドレス

    /// <summary>
    /// コンストラクタ（外部から生成禁止）
    /// </summary>
    SkillManager();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~SkillManager();

public:
    // コピー・ムーブ禁止
    SkillManager(const SkillManager&) = delete;
    SkillManager& operator=(const SkillManager&) = delete;
    SkillManager(SkillManager&&) = delete;
    SkillManager& operator=(SkillManager&&) = delete;

private:
    /// <summary>
    /// インスタンス生成（内部用）
    /// </summary>
    static void CreateInstance();

public:
    /// <summary>
    /// インスタンス取得（唯一のアクセス手段）
    /// </summary>
    static SkillManager* GetInstance();

    /// <summary>
    /// インスタンス破棄
    /// </summary>
    static void DestroyInstance();

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


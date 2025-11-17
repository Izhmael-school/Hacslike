#include "SkillManager.h"
#include"../GameObject/Character/Player/Player.h"
#include <ctime>
#include <algorithm>
#include <iostream>
#include "AudioManager.h"

// ------------------------
// コンストラクタ
// ------------------------
SkillManager::SkillManager()
{
    // スキルプールを初期化
    skillPool = {
        std::make_shared<AttackUpSkill>(),
        std::make_shared<HPUpSkill>(),
        std::make_shared<SpeedUpSkill>(),
        std::make_shared<DefenseUpSkill>(),
        std::make_shared<ProximityCorrectionUpSkill>(),
        std::make_shared<RangedCorrectionUpSkill>(),
        std::make_shared<ItemDropRateUpSkill>(),
        std::make_shared<CriticalHitRateUpSkill>(),
        std::make_shared<CriticalDamageUpSkill>(),
        std::make_shared<IsDashAttack>(), // これは1回でMAX
    };
    Start();
}

// ------------------------
// デストラクタ
// ------------------------
SkillManager::~SkillManager()
{
    activeSkills.clear();
    skillPool.clear();
}

void SkillManager::Start()
{
    AudioManager::GetInstance()->Load("Res/SE/決定ボタンを押す10.mp3", "SkillGet", false);
}

// ------------------------
// スキル3つ生成（最大段階のスキルは除外）
// ------------------------
std::vector<std::shared_ptr<Skill>> SkillManager::GenerateSkillChoices()
{
    AudioManager::GetInstance()->PlayOneShot("SkillGet");
    std::vector<std::shared_ptr<Skill>> candidates;

    // MAX到達していないスキルだけ選ぶ
    for (auto& s : skillPool)
    {
        if (!s->IsMaxLevel())
            candidates.push_back(s);
    }

    // 全部MAXだったら選択肢無し
    if (candidates.empty()) {
        return {};
    }

    // ランダム選択
    std::vector<std::shared_ptr<Skill>> choices;
    std::mt19937 rng((unsigned int)time(nullptr));

    std::shuffle(candidates.begin(), candidates.end(), rng);

    for (int i = 0; i < 3 && i < (int)candidates.size(); ++i)
    {
        choices.push_back(candidates[i]);
    }
    return choices;
}

// ------------------------
// スキル適用
// ------------------------
void SkillManager::ApplySelectedSkill(Player* player, std::shared_ptr<Skill> skill)
{
    if (!player || !skill) return;

    skill->Apply(player);
    activeSkills.push_back(skill);

    std::cout << "スキル獲得: " << skill->GetName()
        << " (Lv." << skill->GetLevel() << "/" << skill->GetMaxLevel() << ")\n";
}

// ------------------------
// 全スキル削除（デバッグ用）
// ------------------------
void SkillManager::ClearSkills(Player* player)
{
    for (auto& s : activeSkills)
    {
        s->ClearLevel();     // レベルを1に戻す
        s->Remove(player);   // 効果解除
    }
    activeSkills.clear();

    std::cout << "スキルを全てリセットしました。\n";
}
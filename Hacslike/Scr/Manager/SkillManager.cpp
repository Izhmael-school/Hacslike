#include "SkillManager.h"
#include"../GameObject/Character/Player/Player.h"
#include <ctime>
#include <algorithm>
#include <iostream>

// ------------------------
// 静的メンバ初期化
// ------------------------
SkillManager* SkillManager::pInstance = nullptr;

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
    };
}

// ------------------------
// デストラクタ
// ------------------------
SkillManager::~SkillManager()
{
    activeSkills.clear();
    skillPool.clear();
}

// ------------------------
// インスタンス生成
// ------------------------
void SkillManager::CreateInstance()
{
    if (!pInstance)
    {
        pInstance = new SkillManager();
    }
}

// ------------------------
// インスタンス取得
// ------------------------
SkillManager* SkillManager::GetInstance()
{
    if (!pInstance)
    {
        CreateInstance();
    }
    return pInstance;
}

// ------------------------
// インスタンス破棄
// ------------------------
void SkillManager::DestroyInstance()
{
    if (pInstance)
    {
        delete pInstance;
        pInstance = nullptr;
    }
}

// ------------------------
// スキル3つ生成
// ------------------------
std::vector<std::shared_ptr<Skill>> SkillManager::GenerateSkillChoices()
{
    std::vector<std::shared_ptr<Skill>> choices;
    std::mt19937 rng((unsigned int)time(nullptr));

    // スキルプールをシャッフル
    std::vector<std::shared_ptr<Skill>> shuffled = skillPool;
    std::shuffle(shuffled.begin(), shuffled.end(), rng);

    for (int i = 0; i < 3 && i < (int)shuffled.size(); ++i)
    {
        choices.push_back(shuffled[i]);
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

    std::cout << "スキル獲得: " << skill->GetName() << std::endl;
}

// ------------------------
// 全スキル削除
// ------------------------
void SkillManager::ClearSkills(Player* player)
{
    for (auto& s : activeSkills)
    {
        s->Remove(player);
    }
    activeSkills.clear();

    std::cout << "スキルを全てリセットしました。\n";
}

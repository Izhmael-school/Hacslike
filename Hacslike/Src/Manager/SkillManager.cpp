#include "SkillManager.h"
#include"../GameObject/Character/Player/Player.h"
#include <ctime>
#include <algorithm>
#include <iostream>
#include "AudioManager.h"
#include"../Save/SaveIO.h"


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
        //std::make_shared<RangedCorrectionUpSkill>(),
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
    AudioManager::GetInstance().Load("Res/SE/決定ボタンを押す10.mp3", "SkillGet", false);
}

// ------------------------
// スキル3つ生成（最大段階のスキルは除外）
// ------------------------
std::vector<std::shared_ptr<Skill>> SkillManager::GenerateSkillChoices()
{
    AudioManager::GetInstance().PlayOneShot("SkillGet");
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

void SkillManager::SaveTo(BinaryWriter& w)
{
    uint32_t poolCount = static_cast<uint32_t>(skillPool.size());
    w.WritePOD(poolCount);
    for (auto& s : skillPool) {
        int id = s->GetID();
        int lv = s->GetLevel();
        w.WritePOD(id);
        w.WritePOD(lv);
    }
    // activeSkills の ID リストも保存（どのスキルがアクティブか）
    uint32_t activeCount = static_cast<uint32_t>(activeSkills.size());
    w.WritePOD(activeCount);
    for (auto& s : activeSkills) {
        int id = s->GetID();
        w.WritePOD(id);
    }

    std::cout << "[SkillManager] Saved " << poolCount << " skills, " << activeCount << " active\n";
}

void SkillManager::LoadFrom(BinaryReader& r, uint32_t ver)
{
    // skillPool 側の情報（ID, level）
    uint32_t poolCount = 0;
    r.ReadPOD(poolCount);
    for (uint32_t i = 0; i < poolCount; ++i) {
        int id = 0;
        int lv = 0;
        r.ReadPOD(id);
        r.ReadPOD(lv);
        auto it = std::find_if(skillPool.begin(), skillPool.end(), [&](const std::shared_ptr<Skill>& s) {
            return s->GetID() == id;
            });
        if (it != skillPool.end()) {
            (*it)->SetLevel(lv);
        }
        else {
            std::cout << "[SkillManager] Unknown skill id in save: " << id << "\n";
        }
    }

    // active list を復元（ID -> skillPool 内の shared_ptr を参照）
    uint32_t activeCount = 0;
    r.ReadPOD(activeCount);
    activeSkills.clear();
    for (uint32_t i = 0; i < activeCount; ++i) {
        int id = 0;
        r.ReadPOD(id);
        auto it = std::find_if(skillPool.begin(), skillPool.end(), [&](const std::shared_ptr<Skill>& s) {
            return s->GetID() == id;
            });
        if (it != skillPool.end()) {
            activeSkills.push_back(*it);
        }
        else {
            std::cout << "[SkillManager] Unknown active skill id in save: " << id << "\n";
        }
    }

    // プレイヤーに効果を反映（Player がロード済みであることが前提）
    Player* player = Player::GetInstance();
    if (player) {
        for (auto& s : activeSkills) {
            s->ApplyAllLevels(player);
        }
    }

    std::cout << "[SkillManager] Loaded. poolCount=" << poolCount << " activeCount=" << activeCount << "\n";
}



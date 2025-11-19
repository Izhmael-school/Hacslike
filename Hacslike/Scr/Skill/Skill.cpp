#include "Skill.h"
#include "../GameObject/Character/Player/Player.h"


// =============================
//   Skill 基底クラス
// =============================
Skill::Skill(const std::string& name, const std::string& desc, const std::string& icon, int maxLv)
    : name(name)
    , description(desc)
    , iconPath(icon)
    , maxLevel(maxLv)
{
}

void Skill::Apply(Player* player) {}
void Skill::Remove(Player* player) {}
void Skill::ClearLevel(){}

// =============================
//   攻撃力増加
// =============================
AttackUpSkill::AttackUpSkill(float boost)
    : Skill("攻撃力アップ", "攻撃力が20％上昇する", "Res/SkillIcon/attack_up.png", 5)
    , attackBoost(boost)
{
}

void AttackUpSkill::Apply(Player* player)
{
    LevelUp();
    if (!player) return;
    player->SetAtk(player->GetAtk() + attackBoost);
}

void AttackUpSkill::Remove(Player* player)
{
    if (!player) return;
    player->SetAtk(player->GetAtk() - attackBoost);
}

void AttackUpSkill::ClearLevel()
{
    level = 0;
}


// =============================
//   HP 上限増加
// =============================
HPUpSkill::HPUpSkill(int hp)
    : Skill("HP上限アップ", "HP上限を50上昇する", "Res/SkillIcon/hp_up.png", 5)
    , hpIncrease(hp)
{
}

void HPUpSkill::Apply(Player* player)
{
    LevelUp();
    if (!player) return;
    player->SetMaxHp(player->GetMaxHp() + hpIncrease);
}

void HPUpSkill::Remove(Player* player)
{
    if (!player) return;
    player->SetMaxHp(player->GetMaxHp() - hpIncrease);
}

void HPUpSkill::ClearLevel()
{
    level = 0;
}

// =============================
//   移動速度上昇
// =============================
SpeedUpSkill::SpeedUpSkill(float boost)
    : Skill("移動速度アップ", "移動速度が30％上昇する", "Res/SkillIcon/speed_up.png", 5)
    , speedBoost(boost)
{
}

void SpeedUpSkill::Apply(Player* player)
{
    LevelUp();
    if (!player) return;
    player->SetSpeed(player->GetSpeed() + speedBoost);
}

void SpeedUpSkill::Remove(Player* player)
{
    if (!player) return;
    player->SetSpeed(player->GetSpeed() - speedBoost);
}

void SpeedUpSkill::ClearLevel()
{
    level = 0;
}

// =============================
//   防御力上昇
// =============================
DefenseUpSkill::DefenseUpSkill(int boost)
    : Skill("防御力アップ", "防御力が5上昇する", "Res/SkillIcon/Defense_up.png", 5)
    , defenseBoost(boost)
{
}

void DefenseUpSkill::Apply(Player* player)
{
    LevelUp();
    if (!player) return;
    player->SetDef(player->GetDef() + defenseBoost);
}

void DefenseUpSkill::Remove(Player* player)
{
    if (!player) return;
    player->SetDef(player->GetDef() - defenseBoost);
}

void DefenseUpSkill::ClearLevel()
{
    level = 0;
}

// =============================
//   近距離補正 UP
// =============================
ProximityCorrectionUpSkill::ProximityCorrectionUpSkill(float boost)
    : Skill("近距離攻撃力アップ", "近距離攻撃力が5上昇する", "Res/SkillIcon/ProximityCorrectionUp.png", 5)
    , ProximityCorrectionBoost(boost)
{
}

void ProximityCorrectionUpSkill::Apply(Player* player)
{
    LevelUp();
    if (!player) return;
    player->SetProximityCorrection(player->GetProximityCorrection() + ProximityCorrectionBoost);
}

void ProximityCorrectionUpSkill::Remove(Player* player)
{
    if (!player) return;
    player->SetProximityCorrection(player->GetProximityCorrection() - ProximityCorrectionBoost);
}

void ProximityCorrectionUpSkill::ClearLevel()
{
    level = 0;
}

// =============================
//   遠距離補正 UP
// =============================
RangedCorrectionUpSkill::RangedCorrectionUpSkill(float boost)
    : Skill("遠距離攻撃力アップ", "遠距離攻撃力が5上昇する", "Res/SkillIcon/RangedCorrectionUp.png", 5)
    , RangedCorrectionBoost(boost)
{
}

void RangedCorrectionUpSkill::Apply(Player* player)
{
    LevelUp();
    if (!player) return;
    player->SetRangedCorrection(player->GetRangedCorrection() + RangedCorrectionBoost);
}

void RangedCorrectionUpSkill::Remove(Player* player)
{
    if (!player) return;
    player->SetRangedCorrection(player->GetRangedCorrection() - RangedCorrectionBoost);
}

void RangedCorrectionUpSkill::ClearLevel()
{
    level = 0;
}

// =============================
//   アイテムドロップ率
// =============================
ItemDropRateUpSkill::ItemDropRateUpSkill(float boost)
    : Skill("アイテムドロップ率アップ", "アイテムドロップ率が\n10%上昇する", "Res/SkillIcon/ItemDropRate.png", 5)
    , rateBoost(boost)
{
}

void ItemDropRateUpSkill::Apply(Player* player)
{
    LevelUp();
    ItemDropManager::GetInstance().SetItemDropRate(
        ItemDropManager::GetInstance().GetItemDropRate() + rateBoost
    );
}

void ItemDropRateUpSkill::Remove(Player* player)
{
    ItemDropManager::GetInstance().SetItemDropRate(
        ItemDropManager::GetInstance().GetItemDropRate() - rateBoost
    );
}

void ItemDropRateUpSkill::ClearLevel()
{
    level = 0;
}

// =============================
//   会心率 UP
// =============================
CriticalHitRateUpSkill::CriticalHitRateUpSkill(float boost)
    : Skill("会心率アップ", "会心率が10上昇する", "Res/SkillIcon/CriticalUP.png", 5)
    , CriticalHitRateBoost(boost)
{
}

void CriticalHitRateUpSkill::Apply(Player* player)
{
    LevelUp();
    if (!player) return;
    player->SetCriticalHitRate(player->GetCriticalHitRate() + CriticalHitRateBoost);
}

void CriticalHitRateUpSkill::Remove(Player* player)
{
    if (!player) return;
    player->SetCriticalHitRate(player->GetCriticalHitRate() - CriticalHitRateBoost);
}

void CriticalHitRateUpSkill::ClearLevel()
{
    level = 0;
}

// =============================
//   会心ダメージ UP
// =============================
CriticalDamageUpSkill::CriticalDamageUpSkill(float boost)
    : Skill("会心ダメージアップ", "会心ダメージが15上昇する", "Res/SkillIcon/CriticalDamageUP.png", 5)
    , CriticalDamageBoost(boost)
{
}

void CriticalDamageUpSkill::Apply(Player* player)
{
    LevelUp();
    if (!player) return;
    player->SetCriticalDamage(player->GetCriticalDamage() + CriticalDamageBoost);
}

void CriticalDamageUpSkill::Remove(Player* player)
{
    if (!player) return;
    player->SetCriticalDamage(player->GetCriticalDamage() - CriticalDamageBoost);
}

void CriticalDamageUpSkill::ClearLevel()
{
    level = 0;
}

// =============================
//   ダッシュ攻撃（1回のみ）
// =============================
IsDashAttack::IsDashAttack()
    : Skill("ダッシュ攻撃取得", "ダッシュ攻撃が可能になる", "Res/SkillIcon/dashAttack.png", 1)
    ,isDashAttack(false){
}

void IsDashAttack::Apply(Player* player)
{
    LevelUp();
    if (!player) return;
    player->GetPlayerAttack()->SetIsCheckDashAttack(true);
}

void IsDashAttack::Remove(Player* player)
{
    if (!player) return;
    player->GetPlayerAttack()->SetIsCheckDashAttack(false);
}

void IsDashAttack::ClearLevel()
{
    level = 0;
}
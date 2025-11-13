#include "Skill.h"
#include"../GameObject/Character/Player/Player.h"

class Player;

Skill::Skill(const std::string& name, const std::string& desc, const std::string& icon)
    : name(name)
    , description(desc)
    , iconPath(icon) {
}

void Skill::Apply(Player* player)
{
}

void Skill::Remove(Player* player)
{
}

#pragma region 攻撃力増加
AttackUpSkill::AttackUpSkill(float boost)
    : Skill("攻撃力アップ", "攻撃力が上昇する", "Res/SkillIcon/attack_up.png")
    , attackBoost(boost)
{
}

void AttackUpSkill::Apply(Player* player)
{
    if (!player) return;
    player->SetBaseAtk(player->GetBaseAtk() + attackBoost);

}

void AttackUpSkill::Remove(Player* player)
{
    if (!player) return;
    player->SetBaseAtk(player->GetBaseAtk() / attackBoost);

}

#pragma endregion 

#pragma region HP上限増加
HPUpSkill::HPUpSkill(int hp)
    :Skill("HP上限アップ", "HP上限が上昇する", "Res/SkillIcon/hp_up.png")
    , hpIncrease(hp)
{
}

void HPUpSkill::Apply(Player* player)
{
    if (!player) return;
    player->SetMaxHp(player->GetMaxHp() + hpIncrease);
}

void HPUpSkill::Remove(Player* player)
{
    if (!player) return;
    player->SetMaxHp(player->GetMaxHp() / hpIncrease);
}
#pragma endregion

#pragma region 移動速度上昇
SpeedUpSkill::SpeedUpSkill(float boost)
    :Skill("移動速度アップ", "移動速度が上昇する", "Res/SkillIcon/speed_up.png")
    , speedBoost(boost) {
}

void SpeedUpSkill::Apply(Player* player){
    if (!player) return;
    player->SetSpeed(player->GetSpeed() + speedBoost);
}

void SpeedUpSkill::Remove(Player* player){
    if (!player) return;
    player->SetSpeed(player->GetSpeed() / speedBoost);
}
#pragma endregion

#pragma region 防御力上昇
DefenseUpSkill::DefenseUpSkill(int boost)
    :Skill("防御力アップ", "防御力が上昇する", "Res/SkillIcon/Defense_up.png")
    , defenseBoost(boost) {
}

void DefenseUpSkill::Apply(Player* player)
{
    if (!player) return;
    player->SetDef(player->GetDef() + defenseBoost);
}

void DefenseUpSkill::Remove(Player* player)
{
    if (!player) return;
    player->SetDef(player->GetDef() / defenseBoost);
}
#pragma endregion

#pragma region 近距離補正を上げるスキル
ProximityCorrectionUpSkill::ProximityCorrectionUpSkill(float boost)
    :Skill("近距離攻撃力アップ", "近距離攻撃力が上昇する", "Res/SkillIcon/ProximityCorrectionUp.png")
    , ProximityCorrectionBoost(boost) {
}

void ProximityCorrectionUpSkill::Apply(Player* player)
{
    if (!player)return;
    player->SetProximityCorrection(player->GetProximityCorrection() + ProximityCorrectionBoost);
}

void ProximityCorrectionUpSkill::Remove(Player* player)
{
    if (!player)return;
    player->SetProximityCorrection(player->GetProximityCorrection() - ProximityCorrectionBoost);
}
#pragma endregion

#pragma region 遠距離補正を上げるスキル
RangedCorrectionUpSkill::RangedCorrectionUpSkill(float boost)
    :Skill("遠距離攻撃力アップ", "遠距離攻撃力が上昇する", "Res/SkillIcon/RangedCorrectionUp.png")
    , RangedCorrectionBoost(boost) {
}

void RangedCorrectionUpSkill::Apply(Player* player)
{
    if (!player)return;
    player->SetRangedCorrection(player->GetRangedCorrection() + RangedCorrectionBoost);
}

void RangedCorrectionUpSkill::Remove(Player* player)
{
    if (!player)return;
    player->SetRangedCorrection(player->GetRangedCorrection() - RangedCorrectionBoost);
}
#pragma endregion

#pragma region アイテムドロップ確率
ItemDropRateUpSkill::ItemDropRateUpSkill(float boost)
    :Skill("アイテムドロップ率アップ", "アイテムドロップ率が上昇する", "Res/SkillIcon/ItemDropRate.png")
    , rateBoost(boost) {
}

void ItemDropRateUpSkill::Apply(Player* player)
{
    ItemDropManager::GetInstance().SetItemDropRate(ItemDropManager::GetInstance().GetItemDropRate() + rateBoost);
}

void ItemDropRateUpSkill::Remove(Player* player)
{
    ItemDropManager::GetInstance().SetItemDropRate(ItemDropManager::GetInstance().GetItemDropRate() - rateBoost);
}
#pragma endregion 

#pragma region 会心率上昇
CriticalHitRateUpSkill::CriticalHitRateUpSkill(float boost)
    :Skill("会心率アップ", "会心率が上昇する", "Res/SkillIcon/CriticalUP.png")
    , CriticalHitRateBoost(boost) {
}

void CriticalHitRateUpSkill::Apply(Player* player)
{
    if (!player)return;
    player->SetCriticalHitRate(player->GetCriticalHitRate() + CriticalHitRateBoost);
}

void CriticalHitRateUpSkill::Remove(Player* player)
{
    if (!player)return;
    player->SetCriticalHitRate(player->GetCriticalHitRate() - CriticalHitRateBoost);
}
#pragma endregion

CriticalDamageUpSkill::CriticalDamageUpSkill(float boost)
    :Skill("会心ダメージアップ", "会心ダメージが上昇する", "Res/SkillIcon/CriticalDamageUP.png")
    , CriticalDamageBoost(boost) {
}

void CriticalDamageUpSkill::Apply(Player* player)
{
    if (!player) return;
    player->SetCriticalDamage(player->GetCriticalDamage() + CriticalDamageBoost);
}

void CriticalDamageUpSkill::Remove(Player* player)
{
    if (!player) return;
    player->SetCriticalDamage(player->GetCriticalDamage() - CriticalDamageBoost);
}

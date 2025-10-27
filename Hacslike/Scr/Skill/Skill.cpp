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
    player->SetAtk(player->GetAtk() * attackBoost);

}

void AttackUpSkill::Remove(Player* player)
{
    if (!player) return;
    player->SetAtk(player->GetAtk() / attackBoost);
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
    player->SetHp(player->GetHp() * hpIncrease);
}

void HPUpSkill::Remove(Player* player)
{
    if (!player) return;
    player->SetHp(player->GetHp() / hpIncrease);
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
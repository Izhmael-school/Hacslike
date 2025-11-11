#include "ArtifactBase.h"
#include"../Character/Player/Player.h"
#include"../../Manager/TimeManager.h"
#include"../Character/Player/PlayerMovement.h"

ArtifactBase::ArtifactBase(const std::string& _name, const std::string& _desc, const std::string& _icon)
    :name(_name)
    , description(_desc)
    , iconPath(_icon)
    , iconHandle(-1) {
    if (!iconPath.empty()) {
        iconHandle = LoadGraph(iconPath.c_str());
    }
}

void ArtifactBase::Apply(Player* player)
{
}

void ArtifactBase::Remove(Player* player)
{
}

#pragma region HPが一定値以上だと攻撃力上昇
conditional_attack_power_raise_HP::conditional_attack_power_raise_HP(float boost)
    :ArtifactBase("不屈の印章", "HPが7割り以上の時攻撃力上昇", "Res/ArtifactIcon/Attact_HP.png")
    , attackBoost(boost)
    , isBoosted(false) {
}

void conditional_attack_power_raise_HP::Update(Player* player)
{
    if (!player) return;

    float hpRate = static_cast<float>(player->GetHp()) / player->GetMaxHp();

    // HPが7割以上 → 攻撃力アップ
    if (hpRate >= 0.7f && !isBoosted)
    {
        player->SetAtk(player->GetAtk() * attackBoost);
        isBoosted = true;
    }
    // HPが7割未満 → 攻撃力を元に戻す
    else if (hpRate < 0.7f && isBoosted)
    {
        player->SetAtk(player->GetAtk() / attackBoost);
        isBoosted = false;
    }
}

void conditional_attack_power_raise_HP::Apply(Player* player)
{
    Update(player);
}

void conditional_attack_power_raise_HP::Remove(Player* player)
{
    if (isBoosted && player)
    {
        player->SetAtk(player->GetAtk() / attackBoost);
        isBoosted = false;
    }
}
#pragma endregion

#pragma region HP上限上昇
Hp_Max_Raise::Hp_Max_Raise(int boost)
    :ArtifactBase("生命の宝珠", "HP上限が大幅上昇", "Res/ArtifactIcon/Max_HP_Raise.png")
    , MaxHpBoost(boost) {

}

void Hp_Max_Raise::Update(Player* player)
{
}

void Hp_Max_Raise::Apply(Player* player)
{
    if (!player) return;
    player->SetMaxHp(player->GetMaxHp() + MaxHpBoost);
    player->SetHp(player->GetMaxHp());

}

void Hp_Max_Raise::Remove(Player* player)
{
    if (!player) return;
    player->SetMaxHp(player->GetMaxHp() / MaxHpBoost);
}
#pragma endregion

#pragma region コインの取得量上昇
CoinValue_raise::CoinValue_raise(int value)
    :ArtifactBase("富豪への道", "コインの取得量上昇", "Res/ArtifactIcon/Coin.png")
    , UpValue(value) {
}

void CoinValue_raise::Update(Player* player)
{
}

void CoinValue_raise::Apply(Player* player)
{
    if (!player) return;
    player->SetCoinAcquisitionValue(player->GetCoinAcquisitionValue() + UpValue);
}

void CoinValue_raise::Remove(Player* player)
{
}
#pragma endregion

#pragma region HPが一定値以下だと防御力上昇
conditional_defense_power_raise_HP::conditional_defense_power_raise_HP(float boost)
    :ArtifactBase("鋼の意志", "HPが半分以下だと防御力上昇", "Res/ArtifactIcon/Defense_HP.png")
    , defenseBoost(boost)
    , isDefBoosted(false) {
}

void conditional_defense_power_raise_HP::Update(Player* player)
{
    if (!player) return;

    float HpRate = static_cast<float>(player->GetHp()) / player->GetMaxHp();

    // HPが7割以上 → 攻撃力アップ
    if (HpRate >= 0.5f && isDefBoosted)
    {
        player->SetDef(player->GetDef() / defenseBoost);
        isDefBoosted = false;
    }
    // HPが7割未満 → 攻撃力を元に戻す
    else if (HpRate < 0.5f && !isDefBoosted)
    {
        player->SetDef(player->GetDef() * defenseBoost);
        isDefBoosted = true;
    }
}

void conditional_defense_power_raise_HP::Apply(Player* player)
{
    Update(player);
}

void conditional_defense_power_raise_HP::Remove(Player* player)
{
    if (isDefBoosted && player)
    {
        player->SetDef(player->GetDef() / defenseBoost);
        isDefBoosted = false;
    }
}
#pragma endregion

#pragma region コイン取得時に攻撃力が少し上昇
attactPower_raise_GetCoin::attactPower_raise_GetCoin(float boost, float time)
    :ArtifactBase("金の力", "コイン取得時攻撃力上昇", "Res/ArtifactIcon/UpAttact_Coin.png")
    , boostAmount(boost)
    , duration(time)
    , timer(0.0f)
    , isBoosted(false)
    , originalAtk(0) {
}

void attactPower_raise_GetCoin::OnGetCoin(Player* player)
{
    if (!player) return;

    // 既に上昇中ならタイマーをリセットする（重ねがけ防止）
    if (isBoosted) {
        timer = duration;
        return;
    }
    originalAtk = player->GetAtk(); // 元の攻撃力を記録
    player->SetBaseAtk(originalAtk * boostAmount);
    timer = duration;
    isBoosted = true;
}

void attactPower_raise_GetCoin::Update(Player* player)
{
    TimeManager* time = &TimeManager::GetInstance();

    if (!isBoosted || !player) return;

    timer -= time->deltaTime;  // 経過時間を減らす

    if (timer <= 0.0f) {
        // 効果終了
        player->SetBaseAtk(originalAtk);
        isBoosted = false;
        timer = 0.0f;
    }
}

void attactPower_raise_GetCoin::Apply(Player* player)
{

    player->SetCoinArtifact(this);
    Update(player);
}

void attactPower_raise_GetCoin::Remove(Player* player)
{
    player->SetBaseAtk(originalAtk);

}
#pragma endregion

#pragma region コイン取得時にアイテムドロップ率が上昇
itemDropRateUpwardOnCoinAcquisition::itemDropRateUpwardOnCoinAcquisition(float boost, float time)
    :ArtifactBase("強運の連鎖", "コイン取得時\nアイテムドロップ率上昇", "Res/ArtifactIcon/DropRateUpwaed.png")
    , dropRateUpward(boost)
    , duration(time)
    , isBoost(false)
    , timer(0.0f)
{
}

void itemDropRateUpwardOnCoinAcquisition::OnGetCoin_Item(Player* player)
{
    if (!player) return;

    // 既に上昇中ならタイマーをリセットする（重ねがけ防止）
    if (isBoost) {
        timer = duration;
        return;
    }
    ItemDropManager::GetInstance()->SetItemDropRate(ItemDropManager::GetInstance()->GetItemDropRate() + dropRateUpward);
    timer = duration;
    isBoost = true;
}

void itemDropRateUpwardOnCoinAcquisition::Update(Player* player)
{
    TimeManager* time = &TimeManager::GetInstance();

    if (!isBoost || !player) return;

    timer -= time->deltaTime;  // 経過時間を減らす

    if (timer <= 0.0f && isBoost) {
        // 効果終了
        ItemDropManager::GetInstance()->SetItemDropRate(ItemDropManager::GetInstance()->GetItemDropRate() - dropRateUpward);
        isBoost = false;
        timer = 0.0f;
    }


}

void itemDropRateUpwardOnCoinAcquisition::Apply(Player* player)
{
    player->SetItemArtifact(this);
    Update(player);
}

void itemDropRateUpwardOnCoinAcquisition::Remove(Player* player)
{
    ItemDropManager::GetInstance()->SetItemDropRate(ItemDropManager::GetInstance()->GetItemDropRate() - dropRateUpward);
}
#pragma endregion

#pragma region  回避時攻撃力上昇
AttackincreasesforSeveralSecondsAfterEvasion::AttackincreasesforSeveralSecondsAfterEvasion(float boost, float time)
    :ArtifactBase("刹那の指輪", "回避後数秒間攻撃上昇", "Res/ArtifactIcon/Attack_UP.png")
    , attackPowerUp(boost)
    , duration(time)
    , isBoost(false)
    , timer(0.0f)
    , originalAtk(0) {
}

/// <summary>
/// PlayerMovementで呼んでいる
/// 回避した時
/// </summary>
/// <param name="playermove"></param>
void AttackincreasesforSeveralSecondsAfterEvasion::OnBlinking(PlayerMovement* playermove)
{
        timer = duration;
        isBoost = true;
}

void AttackincreasesforSeveralSecondsAfterEvasion::Update(Player* player)
{
    if (PlayerMovement::GetInstance()->IsBlinking()) {
        // 常にリセット（再回避で延長される）
    }
  
    if (!player) return;

    TimeManager* time = &TimeManager::GetInstance();


    if (isBoost)
    {
        // 初回のみ攻撃力上昇を適用
        if (timer == duration)
        {
            originalAtk = player->GetAtk();
            player->SetAtk(originalAtk * attackPowerUp);
        }

        // 時間経過
        timer -= time->deltaTime;

        // 終了処理
        if (timer <= 0.0f)
        {
            player->SetAtk(originalAtk);
            isBoost = false;
            timer = 0.0f;
        }
    }
}

void AttackincreasesforSeveralSecondsAfterEvasion::Apply(Player* player)
{
    PlayerMovement::GetInstance()->SetAttactArtifact(this);
    Update(player);
}

void AttackincreasesforSeveralSecondsAfterEvasion::Remove(Player* player)
{
    if (isBoost && player)
    {
        player->SetBaseAtk(originalAtk);
        isBoost = false;
        timer = 0.0f;
    }
}
#pragma endregion
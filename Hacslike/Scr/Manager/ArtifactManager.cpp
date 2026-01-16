#include "ArtifactManager.h"
#include <random>
#include <DxLib.h>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "AudioManager.h"
#include "../GameObject/Character/Player/Player.h"
#include "../Save/SaveIO.h" 


ArtifactManager::ArtifactManager()
{
    artifacrPool = {
        //ここに作ったアーチファクトを入れていく
        std::make_shared<conditional_attack_power_raise_HP>(),
        std::make_shared<Hp_Max_Raise>(),
        std::make_shared<CoinValue_raise>(),
        std::make_shared<conditional_defense_power_raise_HP>(),
        std::make_shared<attactPower_raise_GetCoin>(),
        std::make_shared<itemDropRateUpwardOnCoinAcquisition>(),
        std::make_shared<AttackincreasesforSeveralSecondsAfterEvasion>(),
        std::make_shared<CriticalHitRateIncreasesForSeveralSecondsAfterEvasion>(),

    };
    Start();
}

ArtifactManager::~ArtifactManager()
{
    activeArtifact.clear();
    artifacrPool.clear();
}


void ArtifactManager::Start()
{
    AudioManager::GetInstance().Load("Res/SE/決定ボタンを押す36.mp3", "ArtifactGet", false);
}


/// <summary>
/// ボスを討伐後の選択式アーティファクト
/// </summary>
/// <param name="artifact"></param>
std::vector<std::shared_ptr<ArtifactBase>> ArtifactManager::GenerateArtifactChoices()
{

    AudioManager::GetInstance().PlayOneShot("ArtifactGet");

    std::vector<std::shared_ptr<ArtifactBase>> choices;

    if (artifacrPool.empty())
    {
#if _DEBUG
        printfDx("[ArtifactManager::GenerateArtifactChoices] artifactPool が空\n");
#endif
        return choices;
    }

    std::mt19937 rng((unsigned int)time(nullptr));

    //プールをシャッフル
    std::vector<std::shared_ptr<ArtifactBase>> shuffled = artifacrPool;
    std::shuffle(shuffled.begin(), shuffled.end(), rng);

    for (int i = 0l; i < 3 && i < (int)shuffled.size(); ++i) {
        choices.push_back(shuffled[i]);
    }


    return choices;
}

/// <summary>
/// スキル適応
/// </summary>
/// <param name="_player"></param>
/// <param name="_artifact"></param>
void ArtifactManager::ApplySelectedArtifact(Player* _player, std::shared_ptr<ArtifactBase> _artifact)
{
    if (!_artifact)
    {
#if _DEBUG
        printfDx("[ArtifactManager::ApplySelectedArtifact] _artifact が null\n");
#endif
        return;
    }

    // ====== アーティファクト効果の適用 ======
#if _DEBUG
    printfDx("[ArtifactManager::ApplySelectedArtifact] 適用: %s\n", _artifact->GetName().c_str());
#endif

    _artifact->Apply(_player);  // ArtifactBaseにApplyEffect(Player*)がある前提
    activeArtifact.push_back(_artifact);

    // ===== 獲得済みリストに追加 =====
    obtainedArtifacts.push_back(_artifact);

    // ====== プールから削除 ======
    auto it = std::find_if(artifacrPool.begin(), artifacrPool.end(),
        [&](const std::shared_ptr<ArtifactBase>& a)
        {
            // 同じポインタを比較（内容で比較したいならIDなどを使う）
            return a == _artifact;
        });

    if (it != artifacrPool.end())
    {
#if _DEBUG
        printfDx("[ArtifactManager::ApplySelectedArtifact] プールから削除: %s\n", (*it)->GetName().c_str());
#endif
        artifacrPool.erase(it);
    }
    else
    {
#if _DEBUG
        printfDx("[ArtifactManager::ApplySelectedArtifact] プールに存在しないアーティファクト\n");
#endif
    }
}

/// <summary>
/// ゲームスタート時にランダムで一つ選出
/// </summary>
/// <returns></returns>
std::vector<std::shared_ptr<ArtifactBase>> ArtifactManager::ApplyArtifact()
{

    std::vector<std::shared_ptr<ArtifactBase>> choices;
    if (artifacrPool.empty())
    {
#if _DEBUG 
        printfDx("[ArtifactManager::ApplyArtifact] artifactPool が空です\n");
#endif
        return choices;
    }
    // ランダムエンジン初期化
    std::mt19937 rng(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<int> dist(0, static_cast<int>(artifacrPool.size()) - 1);

    // ランダムで1つ選ぶ
    int randomIndex = dist(rng);
    auto selectedArtifact = artifacrPool[randomIndex];

#if _DEBUG
    printfDx("[ArtifactManager::ApplyArtifact] ランダム選出: %s\n", selectedArtifact->GetName().c_str());
#endif

    // 適用処理（ArtifactBase の Apply 等があるならここで呼ぶ）
    if (selectedArtifact)
    {
        choices.push_back(selectedArtifact);
    }

    return choices;
    // 取得済みとして管理したい場合はプールから削除
    artifacrPool.erase(artifacrPool.begin() + randomIndex);
}

void ArtifactManager::Update(Player* player)
{
    for (auto& artifact : activeArtifact)
    {
        artifact->Update(player);
    }
}

/// <summary>
/// 削除
/// </summary>
void ArtifactManager::ClearArtifact(Player* player)
{
    for (auto& artifact : activeArtifact) {
        artifact->Remove(player);

    }
    activeArtifact.clear();


}



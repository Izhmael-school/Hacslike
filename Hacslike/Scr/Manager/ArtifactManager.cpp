#include "ArtifactManager.h"
#include <random>
#include <DxLib.h>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "AudioManager.h"
#include "../GameObject/Character/Player/Player.h"
#include "../Save/SaveIO.h" 
#include "../GameSystem/GameSystem.h"


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


void ArtifactManager::RegisterBossDeath(BossBase* boss)
{
    if (!boss) return;

    bossDeathQueue.push(boss); // キューにボスを登録
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

void ArtifactManager::ProcessBossDeath(Player* player)
{
    while (!bossDeathQueue.empty()) {
        BossBase* boss = bossDeathQueue.front();
        bossDeathQueue.pop(); // キューから削除

        // アーティファクト選択処理
        std::vector<std::shared_ptr<ArtifactBase>> artifactChoices = GenerateArtifactChoices();
        if (!artifactChoices.empty()) {
            ArtifactSelectUI artifactUI;
            artifactUI.StartSelection();
            while (artifactUI.IsActive()) {
                int selectedIndex = artifactUI.UpdateSelection(artifactChoices);
                if (selectedIndex >= 0) {
                    ApplySelectedArtifact(player, artifactChoices[selectedIndex]);
                    break; // 選択完了
                }
            }
        }
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
    // ボス死亡イベントの処理
    if (!bossDeathQueue.empty()) {
        ProcessBossDeath(player);
    }
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

    // 取得済みリストをクリア（全て忘れる）
    obtainedArtifacts.clear();

    // アーティファクトプールを初期状態に戻す
    artifacrPool = MakeInitialPool();


}

void ArtifactManager::SaveTo(BinaryWriter& w) const
{
    uint32_t count = static_cast<uint32_t>(obtainedArtifacts.size());
    w.WritePOD(count);
    for (const auto& art : obtainedArtifacts)
    {
        int32_t aid = art->GetID();
        bool active = (std::find(activeArtifact.begin(), activeArtifact.end(), art) != activeArtifact.end());
        w.WritePOD(aid);
        w.WritePOD(active);
        art->Save(w);
    }
}

void ArtifactManager::LoadFrom(BinaryReader& r, uint32_t version)
{
    // Clear current lists
    activeArtifact.clear();
    obtainedArtifacts.clear();
    artifacrPool.clear();

    Player* player = Player::GetInstance(); // assumes Player singleton accessor exists

    uint32_t count = 0;
    r.ReadPOD(count);
    for (uint32_t i = 0; i < count; ++i)
    {
        int32_t aid = 0;
        bool active = false;
        r.ReadPOD(aid);
        r.ReadPOD(active);

        auto art = CreateArtifactByID(aid);
        if (!art)
        {
            // Unknown id: skip -- but reading of artifact-specific data must still be consistent.
#if _DEBUG
            printfDx("[ArtifactManager::LoadFrom] Unknown artifact id: %d\n", aid);
#endif
            continue;
        }
        art->Load(r, version);

        obtainedArtifacts.push_back(art);
        if (active)
        {
            activeArtifact.push_back(art);
            // Reattach to player for event callbacks without reapplying permanent stat changes
            art->Restore(player);
        }
    }

    // Reconstruct artifacrPool as the initial set minus obtained ones (by id)
    auto proto = MakeInitialPool();
    // remove obtained IDs from proto (one-by-one)
    for (const auto& got : obtainedArtifacts)
    {
        int idgot = got->GetID();
        auto it = std::find_if(proto.begin(), proto.end(), [&](const std::shared_ptr<ArtifactBase>& p) {
            return p->GetID() == idgot;
            });
        if (it != proto.end())
            proto.erase(it);
    }
    artifacrPool = std::move(proto);
}

std::shared_ptr<ArtifactBase> ArtifactManager::CreateArtifactByID(int id)
{
    switch (id)
    {
    case 1: return std::make_shared<conditional_attack_power_raise_HP>();
    case 2: return std::make_shared<Hp_Max_Raise>();
    case 3: return std::make_shared<CoinValue_raise>();
    case 4: return std::make_shared<conditional_defense_power_raise_HP>();
    case 5: return std::make_shared<attactPower_raise_GetCoin>();
    case 6: return std::make_shared<itemDropRateUpwardOnCoinAcquisition>();
    case 7: return std::make_shared<AttackincreasesforSeveralSecondsAfterEvasion>();
    case 8: return std::make_shared<CriticalHitRateIncreasesForSeveralSecondsAfterEvasion>();
    default:
        return nullptr;
    }
}

std::vector<std::shared_ptr<ArtifactBase>> ArtifactManager::MakeInitialPool()
{
    return {
       std::make_shared<conditional_attack_power_raise_HP>(),
       std::make_shared<Hp_Max_Raise>(),
       std::make_shared<CoinValue_raise>(),
       std::make_shared<conditional_defense_power_raise_HP>(),
       std::make_shared<attactPower_raise_GetCoin>(),
       std::make_shared<itemDropRateUpwardOnCoinAcquisition>(),
       std::make_shared<AttackincreasesforSeveralSecondsAfterEvasion>(),
       std::make_shared<CriticalHitRateIncreasesForSeveralSecondsAfterEvasion>(),
    };
}



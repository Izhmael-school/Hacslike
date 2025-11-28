#pragma once
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>

// 暗号化用の簡単なXORキー
//改ざん防止
static const unsigned char KEY[] = { 0x5A, 0xAF, 0x13, 0x80 };
static const int KEY_SIZE = sizeof(KEY);
// 各種データ構造（例）
//プレイヤーのデータを保存する場所
struct PlayerData {
    int Lv;                     //プレイヤーのレベル
    int exp;                    //経験値
    int maxExp;                 //必要経験値
    int hp;                     //体力
    int maxhp;                  //最大体力
    int baseAtk;                //攻撃力
    int defanse;                //防御力
    float criticalHitRate;      //会心率
    float creticalDamage;       //
    float proximityCorrection;  //
    float rangedCorrection;     //
    int coin;                   //


    void Save(char*& p);
    void Load(const char*& p);
};

/// <summary>
/// インベントリのデータを保存する場所
/// </summary>
struct InventoryData {
    int items[100];
    int count;

    void Save(char*& p);
    void Load(const char*& p);
};

/// <summary>
/// スキルのデータを保存する場所
/// </summary>
struct SkillData {
    int skillLevels[50];
    int skillCount;

    void Save(char*& p);
    void Load(const char*& p);
};

/// <summary>
/// アーティファクトのデータを保存する場所
/// </summary>
struct ArtifactData {
    int artifacts[50];
    int artifactCount;

    void Save(char*& p);
    void Load(const char*& p);
};

/// <summary>
/// 敵のデータを保存する場所
/// </summary>
struct EnemyData {
    int enemyHP[50];
    int enemyCount;

    void Save(char*& p);
    void Load(const char*& p);
};

/// <summary>
/// ステージのデータを保存する場所
/// </summary>
struct StageData {
    int roomID[50];
    int roomCount;
    int floor;

    void Save(char*& p);
    void Load(const char*& p);
};


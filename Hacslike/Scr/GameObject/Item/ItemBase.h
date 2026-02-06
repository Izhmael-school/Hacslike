#pragma once
#include <string>
#include "../GameObject.h"
#include"../../Manager/SaveManager.h"

class Player;

enum class ItemType
{
    Invaled = -1,
    Heal,          // 回復
    AttackPotion,  // 攻撃UP
    DefensePotion, // 防御UP
    Grenade        // グレネード
};

enum class HealSize {
    Invaled = -1,
    Small,
    Medium,
    Large
};

class ItemBase : public GameObject
{
protected: //メンバ変数
    std::string id;      // ← ファクトリ登録用ID
    std::string name;      //アイテムの名前
    std::string description;	//説明
    std::string type;      //アイテムのタイプ
    std::string Reality;
    int value;             //アイテムの価値など
    int itenEffectValue;   //アイテムの効果値
    std::string itemIcon;  //アイテムUI用のアイコン
    bool isEffectFinished = false;  // 効果が終わったらtrueに

    int itemImageHandle = -1;

public:
    static ItemBase* item;

public://コンストラクタとデストラクタ
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="_name"></param>
    /// <param name="_type"></param>
    /// <param name="_value"></param>
    ItemBase(VECTOR _pos = VZero, std::string tag = "", const std::string& _id = "", const std::string& _name = "", const std::string& _desc = "", 
        const std::string& _type = "",const std::string& _reality = "", int _value = 0, int _effectValue = 0, const std::string& _icon = "");
    

    /// <summary>
    /// デストラクタ
    /// </summary>
    virtual ~ItemBase();

public://メンバ関数
    virtual void Use() = 0;
    virtual void UnEquip() {}; // 装備解除時に効果を外すため
    virtual void Update() {};
    virtual void SaveTo(BinaryWriter& w) = 0;   // セーブ用関数
    virtual void LoadFrom(BinaryReader& r) = 0;       // ロード用関数
    inline bool IsEffectFinished() const { return isEffectFinished; }
public://ゲッター
    /// <summary>
    /// 名前の取得
    /// </summary>
    /// <returns></returns>
    inline const std::string& GetName() const { return name; }
    /// <summary>
    /// アイテムの種類の取得
    /// </summary>
    /// <returns></returns>
    inline const std::string& GetType() const { return type; }

    inline const std::string& GetReality() const { return Reality; }

    /// <summary>
    /// 価値の取得
    /// </summary>
    /// <returns></returns>
    inline int GetValue() const { return value; }

    /// <summary>
    /// アイテムの効果値
    /// </summary>
    /// <returns></returns>
    virtual inline int GetEffectValue() const { return itenEffectValue; }

    inline void  SetAttackValue(int _v) { itenEffectValue = _v; }

    /// <summary>
    /// 説明の取得
    /// </summary>
    /// <returns></returns>
    inline const std::string& GetDescription() const { return description; }

    /// <summary>
    /// アイコンの取得
    /// </summary>
    /// <returns></returns>
    inline const std::string& GetItemIcon() const { return itemIcon; }

    inline const std::string& GetID() const { return id; }

    inline const int GetItemImageHandle() const { return itemImageHandle; }

    virtual ItemType GetItemType() const = 0;  // 派生クラスで返す
    virtual HealSize GetHealType() const =0;
};


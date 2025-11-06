#pragma once
#include <string>
#include "../GameObject.h"

class Player;

class ItemBase : public GameObject
{
protected: //メンバ変数
    std::string name;      //アイテムの名前
    std::string  description;	//説明
    std::string type;      //アイテムのタイプ
    int value;             //アイテムの価値など
    int itenEffectValue;   //アイテムの効果値
    std::string itemIcon;  //アイテムUI用のアイコン

public:
    static ItemBase* item;

public://コンストラクタとデストラクタ
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="_name"></param>
    /// <param name="_type"></param>
    /// <param name="_value"></param>
    ItemBase(VECTOR _pos = VZero, std::string tag = "", const std::string& _name = "", const std::string& _desc = "", const std::string& _type = "", int _value = 0, int _effectValue = 0, const std::string& _icon = "");
    

    /// <summary>
    /// デストラクタ
    /// </summary>
    virtual ~ItemBase();

public://メンバ関数
    virtual void Use() = 0;
    virtual void UnEquip() {}; // 装備解除時に効果を外すため
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
    /// <summary>
    /// 価値の取得
    /// </summary>
    /// <returns></returns>
    inline int GetValue() const { return value; }

    /// <summary>
    /// アイテムの効果値
    /// </summary>
    /// <returns></returns>
    inline int GetEffectValue() const { return itenEffectValue; }

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
};


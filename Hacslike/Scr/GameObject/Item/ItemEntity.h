#pragma once
#include <DxLib.h>
#include <memory>
#include "ItemBase.h"
#include"../../Component/Collider/Collider.h"
#include"../GameObject.h"
#include"../../Manager/CollisionManager.h"
class ItemEntity : public ItemBase
{
public: //メンバ変数
    std::unique_ptr<ItemBase> item;   // アイテム情報本体
    VECTOR position;                  // 生成位置
    float radius;                     //当たり判定の半径
    Collider* pCollider;              //当たり判定のポインタ
    bool isCollected;                 // 取得したかの判定


public://コンストラクタとデストラクタ
    /// <summary>
    /// コンストラクタ
    /// </summary>
    ItemEntity(std::unique_ptr<ItemBase> _item, const VECTOR& _pos, float _radius);

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~ItemEntity();

public://メンバ関数
    void Start() override;
    void Update();
    void Render() override;

    /// <summary>
    /// アイテムを使用した時の効果
    /// </summary>
    void Use()override;

    // ??これを追加（インベントリに渡すため）
    std::unique_ptr<ItemBase> TakeItem();

public://ゲッターとセッター

    /// <summary>
    /// アイテムの取得
    /// </summary>
    /// <returns></returns>
    inline ItemBase* GetItem() { return item.get(); }
    /// <summary>
    /// ポジションの取得
    /// </summary>
    /// <returns></returns>
    inline const VECTOR& GetPosition() const { return position; }
    /// <summary>
    /// 取得したかどうか
    /// </summary>
    /// <returns></returns>
    inline bool IsCollected() const { return isCollected; }


    /// <summary>
    /// 名前の取得
    /// </summary>
    /// <returns></returns>
    inline const std::string& GetName() const { return name; }


};


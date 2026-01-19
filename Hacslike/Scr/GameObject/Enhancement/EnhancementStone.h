#pragma once
#include "../GameObject.h"
#include "../../Enhancement/StatusEnhancement.h"

class CapsuleHitBox;

class EnhancementStone : public GameObject {
private:
    int modelHandle;
    Collider* pCollider;
    StatusEnhancement* pMenu; // 強化メニューのインスタンス
    bool canEnhance;          // プレイヤーが範囲内にいるか
    bool isMenuOpen;          // メニューが開いているか

public:
    EnhancementStone(VECTOR _pos);
    virtual ~EnhancementStone();

    void Start() override;
    void Update() override;
    void Render() override;

    void OpenEnhanceMenu();
    void CloseEnhanceMenu();

    void OnTriggerStay(Collider* _pCol) override;

    void OnTriggerExit(Collider* _pCol) override;
};
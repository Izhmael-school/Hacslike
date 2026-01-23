#pragma once
#include "../GameObject.h"
#include "../../Enhancement/StatusEnhancement.h"
#include "../../Component/Collider/Collider.h"

class CapsuleHitBox;

class EnhancementStone : public GameObject {
public:
    static EnhancementStone* pInstance;

private:
    int modelHandle;
    Collider* pCollider;
    StatusEnhancement* pMenu; // 強化メニューのインスタンス
    bool canEnhance;          // プレイヤーが範囲内にいるか
    bool isMenuOpen;          // メニューが開いているか

private://静的メンバ関数
	/// <summary>
	/// CreateInstance
	/// 自信のインスタンスを生成する
	/// </summary>
	static void CreateInstance();

public://静的メンバ関数
	/// <summary>
	/// GetInstance
	/// 自身のインスタンスを取得する唯一の手段
	/// return InputManager*
	/// </summary>
	/// <returns></returns>DINPUT_JOYSTATE
	static EnhancementStone* GetInstance();

	/// <summary>
	/// DestroyInstance
	/// 自身のインスタンスを破棄する唯一の手段
	/// </summary>
	static void DestroyInstance();
public:
    EnhancementStone();
    virtual ~EnhancementStone();

    void Start() override;
    void Update() override;
    void Render() override;

    void OpenEnhanceMenu();
    void CloseEnhanceMenu();

	inline bool GetIsOpenMenu() { return isMenuOpen; }

    void OnTriggerStay(Collider* _pCol) override;

    void OnTriggerExit(Collider* _pCol) override;
};
#pragma once
#include <vector>
#include "../GameObject/GameObject.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/EffectManager.h"
#include "../GameObject/Weapon/Weapon.h"
/*
 * @brief シーンクラス
 * @tips  全てのシーンの基底クラス
 */
class BaseScene {
protected:	// メンバ変数
	std::vector<GameObject*> pGameObjectArray;	// 一元管理配列

public:		// トラクタ
	BaseScene() = default;
	virtual ~BaseScene() {
		// 動的確保したインスタンスの開放
		for (auto pObj : pGameObjectArray) {
			if (pObj == nullptr)
				return;
		}
	}

public:		// メンバ関数
	/*
	 * @function	Start
	 * @brief		初期化処理
	 */
	virtual void Start() = 0;

	/*
	 * @function	Update
	 * @brief		更新処理
	 */
	virtual void Update() = 0;

	/*
	 * @function	Render
	 * @brief		描画処理
	 */
	virtual void Render() = 0;

};

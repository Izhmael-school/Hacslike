#pragma once
#include "../../Component/Collider/Collider.h"
#include <vector>
#include <DxLib.h>

class CapsuleHitBox {
private:
    GameObject* owner;
    VECTOR startPos;
    VECTOR endPos;
    float radius;

    CapsuleCollider* pCollider;
    float timer;
    float lifeTime;

public:
    CapsuleHitBox(GameObject* _owner, const VECTOR& p1, const VECTOR& p2, float _radius, float _lifeTime);
    ~CapsuleHitBox();

    void Update();
    void Render();
    bool IsDead() const;

    void CreateCollider();

    CapsuleCollider* GetCollider() const { return pCollider; }

public:		//	オーバーライドした衝突検知
	/*
	 *	@function	OnTriggerEnter
	 *	@brief		当たった瞬間
	 *	@param[in]	Collider* _pCol
	 */
	void OnTriggerEnter(Collider* _pCol);

	/*
	 *	@function	OnTriggerEnter
	 *	@brief		当たっている間
	 *	@param[in]	Collider* _pCol
	 */
	void OnTriggerStay(Collider* _pCol);

	/*
	 *	@function	OnTriggerEnter
	 *	@brief		離れた瞬間
	 *	@param[in]	Collider* _pCol
	 */
	void OnTriggerExit(Collider* _pCol);
};
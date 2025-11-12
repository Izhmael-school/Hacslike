#pragma once
#include "Collider.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/Character/Character.h"
#include <vector>

#include <DxLib.h>

class SphereHitBox : public GameObject{
private:
	GameObject* owner;
	Character* character;

	VECTOR offset;
	float radius;

	SphereCollider* pCollider;
	float timer;
	float lifeTime;

public:
	SphereHitBox(GameObject* _ownwr, VECTOR _offset,float _radius, float _lifeTime);
	~SphereHitBox();

	void Start() override;
	void Update()override;
	void Render()override;
	bool IsDead() const;

	void CreateCollider();

	SphereCollider* GetCollider() const { return pCollider; }

public:		//	オーバーライドした衝突検知
	/*
	 *	@function	OnTriggerEnter
	 *	@brief		当たった瞬間
	 *	@param[in]	Collider* _pCol
	 */
	void OnTriggerEnter(Collider* _pCol) override;

	/*
	 *	@function	OnTriggerEnter
	 *	@brief		当たっている間
	 *	@param[in]	Collider* _pCol
	 */
	void OnTriggerStay(Collider* _pCol) override;

	/*
	 *	@function	OnTriggerEnter
	 *	@brief		離れた瞬間
	 *	@param[in]	Collider* _pCol
	 */
	void OnTriggerExit(Collider* _pCol) override;

};


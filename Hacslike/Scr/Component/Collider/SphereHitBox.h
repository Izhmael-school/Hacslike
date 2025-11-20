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
	VECTOR position;
	VECTOR velocity;
	float radius;
	float lifeTime;
	float timer;
	bool active;

	SphereCollider* pCollider;

public:
	SphereHitBox(GameObject* _ownwr, VECTOR _offset,float _radius, float _lifeTime);
	SphereHitBox();
	~SphereHitBox();

	void Start() override;
	void Update()override;
	void Render()override;
	bool IsDead() const;

	void CreateCollider();

	SphereCollider* GetCollider() const { return pCollider; }

	void Move(const VECTOR& vel);

	void SetVelocity(const VECTOR& v) { velocity = v; }

	void Reset(GameObject* _owner, const VECTOR& startPos, const VECTOR& _velocity,
		float _radius, float _life);

	inline bool GetActive()const { return active; }

	inline void SetActive(bool _active) { active = _active; }

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


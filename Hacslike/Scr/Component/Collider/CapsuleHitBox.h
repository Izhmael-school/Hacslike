#pragma once
#include "../../Component/Collider/Collider.h"
#include <vector>
#include <DxLib.h>

class CapsuleHitBox :public GameObject{
private:
    GameObject* owner;
    VECTOR startPos;
    VECTOR endPos;
    float radius;

	Character* character;

    CapsuleCollider* pCollider;
    float timer;
    float lifeTime;

	VECTOR velocity;

	bool active;

public:
    CapsuleHitBox(GameObject* _owner, const VECTOR& p1, const VECTOR& p2, float _radius, float _lifeTime);
    CapsuleHitBox();
    ~CapsuleHitBox();

	void Start() override;
    void Update() override;
    void Render() override;
    bool IsDead() const;

    CapsuleCollider* GetCollider() const { return pCollider; }

	void CreateCollider();

	void Move(const VECTOR& vel);

	void Reset(GameObject* _owner, const VECTOR& p1, const VECTOR& p2,
		const VECTOR& _velocity, float _radius, float _life);

	inline bool GetActive()const { return active; }

	inline void SetActive(bool _active) { active = _active; }

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
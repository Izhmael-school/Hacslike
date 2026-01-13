#pragma once
#include "Collider.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/Character/Character.h"
#include <vector>

#include <DxLib.h>

class SphereHitBox : public GameObject{
private:

	Character* owner;		//	所有者
	Character* character;	//	キャラクター
	VECTOR offset;			//	基準値からの距離
	VECTOR position;		//	位置
	VECTOR velocity;		//	速度
	float radius;			//	半径
	float lifeTime;			//	出ている時間
	float timer;			//	時間
	bool active;			//	アクティブ
	int chain;				//	チェイン

	SphereCollider* pCollider;	//	コライダー

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_ownwr"></param>
	/// <param name="_offset"></param>
	/// <param name="_radius"></param>
	/// <param name="_lifeTime"></param>
	SphereHitBox(Character* _ownwr, VECTOR _offset,float _radius, float _lifeTime);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SphereHitBox();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SphereHitBox();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Start() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Render()override;

	/// <summary>
	/// 消す処理
	/// </summary>
	/// <returns></returns>
	bool IsDead() const;

	/// <summary>
	/// コライダーの作成
	/// </summary>
	void CreateCollider();

	/// <summary>
	/// コライダーの取得
	/// </summary>
	/// <returns></returns>
	SphereCollider* GetCollider() const { return pCollider; }

	/// <summary>
	/// コライダーの移動
	/// </summary>
	/// <param name="vel"></param>
	void Move(const VECTOR& vel);

	/// <summary>
	/// 速度の設定
	/// </summary>
	/// <param name="v"></param>
	void SetVelocity(const VECTOR& v) { velocity = v; }

	/// <summary>
	/// リセット
	/// </summary>
	/// <param name="_owner"></param>
	/// <param name="startPos"></param>
	/// <param name="_velocity"></param>
	/// <param name="_radius"></param>
	/// <param name="_life"></param>
	void Reset(Character* _owner, const VECTOR& startPos, const VECTOR& _velocity,
		float _radius, float _life);

	/// <summary>
	/// 弾用のリセット
	/// </summary>
	/// <param name="_owner"></param>
	/// <param name="startPos"></param>
	/// <param name="_velocity"></param>
	/// <param name="_radius"></param>
	/// <param name="_life"></param>
	/// <param name="_chain"></param>
	void BulletReset(Character* _owner, const VECTOR& startPos, const VECTOR& _velocity, float _radius, float _life, int _chain);

	/// <summary>
	/// アクティブかどうか
	/// </summary>
	/// <returns></returns>
	inline bool GetActive()const { return active; }
	
	/// <summary>
	/// アクティブの設定
	/// </summary>
	/// <param name="_active"></param>
	inline void SetActive(bool _active) { active = _active; }

	/// <summary>
	/// 所有者の設定
	/// </summary>
	/// <param name="_owner"></param>
	inline void SetOwner(Character* _owner) { owner = _owner; }

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


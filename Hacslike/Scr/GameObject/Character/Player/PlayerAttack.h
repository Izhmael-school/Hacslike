#pragma once
#include "../../../Component/Collider/SphereHitBox.h"
#include "../../../Component/Collider/CapsuleHitBox.h"
#include "PlayerMovement.h"
#include "../../../Manager/AudioManager.h"
#include "../../Bullet/BulletPool.h"
#include "../../HitPool/CapsuleHitPool.h"

class Player;
class Weapon;
class InputManager;

class PlayerAttack {
private:
	Player* pPlayer;		//	プレイヤー
	Weapon* pWeapon;		//	武器
	InputManager* input;	//	入力

	bool isAttacking;	//	攻撃してるか
	int attackIndex;	//	攻撃の番号
	float attackTimer;	//	攻撃の当たり判定の表示時間
	bool canNextAttack;	//	次の攻撃ができるかどうか
	bool attackButtonPressed;	//	攻撃ボタンが押されているかどうか

	std::vector<CapsuleHitBox*> CapsuleHitboxes;	//	当たり判定(カプセル)
	std::vector<SphereHitBox*> SphereHitboxes;		//	当たり判定(スフィア)

	SphereHitBox* spherebox;

	PlayerMovement* playerMovement;	//	プレイヤーの移動処理

	bool isDashAttack;

	bool checkDashAttack; //ダッシュ攻撃が可能かどうか

	bool isCharging;			//	ためてるかどうか
	float chargeTime;			//	ためてる時間
	const float maxChargeTime ;	//	最大溜め

	AudioManager* audio;		//	音

	BulletPool* pSpherePool;	//	Sphereのプール

	CapsuleHitPool* pCapsulePool;	//	Capsuleのプール	

	float addRadius;			//	半径を足す

	int currentChainCount;		//　現在のチェイン回数
	int maxChainCount;			//	チェインの最大回数

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_player"></param>
	/// <param name="_weapon"></param>
	/// <param name="_playerMovement"></param>
	PlayerAttack(Player* _player, Weapon* _weapon, PlayerMovement* _playerMovement);

	/// <summary>
	/// 初期化
	/// </summary>
	void Start();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	void Render();

	/// <summary>
	/// 攻撃入力・HitBox更新
	/// </summary>
	void AttackInput();

	///<summary>
	///攻撃時の当たり判定発生処理
	///</summary>
	///<param name="length"></param>
	///<param name="radius"></param>
	void CreateAttackHitbox(float _length, float _radius);

	/// <summary>
	/// アタック中かどうか
	/// </summary>
	inline bool IsAttacking() const { return isAttacking; }

	/// <summary>
	/// ダッシュアタックしてるかどうか
	/// </summary>
	inline bool IsDashAttacking() const { return isDashAttack; }

	inline void SetIsCheckDashAttack(bool _b) { checkDashAttack = _b; }

	void CreateRangedHitBox();

	void CreateHitBox(VECTOR _pos, float _radius);

	/// <summary>
	/// 武器の設定
	/// </summary>
	void SetWeapon(Weapon* _weapon) {
		pWeapon = _weapon;
	}
};
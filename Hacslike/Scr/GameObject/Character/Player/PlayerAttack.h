#pragma once
#include "../../../Component/Collider/SphereHitBox.h"
#include "../../../Component/Collider/CapsuleHitBox.h"
#include "PlayerMovement.h"
#include "../../../Manager/AudioManager.h"
#include "../../Bullet/BulletPool.h"
#include "../../HitPool/CapsuleHitPool.h"

//	前方宣言
class Player;
class Weapon;
class InputManager;

/// <summary>
/// プレイヤーの攻撃処理
/// </summary>
class PlayerAttack {
private:	//	変数
	//	クラス関連
	Player* pPlayer;				//	プレイヤー
	Weapon* pWeapon;				//	武器
	InputManager* input;			//	入力
	PlayerMovement* playerMovement;	//	プレイヤーの移動処理
	AudioManager* audio;			//	音
	BulletPool* pSpherePool;		//	Sphereのプール
	CapsuleHitPool* pCapsulePool;	//	Capsuleのプール	


	//	通常攻撃関連
	int attackIndex;			//	攻撃の番号
	float attackTimer;			//	攻撃の当たり判定の表示時間
	bool isAttacking;			//	攻撃してるか
	bool canNextAttack;			//	次の攻撃ができるかどうか
	bool attackButtonPressed;	//	攻撃ボタンが押されているかどうか

	bool hasGeneratedHitbox;

	std::string animName;

	//	ダッシュ攻撃関連
	bool isDashAttack;			//	ダッシュ攻撃してるかどうか
	bool checkDashAttack;		//	ダッシュ攻撃が可能かどうか

	//	ため攻撃関連
	bool isCharging;			//	ためてるかどうか
	float chargeTime;			//	ためてる時間
	const float maxChargeTime ;	//	最大溜め

	//	当たり判定関連
	std::vector<CapsuleHitBox*> CapsuleHitboxes;	//	当たり判定(カプセル)
	std::vector<SphereHitBox*> SphereHitboxes;		//	当たり判定(スフィア)
	
	//	遠距離武器関連	
	int currentChainCount;		//　現在のチェイン回数
	int maxChainCount;			//	チェインの最大回数

	float addRadius;			//	半径を足す(デバック用)

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

	/// <summary>
	/// 描画処理
	/// </summary>
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
	/// グレネード用の当たり判定処理
	/// </summary>
	/// <param name="_pos"></param>
	/// <param name="_radius"></param>
	void CreateHitBox(VECTOR _pos, float _radius);

	/// <summary>
	/// アタック中かどうか
	/// </summary>
	inline bool IsAttacking() const { return isAttacking; }

	/// <summary>
	/// ダッシュアタックしてるかどうか
	/// </summary>
	inline bool IsDashAttacking() const { return isDashAttack; }

	/// <summary>
	/// ダッシュアタック可能かの設定
	/// </summary>
	/// <param name="_b"></param>
	inline void SetIsCheckDashAttack(bool _b) { checkDashAttack = _b; }

	/// <summary>
	/// 武器の設定
	/// </summary>
	void SetWeapon(Weapon* _weapon) {
		pWeapon = _weapon;
	}

	/// <summary>
	/// 攻撃時にやること
	/// </summary>
	void AttackReset();

	/// <summary>
	/// 当たり判定生成時にやること
	/// </summary>
	void HitBoxReset();
};
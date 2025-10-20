#pragma once

#include <memory>
#include <vector>

#include "../Character.h"
#include "../../Weapon/Weapon.h"
#include "../../Slash/Slash.h"
#include "../../../Component/Collider/SphereHitBox.h"
#include "../../../Component/Collider/CapsuleHitBox.h"
#include "../../../Manager/InputManager.h" 

/*
 *	@brief	プレイヤークラス
 *	@tips	Characterクラスの派生クラス
 */

class Player : public Character {
#pragma region メンバ変数
private:
	bool isAttacking;	//	攻撃中

	// 所有リソースは unique_ptr に
	std::unique_ptr<Weapon> pWeapon;

	VECTOR inputVec;

	InputManager* input; // シングルトン参照（所有しない）

	XINPUT_STATE XY;

	// 所有コンテナ（自動で破棄される）
	std::vector<std::unique_ptr<Slash>> slashes;

	int attackIndex;       // 現在の攻撃段階（1〜3）
	float attackTimer;     // 経過時間
	bool canNextAttack;    // 次の入力受付可

	std::vector<std::unique_ptr<CapsuleHitBox>> CapsuleHitboxes;
	std::vector<std::unique_ptr<SphereHitBox>> SphereHitboxes;

	// 残像履歴（必要なら Definition.h 等に移設）
	static const int AFTIMAGENUM = 20;
	VECTOR afterImagePos[AFTIMAGENUM];
	float afterImageRotY[AFTIMAGENUM];

	bool isBlinking;
	float blinkTimer;

	float attackInputCooldown;      // 攻撃クールダウン
	bool attackButtonPressed;
	bool evasionButtonPressed;

	float evasionCooldown;        // クールダウンタイマー
	const float EVASION_COOLDOWN_TIME = 2.0f; // 2秒クールダウン

	const float ATTACK_INPUT_COOLDOWN_TIME = 0.2f; // 0.2秒クールダウン

	float evasionSpeed;

	int currentWeaponId;            // 初期武器ID
	bool changeWeaponButtonPressed; // ボタン押下フラグ
	int maxWeaponId;                // 武器の最大ID（JSONの数に合わせる）

#pragma endregion

#pragma region コンストラクタとデストラクタ
public:
	/*
	 *	@brief		コンストラクタ
	 *	@param[in]	VECTOR _pos		初期化する座標
	 */
	Player(VECTOR _pos = VZero);

	/*
	 *	@breif		デストラクタ
	 *
	 *	注: unique_ptr を使っているので多くの delete は不要です。
	 *	　　ただし CollisionManager に登録した HitBox/Collider は破棄前に UnRegister を呼ぶ必要があります。
	 */
	~Player();
#pragma endregion

#pragma region	オーバーライドしたメンバ関数
public:
	/*
	 *	@function	Start
	 *	@breif		初期化処理
	 */
	virtual void Start() override;

	/*
	 *	@function	Update
	 *	@breif		更新処理
	 */
	virtual void Update() override;

	/*
	 *	@function	Render
	 *	@breif		描画処理
	 */
	virtual void Render() override;

public:		//	オーバーライドした衝突検知
	/*
	 *	@function	OnTriggerEnter
	 *	@brief		当たった瞬間
	 *	@param[in]	Collider* _pCol
	 */
	void OnTriggerEnter(Collider* _pCol) override;

	/*
	 *	@function	OnTriggerStay
	 *	@brief		当たっている間
	 *	@param[in]	Collider* _pCol
	 */
	void OnTriggerStay(Collider* _pCol) override;

	/*
	 *	@function	OnTriggerExit
	 *	@brief		離れた瞬間
	 *	@param[in]	Collider* _pCol
	 */
	void OnTriggerExit(Collider* _pCol) override;
#pragma endregion


#pragma region メンバ関数
public:
	void UpdateMovement();
	void MoveInput();
	void EvasionInput();
	void AttackInput();
	void UpdateSlash();

	void CreateAttackHitbox(float length, float radius); // radius に修正
	void Evasion();
	void UpdateBlink();
	void Dash();
	void ChangeWeapon(int weaponId);
	void WeaponInput();
#pragma endregion


#pragma region Getter/Setter
public:
	/*
	 *	@function	GetWeapon
	 *	@brief		武器の取得（非所有ポインタ）
	 *	@return		Weapon*
	 */
	inline Weapon* GetWeapon() const { return pWeapon.get(); }

	/*
	 *	@function	SetWeapon
	 *	@brief		武器の変更（所有権を移す）
	 *	@param[in]	Weapon* _pWeapon
	 *
	 *	使い方:
	 *	 SetWeapon(new Weapon(...)); // 生ポインタ渡しで所有権を移す
	 */
	inline void SetWeapon(Weapon* _pWeapon) { pWeapon.reset(_pWeapon); }

	/*
	 *	@function	SetWeaponUnique
	 *	@brief		既に unique_ptr を持っている場合に所有権を移すオーバーロード
	 */
	inline void SetWeaponUnique(std::unique_ptr<Weapon> wp) { pWeapon = std::move(wp); }

#pragma endregion
};
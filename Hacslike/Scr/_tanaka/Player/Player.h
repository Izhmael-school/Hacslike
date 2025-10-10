#pragma once
#include "../Hacslike/Scr/_sekino/GameObject/Character/Character.h"
#include "../Hacslike/Scr/_sekino/GameObject/Weapon/Weapon.h"
#include "../Hacslike/Scr/_tanaka/Slash/Slash.h"
#include "../Collider/CapsuleHitBox.h"
#include "../Collider/SphereHitBox.h"

/*
 *	@brief	プレイヤークラス
 *	@tips	Characterクラスの派生クラス
 */
class Player : public Character {
#pragma region メンバ変数
private:	//	メンバ変数
	bool isAttacking;	//	攻撃中

	Weapon* pWeapon;

	XINPUT_STATE XY;

	std::vector<Slash*> slashes;

	int attackIndex;       // 現在の攻撃段階（1〜3）
	float attackTimer;     // 経過時間
	bool canNextAttack;    // 次の入力受付可

	std::vector<CapsuleHitBox*> CapsuleHitboxes;

	std::vector<SphereHitBox*> SphereHitboxes;

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


#pragma endregion

#pragma region コンストラクタとデストラクタ
public:		//	コンストラクタとデストラクタ

	/*
	 *	@brief		コンストラクタ
	 *	@param[in]	VECTOR _pos		初期化する座標
	 */
	Player(VECTOR _pos = VZero);

	/*
	 *	@breif		デストラクタ
	 */
	~Player();
#pragma endregion

#pragma region	オーバーライドしたメンバ関数
public:		//	オーバーライドしたメンバ関数
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


public:		//	メンバ関数

	void AttackEnd();

	void CreateAttackHitbox(float length, float Capsuleadius);

	void Evasion();



public:		//	Getter と Setter
	/*
	 *	@function	GetWeapon
	 *	@brief		武器の取得
	 *	@return		Weapon*
	 */
	inline Weapon* GetWeapon() const { return pWeapon; }

	/*
	 *	@function	SetWeapon
	 *	@brief		武器の変更
	 *	@param[in]	Weapon* _pWeapon
	 */
	inline void SetWeapon(Weapon* _pWeapon) { pWeapon = _pWeapon; }

};



#pragma once
#include "../Character.h"
#include "../../Weapon/Weapon.h"
#include "../../Slash/Slash.h"
#include "../../../Component/Collider/SphereHitBox.h"
#include "../../../Component/Collider/CapsuleHitBox.h"
#include"../../Item/Inventory.h"
#include"../../Item/ItemBase.h"
#include"../../../Manager/ItemDropManager.h"

//#include "../../../Manager/InputManager.h"


/*
 *	@brief	プレイヤークラス
 *	@tips	Characterクラスの派生クラス
 */
class Player : public Character {
#pragma region メンバ変数
private:	//	メンバ変数
	bool isAttacking;	//	攻撃中

	Weapon* pWeapon;

	VECTOR inputVec;

	InputManager* input;

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

	int currentWeaponId;            // 初期武器ID
	bool changeWeaponButtonPressed; // ボタン押下フラグ
	int maxWeaponId;                // 武器の最大ID（JSONの数に合わせる）

	Inventory inventory; //アイテムインベントリ
	bool hitItem;		 //アイテムに当たっているかどうか
	bool isItemUI;		 //アイテムのUIを開いているかどうか

	int coinValue;

	int expValue;

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

	/// <summary>
	/// 移動・アニメーション・回転処理
	/// </summary>
	void UpdateMovement();

	/// <summary>
	/// 移動入力
	/// </summary>
	void MoveInput();

	/// <summary>
	/// 回避入力
	/// </summary>
	void EvasionInput();

	/// <summary>
	/// 攻撃入力・HitBox更新
	/// </summary>
	void AttackInput();

	/// <summary>
	/// 斬撃更新
	/// </summary>
	void UpdateSlash();

	//void AttackEnd();

	/// <summary>
	/// 攻撃時の当たり判定発生処理
	/// </summary>
	/// <param name="length"></param>
	/// <param name="radius"></param>
	void CreateAttackHitbox(float length, float radius);

	/// <summary>
	/// 回避
	/// </summary>
	void Evasion();

	/// <summary>
	/// BlinkのUpdate
	/// </summary>
	void UpdateBlink();

	/// <summary>
	/// ダッシュ
	/// </summary>
	void Dash();

	/// <summary>
	/// 武器切り替え
	/// </summary>
	/// <param name="weaponId"></param>
	void ChangeWeapon(int weaponId);

	/// <summary>
	/// 武器切り替え
	/// </summary>
	void WeaponInput();

	/// <summary>
	/// アイテムの取得
	/// </summary>
	void AddItem();

	/// <summary>
	/// アイテムのインベントリを開く
	/// </summary>
	void OpenInventory();
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

	/// <summary>
	/// インベントリの取得
	/// </summary>
	/// <returns></returns>
	inline Inventory* GetInventory() { return &inventory; }

	/// <summary>
	/// コインの取得
	/// </summary>
	/// <returns></returns>
	inline int GetCoinValue() const { return coinValue; }

	/// <summary>
	/// コインの変更
	/// </summary>
	/// <param name="_coin"></param>
	inline void SetCoinValue(int _coin) { coinValue = _coin; }

	/// <summary>
	/// 経験値に取得
	/// </summary>
	/// <returns></returns>
	inline int GetExpValue() const { return expValue; }

	/// <summary>
	/// 経験値の変更
	/// </summary>
	/// <param name="_exp"></param>
	inline void SetExpValue(int _exp) { expValue = _exp; }
};
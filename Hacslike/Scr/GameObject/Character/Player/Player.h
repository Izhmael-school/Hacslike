#pragma once
#include "../Character.h"
#include "../../Weapon/Weapon.h"
#include "../../Slash/Slash.h"
#include"../../Item/Inventory.h"
#include"../../Item/ItemBase.h"
#include"../../../Manager/ItemDropManager.h" 
#include "../Hacslike/Scr/Manager/WeaponManager.h"

#include "PlayerAttack.h"
#include "PlayerMovement.h"



/*
 *	@brief	プレイヤークラス
 *	@tips	Characterクラスの派生クラス
 */
class Player : public Character {
private:
	static Player* instance; // シングルトン用
#pragma region メンバ変数
private:	//	メンバ変数
	//bool isAttacking;	//	攻撃中

	Weapon* pWeapon;

	/*VECTOR inputVec;*/

	InputManager* input;

	//std::vector<Slash*> slashes;

	int currentWeaponId;            // 初期武器ID
	bool changeWeaponButtonPressed; // ボタン押下フラグ
	int maxWeaponId;                // 武器の最大ID（JSONの数に合わせる）

	Inventory inventory; //アイテムインベントリ
	bool hitItem;		 //アイテムに当たっているかどうか
	bool isItemUI;		 //アイテムのUIを開いているかどうか

	int coinValue;

	int expValue;

	float criticalHitRate;

	float criticalDamage;

	PlayerAttack* playerAttack;
	PlayerMovement* playerMovement;

	WeaponData* weaponData;

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

	/// <summary>
	/// プレイヤーステータスの描画
	/// </summary>
	void PlayerStatusRender();

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
	/// 会心率の取得
	/// </summary>
	/// <returns></returns>
	inline float GetCriticalHitRate() const { return criticalHitRate; }

	/// <summary>
	/// 会心率の変更
	/// </summary>
	/// <param name="_criticalHitRate"></param>
	inline void SetCriticalHitRate(float _criticalHitRate) { criticalHitRate = _criticalHitRate; }

	/// <summary>
	/// 会心ダメージの取得
	/// </summary>
	/// <returns></returns>
	inline float GetCriticalDamage() const { return criticalDamage; }

	/// <summary>
	/// 会心ダメージの変更
	/// </summary>
	/// <param name="_criticalDamage"></param>
	inline void SetCriticalDamage(float _criticalDamage) { criticalDamage = _criticalDamage; }

	inline PlayerAttack* GetPlayerAttack() const { return playerAttack; }


	VECTOR GetForward() const {
		return VNorm(VGet(-sinf(Deg2Rad(rotation.y)), 0.0f, -cosf(Deg2Rad(rotation.y))));
	}
public:
	// 🔹 シングルトン関連
	static Player* CreateInstance(VECTOR _pos = VZero);
	static Player* GetInstance();
	static void DestroyInstance();

};
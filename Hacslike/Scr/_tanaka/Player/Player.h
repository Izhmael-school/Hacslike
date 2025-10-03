#pragma once
#include "../Hacslike/Scr/_sekino/GameObject/Character/Character.h"
#include "../Hacslike/Scr/_sekino/GameObject/Weapon/Weapon.h"

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



#pragma once
#include "../GameObject.h"
#include "../../Component/Animator.h"
#include "../Card/Hand/Hand.h"
#include "Buff/Buff.h"

class Character : public GameObject {
protected:	// メンバ変数
	int modelHandle;		// モデルハンドル
	Animator* pAnimator;	// アニメーター
	int hp;					// 体力
	int atk;				// 攻撃力
	bool atking;			// 攻撃フラグ
	int def = 0;				// 防御力
	bool blocking;			// 防御フラグ

	Hand* pHand = nullptr;

	Cost* pCost = nullptr;

	Buff* pBuff = new Buff();

	int Lv;
	int Exp;


public:	// コンストラクタとデストラクタ

	Character(VECTOR _pos = VZero, std::string tag = "", int _Lv = 1,int _Exp = 5);

	virtual ~Character();
public:	// オーバーライドしたメンバ関数
	// あえてオーバーライドをせずに、
	// Characterクラスも抽象クラスとして扱う

public:
	int SetAnimation();


public:	// ゲッターとセッター

#pragma region ModelHandle
	/*
	 * @function	GetModelHandle
	 * @brief		モデルハンドルの取得
	 * @return		int
	 */
	inline int GetModelHandle() const { return modelHandle; }

	/*
	 * @function	SetModelHandle
	 * @brief		モデルハンドルの変更
	 * @param[in]	int _v
	 */
	inline void SetModelHandle(int& _v) { modelHandle = _v; }
	inline void SetModelHandleDup(int _v) { modelHandle = _v; }
#pragma endregion

#pragma region Animator
	inline Animator* GetAnimator() const {
		return pAnimator; }
#pragma endregion

#pragma region Hp
	inline void AddHp(int damage) { hp -= damage; }
#pragma endregion

#pragma region Attack
	// 攻撃力の取得
	inline int GetAtk() const { return atk; }
	// 攻撃力の設定
	inline void SetAtk(int _atk) { atk = _atk; }
	// 攻撃フラグの取得
	inline bool isAttack() const { return atking; }
	// 攻撃フラグの設定
	inline void SetAttacking(bool _fact) { atking = _fact; }
#pragma endregion

#pragma region Defence
	// 防御力の取得
	inline int GetDef() const { return def; }
	// 防御力の設定
	inline void SetDef(int _def) { def = _def; }
	// 防御フラグの取得
	inline bool isBlocking() const { return blocking; }
	// 防御フラグの設定
	inline void SetBlocking(bool _fact) { blocking = _fact; }
#pragma endregion

	inline void SetHand(Hand* _pHand) { pHand = _pHand; }
	inline Hand* GetHand() const { if (pHand != nullptr) 
		return pHand; 
	else 
		return nullptr; }

	inline int GetHp()const { return hp; }
	inline void SetHp(int _Hp) { hp = _Hp; }

	inline void SetCost(Cost* _cost) { pCost = _cost; }
	inline Cost* GetCost() const { return pCost; }

	inline void SetpBuff(Buff* _pBuff) { pBuff = _pBuff; }
	inline Buff* GetpBuff() { return pBuff; }
	inline void AddExp(int _Exp) { Exp += _Exp; }
	inline int GetExp() const { return Exp; }
};


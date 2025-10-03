#pragma once
#include "../GameObject.h"
#include "../../Component/Animator.h"
#include "../../../Definition.h"

class Character : public GameObject {
protected:	// メンバ変数
	int modelHandle;		// モデルハンドル
	Animator* pAnimator;	// アニメーター
	int hp;					// 体力
	int atk;				// 攻撃力
	bool atking;			// 攻撃フラグ
	int def = 0;				// 防御力
	bool blocking;			// 防御フラグ

	int Lv;
	int Exp;
public:
	static Character* player;
	

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

	inline Character* GetPlayer() const { 
		return Character::player; }

	inline void SetPlayer(Character* _player) { Character::player = _player; }
};


#pragma once
#include "../GameObject.h"
#include "../Character/Character.h"

class Weapon : public GameObject {
private:	// メンバ変数
	int modelHandle;	// 武器のモデルハンドル
	int attachModelHandle;	// 武器を持たせるモデルハンドル
	int attachFrameIndex;	// 武器を持たせるフレーム番号
	Character* User;


public:
	Weapon(std::string _tag = "");
	~Weapon();

public:
	/*
	 * @function	Start
	 * @brief		初期化処理
	 */
	virtual void Start() override;

	/*
	 * @function	Update
	 * @brief		更新処理
	 */
	virtual void Update() override;

	/*
	 * @function	Render
	 * @brief		描画処理
	 */
	virtual void Render() override;

public:
	void attach(int& _characterModel, int& _weapon, std::string _framename, Character* _User);
	void attachDup(int& _characterModel, int _weaponDup, std::string _framename, Character* _User);

public:// ゲッタ―セッター
	inline int GetWeaponHandle() const { return modelHandle; }

	inline Character* GetUser() const { return User; }

	void OnTriggerEnter(Collider* _pOther) override;

	void OnTriggerStay(Collider* _pOther) override;

	void OnTriggerExit(Collider* _pOther) override;

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
};


#pragma once
#include <DxLib.h>
#include"../../Artifact/ArtifactBase.h"
#include "../../../Manager/AudioManager.h"

//	前方宣言
class Player;
class PlayerAttack;
class InputManager;

/// <summary>
/// プレイヤーの移動処理
/// </summary>
class PlayerMovement {
private:
	static PlayerMovement* instance; // シングルトン用
	
#pragma region メンバ変数
private:
#pragma region クラス関連
	Player* pPlayer;			//	プレイヤー
	InputManager* input;		//	入力
	PlayerAttack* playerAttack;	//	プレイヤーの攻撃
	AudioManager audio;			//	音
	AttackincreasesforSeveralSecondsAfterEvasion* attactArtifact;			//
	CriticalHitRateIncreasesForSeveralSecondsAfterEvasion* CriticalArtifact;//
#pragma endregion

#pragma region 回避処理
	static const int AFTIMAGENUM = 20;	//	残像数
	VECTOR afterImagePos[AFTIMAGENUM];	//	残像の場所
	float afterImageRotY[AFTIMAGENUM];	//	残像の回転
	bool isBlinking;					//	回避中かどうか
	float blinkTimer;					//	回避時間
	bool evasionButtonPressed;			//	回避入力
	float evasionSpeed;					//	回避速度
	float evasionCooldown;				// クールダウンタイマー
	const float EVASION_COOLDOWN_TIME = 2.0f;		// 2秒クールダウン
	const float ATTACK_INPUT_COOLDOWN_TIME = 0.2f;	// 0.2秒クールダウン
#pragma endregion

	VECTOR inputVec;	//	移動量

	bool dashState;		//	ダッシュしてるかどうか
#pragma endregion

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_player"></param>
	PlayerMovement(Player* _player);

#pragma region シングルトン関連
	static PlayerMovement* CreateInstance(Player* _player);
	static PlayerMovement* GetInstance();
	static void DestroyInstance();
#pragma endregion

	/// <summary>
	/// 初期化処理
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

public:
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

public:
	/// <summary>
	/// 回避中かどうか
	/// </summary>
	/// <returns></returns>
	inline bool IsBlinking() const { return isBlinking; }

	/// <summary>
	/// 回避入力されてるかどうか
	/// </summary>
	/// <returns></returns>
	inline bool IsEvasionButtonPressed() const { return evasionButtonPressed; }

	/// <summary>
	/// ダッシュしてるかどうか
	/// </summary>
	/// <returns></returns>
	inline bool IsDashState() const { return dashState; }

	/// <summary>
	/// プレイヤーの取得
	/// </summary>
	/// <returns></returns>
	inline Player* GetPlayer()const {return pPlayer;}

	inline void SetAttactArtifact(AttackincreasesforSeveralSecondsAfterEvasion* artifact) { attactArtifact = artifact; }
	inline void SetCriticalArtifact(CriticalHitRateIncreasesForSeveralSecondsAfterEvasion* artifact) { CriticalArtifact = artifact; }
};
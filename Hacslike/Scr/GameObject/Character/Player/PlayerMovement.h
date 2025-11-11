#pragma once
#include <DxLib.h>
#include"../../Artifact/ArtifactBase.h"

class Player;
class PlayerAttack;
class InputManager;



class PlayerMovement {
private:
	static PlayerMovement* instance; // シングルトン用
private:
	Player* pPlayer;

	InputManager* input;

	static const int AFTIMAGENUM = 20;
	VECTOR afterImagePos[AFTIMAGENUM];
	float afterImageRotY[AFTIMAGENUM];

	bool isBlinking;
	float blinkTimer;

	bool evasionButtonPressed;

	float evasionCooldown;        // クールダウンタイマー
	const float EVASION_COOLDOWN_TIME = 2.0f; // 2秒クールダウン

	const float ATTACK_INPUT_COOLDOWN_TIME = 0.2f; // 0.2秒クールダウン

	float evasionSpeed;

	VECTOR inputVec;

	PlayerAttack* playerAttack;

	bool dashState;

	AttackincreasesforSeveralSecondsAfterEvasion* attactArtifact;


public:
	PlayerMovement(Player* _player);

	static PlayerMovement* CreateInstance(Player* _player);
	static PlayerMovement* GetInstance();
	static void DestroyInstance();

	void Start();
	void Update();
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
	inline bool IsBlinking() const { return isBlinking; }

	inline bool IsEvasionButtonPressed() const { return evasionButtonPressed; }

	inline bool IsDashState() const { return dashState; }

	inline Player* GetPlayer()const {return pPlayer;}

	inline void SetAttactArtifact(AttackincreasesforSeveralSecondsAfterEvasion* artifact) { attactArtifact = artifact; }
};
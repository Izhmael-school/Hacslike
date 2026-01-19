#include "PlayerMovement.h"
#include "Player.h"
#include "PlayerAttack.h"
#include "../../Camera/Camera.h"
#include "../../../Manager/TimeManager.h"
#include "../../../Manager/InputManager.h"

PlayerMovement* PlayerMovement::instance = nullptr;

#pragma region シングルトン関連
PlayerMovement* PlayerMovement::CreateInstance(Player* _player) {
	if (!instance) {
		instance = new PlayerMovement(_player);
	}
	return instance;
}

PlayerMovement* PlayerMovement::GetInstance() {
	return instance;
}

void PlayerMovement::DestroyInstance() {
	delete instance;
	instance = nullptr;
}
#pragma endregion

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="_player"></param>
PlayerMovement::PlayerMovement(Player* _player)
	: pPlayer(_player)
	, input(&InputManager::GetInstance())
	, isBlinking(false)
	, blinkTimer(0.0f)
	, evasionButtonPressed(false)
	, evasionCooldown(0.0f)
	, evasionSpeed(1.0f)
	, inputVec()
	, dashState(false)
	, audio()
	, blinkDirection(){
	Start();
}


/// <summary>
/// 初期化処理
/// </summary>
void PlayerMovement::Start() {
	audio.GetInstance().Load("Res/Audio/SE/Player/run.mp3", "run", false);

	// 残像データ初期化
	for (int i = 0; i < AFTIMAGENUM; i++) {
		afterImagePos[i] = pPlayer->GetPosition();
		afterImageRotY[i] = pPlayer->GetRotation().y;
	}
}

/// <summary>
/// 更新処理
/// </summary>
void PlayerMovement::Update() {
	if (!pPlayer->GetIsDead()) {
		inputVec = VZero;
		MoveInput();
		EvasionInput();
		UpdateMovement();
		UpdateBlink();
	}
}

/// <summary>
/// 描画処理
/// </summary>
void PlayerMovement::Render() {
	// --- 残像描画 ---
	if (isBlinking && !pPlayer->GetPlayerAttack()->IsAttacking()) {
		for (int i = AFTIMAGENUM - 1; i >= 0; i -= 4) {
			int alpha = 255 - 255 * i / AFTIMAGENUM;

			// モデルの座標・回転設定
			MATRIX matRot = MGetRotY(Deg2Rad(afterImageRotY[i]));
			MATRIX matTrans = MGetTranslate(afterImagePos[i]);
			MATRIX mat = MMult(matRot, matTrans);

			MV1SetMatrix(pPlayer->GetModelHandle(), mat);

			// 半透明描画
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			MV1DrawModel(pPlayer->GetModelHandle());
		}
	}
}

void PlayerMovement::ForceMove(float speed) {
	// 攻撃開始時に固定した方向へ進む
	VECTOR moveVec = VScale(lockedDirection, speed);
	VECTOR pos = VAdd(pPlayer->GetPosition(), moveVec);

	// 壁判定を通して座標をセット（壁抜け防止）
	VECTOR correctedPos = pPlayer->CheckWallToWallRubbing(pos);
	pPlayer->SetPosition(correctedPos.x, correctedPos.y, correctedPos.z);
}

void PlayerMovement::LockDirection() {
	lockedDirection = pPlayer->GetForward();
}

void PlayerMovement::StopDash() {
	evasionSpeed = 1.0f; // スピードを通常に戻す
	dashState = false;   // ダッシュフラグを下ろす
}

/// <summary>
/// 移動・アニメーション・回転処理
/// </summary>
void PlayerMovement::UpdateMovement() {
	if (pPlayer->GetPlayerAttack()->IsAttacking()) return;

	if (VSquareSize(inputVec) >= 0.01f || isBlinking) {

		VECTOR moveDirection;

		if (isBlinking) {
			// --- 回避中は保存した方向を使い続ける (スティック無視) ---
			moveDirection = blinkDirection;
		}
		else {
			// --- 通常時はスティック入力から計算 ---
			VECTOR normalizedInput = VNorm(inputVec);
			MATRIX mRotY = MGetRotY(Deg2Rad(Camera::main->GetRotation().y));
			moveDirection = VTransform(normalizedInput, mRotY);

			// 回転更新 (通常時のみ)
			VECTOR rot = pPlayer->GetRotation();
			rot.y = Rad2Deg(atan2f(moveDirection.x, moveDirection.z)) + 180.0f;
			pPlayer->SetRotation(rot);
		}

		// --- 座標更新 (moveDirection を元に移動) ---
		float moveMagnitude = 10.0f * evasionSpeed;
		VECTOR pos = pPlayer->GetPosition();
		pos = VAdd(pos, VScale(moveDirection, moveMagnitude));

		VECTOR dir = pPlayer->CheckWallToWallRubbing(pos);
		pPlayer->SetPosition(dir.x, dir.y, dir.z);

		// アニメーション (通常時のみ)
		if (!isBlinking) {
			if (evasionSpeed >= 1.2f) {
				pPlayer->GetAnimator()->Play(5, 0.5f);
				dashState = true;
			}
			else {
				pPlayer->GetAnimator()->Play(1, 1.3f);
				dashState = false;
			}
			audio.PlayOneShot("run");
		}
	}
	else {
		pPlayer->GetAnimator()->Play(0);
		evasionSpeed = 1.0f;
		dashState = false;
	}
}


/// <summary>
/// 移動入力
/// </summary>
void PlayerMovement::MoveInput() {
	inputVec = VGet(input->IsJoypadSthick("L_Horizontal"),
		0.0f,
		input->IsJoypadSthick("L_Vertical"));

	if (input->IsKey(KEY_INPUT_W))
		inputVec.z = inputVec.z + pPlayer->GetSpeed();
	if (input->IsKey(KEY_INPUT_S))
		inputVec.z = inputVec.z - pPlayer->GetSpeed();
	if (input->IsKey(KEY_INPUT_D))
		inputVec.x = inputVec.x + pPlayer->GetSpeed();
	if (input->IsKey(KEY_INPUT_A))
		inputVec.x = inputVec.x - pPlayer->GetSpeed();

	if (input->IsKey(KEY_INPUT_Q))
		inputVec = VAdd(inputVec, VUp);
}

/// <summary>
/// 回避入力
/// </summary>
void PlayerMovement::EvasionInput() {
	// ===== 回避入力 =====
	bool isEvasionButtonDown = input->IsKeyDown(KEY_INPUT_LSHIFT) || input->IsButtonDown(XINPUT_GAMEPAD_A) || input->IsMouseDown(MOUSE_INPUT_MIDDLE);

	if (isEvasionButtonDown && !evasionButtonPressed && evasionCooldown <= 0.0f && VSize(inputVec) != 0 && !pPlayer->GetPlayerAttack()->IsAttacking()) {
		// 押した瞬間＆クールダウン終了時のみ回避
		evasionButtonPressed = true;
		evasionCooldown = EVASION_COOLDOWN_TIME; // クールダウン開始
		Evasion();
	}
	else if (!isEvasionButtonDown) {
		// ボタンを離したら押下フラグリセット
		evasionButtonPressed = false;
	}

	// ===== 毎フレームクールダウン減算 =====
	if (evasionCooldown > 0.0f)
		evasionCooldown -= TimeManager::GetInstance().deltaTime;
}

/// <summary>
/// 回避
/// </summary>
void PlayerMovement::Evasion() {

	pPlayer->GetCollider()->SetEnable(false);

	// --- 回避開始時の移動方向を確定・保存する ---
	if (VSquareSize(inputVec) >= 0.01f) {
		// 現在の入力から方向を計算
		MATRIX mRotY = MGetRotY(Deg2Rad(Camera::main->GetRotation().y));
		blinkDirection = VNorm(VTransform(inputVec, mRotY)); // 正規化して保存

		// 向き（回転）もここで確定
		VECTOR rot = pPlayer->GetRotation();
		rot.y = Rad2Deg(atan2f(blinkDirection.x, blinkDirection.z)) + 180.0f;
		pPlayer->SetRotation(rot);
	}
	else {
		// 入力がない場合は正面に進む（または回避をキャンセルする等の処理）
		blinkDirection = pPlayer->GetForward();
	}

	evasionSpeed = 6;
	// 残像開始
	isBlinking = true;
	blinkTimer = 0.15f;

	if (attactArtifact)
		attactArtifact->OnBlinking(this);
	if (CriticalArtifact)
		CriticalArtifact->OnBlinking(this);


	// --- 履歴をすべて現在位置にリセット ---
	for (int i = 0; i < AFTIMAGENUM; i++) {
		afterImagePos[i] = pPlayer->GetPosition();
		afterImageRotY[i] = pPlayer->GetRotation().y;
	}
}

/// <summary>
/// BlinkのUpdate
/// </summary>
void PlayerMovement::UpdateBlink() {
	// --- ブリンク中の処理 ---
	if (isBlinking && !pPlayer->GetPlayerAttack()->IsAttacking()) {
		blinkTimer -= TimeManager::GetInstance().deltaTime;
		if (blinkTimer <= 0.0f) {
			pPlayer->GetCollider()->SetEnable(true);
			isBlinking = false;
			Dash();
			blinkTimer = 0;
		}

		// 先に最新位置を入れる
		afterImagePos[0] = pPlayer->GetPosition();
		afterImageRotY[0] = pPlayer->GetRotation().y;

		// 古い残像を後ろにずらす
		for (int i = AFTIMAGENUM - 1; i > 0; i--) {
			afterImagePos[i] = afterImagePos[i - 1];
			afterImageRotY[i] = afterImageRotY[i - 1];
		}
	}
}

/// <summary>
/// ダッシュ
/// </summary>
void PlayerMovement::Dash() {
	evasionSpeed = 1.2f;
}

#include "PlayerMovement.h"
#include "../Hacslike/Scr/Manager/InputManager.h"
#include "Player.h"
#include "PlayerAttack.h"
#include "../../Camera/Camera.h"
#include "../../../Manager/TimeManager.h"

PlayerMovement* PlayerMovement::instance = nullptr;

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
	, audio(){
	Start();
}

PlayerMovement* PlayerMovement::CreateInstance(Player* _player)
{
	if (!instance) {
		instance = new PlayerMovement(_player);
	}
	return instance;
}

PlayerMovement* PlayerMovement::GetInstance()
{
	return instance;
}

void PlayerMovement::DestroyInstance()
{
	delete instance;
	instance = nullptr;
}

void PlayerMovement::Start() {
	audio.GetInstance().Load("Res/Audio/SE/Player/run.mp3", "run", false);

	// 残像データ初期化
	for (int i = 0; i < AFTIMAGENUM; i++) {
		afterImagePos[i] = pPlayer->GetPosition();
		afterImageRotY[i] = pPlayer->GetRotation().y;
	}
}

void PlayerMovement::Update() {
	if (!pPlayer->GetIsDead()) {
		inputVec = VZero;
		MoveInput();
		EvasionInput();
		UpdateMovement();
		UpdateBlink();
	}
	
}

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

/// <summary>
/// 移動・アニメーション・回転処理
/// </summary>
void PlayerMovement::UpdateMovement() {
	if (!pPlayer->GetPlayerAttack()->IsAttacking()) {
		//	入力があれば
		if (VSquareSize(inputVec) >= 0.01f) {
			//	入力ベクトルの正規化
			inputVec = VNorm(inputVec);

			inputVec = VScale(inputVec, pPlayer->GetSpeed());

			inputVec = VScale(inputVec, evasionSpeed);

			//	カメラからみた移動する方向ベクトル
			VECTOR moveDirection = VZero;

			MATRIX mRotY = MGetRotY(Deg2Rad(Camera::main->GetRotation().y));		//	カメラのY軸回転行列
			moveDirection = VTransform(inputVec, mRotY);

			VECTOR pos = pPlayer->GetPosition();
			pos = VAdd(pos, VScale(moveDirection, 10.0f));
			VECTOR dir = pPlayer->CheckWallToWallRubbing(pos); // ← 壁補正（これで壁抜けしない）
			pPlayer->SetPosition(dir.x, dir.y, dir.z);

			VECTOR rot = pPlayer->GetRotation();
			rot.y = Rad2Deg(atan2f(moveDirection.x, moveDirection.z)) + 180.0f;
			pPlayer->SetRotation(rot);

			//	移動アニメーションを再生
			if (evasionSpeed >= 1.2f) {
				pPlayer->GetAnimator()->Play(5, 0.5f);
				dashState = true;
				audio.PlayOneShot("run");
			}
			else {
				pPlayer->GetAnimator()->Play(1, 1.3f);
				dashState = false;
				audio.PlayOneShot("run");
			}
		}
		else {
			//	待機アニメーションを再生
			pPlayer->GetAnimator()->Play(0);
			evasionSpeed = 1;
			dashState = false;
		}
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

	// 瞬間移動
	evasionSpeed = 6;

	if(attactArtifact)
		attactArtifact->OnBlinking(this);
	if (CriticalArtifact)
		CriticalArtifact->OnBlinking(this);

	// 残像開始
	isBlinking = true;
	blinkTimer = 0.15f;


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

#include "Player.h"
#include "../../../Manager/InputManager.h"
#include "../../../Manager/SceneManager.h"
#include "../../../Manager/FadeManager.h"
#include "../../Camera/Camera.h"
#include "../../../Component/Collider/Collider.h"
#include "../../../Definition.h"
//#include "../Hacslike/Scr/_sekino/Manager/EffectManager.h"
//#include "../Hacslike/Scr/_sekino/Manager/Effect/Effect.h"

/*
 *	@brief		コンストラクタ
 *	@param[in]	VECTOR _pos		初期化する座標
 */
Player::Player(VECTOR _pos)
	: Character(_pos, "Player")
	, isAttacking(false)
	, pWeapon(nullptr)
	, XY()
	, slashes()						//	斬撃
	, attackIndex(0)				//	コンボが何段目か
	, attackTimer(0.0f)				//	クールタイム
	, canNextAttack(false)			//	次の攻撃ができるか
	, CapsuleHitboxes()				//	1,2段目のHitBox(Capsule)
	, SphereHitboxes()				//	3段目のHitBox(Sphere)
	, isBlinking(false)				//	回避の長押し防止
	, blinkTimer(0.0f)				//	回避のクールダウン
	, attackInputCooldown(0.0f)		//	
	, attackButtonPressed(false)	//	
	, evasionButtonPressed(false)	//	
	, evasionCooldown(0.0f)
	, evasionSpeed(1.0f) {
	Start();
}

/*
 *	@breif		デストラクタ
 */
Player::~Player() {
	delete pWeapon;
	pWeapon = nullptr;

	MV1DeleteModel(PLAYER_MODEL_HANDLE);
}

/*
 *	@function	Start
 *	@breif		初期化処理
 */
void Player::Start() {
	//	非表示だったら初期化しない
	if (!isVisible)
		return;
	SetCollider(new CapsuleCollider(this, VZero, VScale(VUp, 200), 50.0f));

	modelHandle = PLAYER_MODEL_HANDLE;

	pAnimator->SetModelHandle(modelHandle);

	SetPlayer(this);

	//	アニメーションの読み込み
	GetAnimator()->Load("Res/PlayerModel/Neutral.mv1", true);
	GetAnimator()->Load("Res/PlayerModel/Walking.mv1", true);
	GetAnimator()->Load("Res/PlayerModel/Attack1.mv1");
	GetAnimator()->Load("Res/PlayerModel/Attack2.mv1");
	GetAnimator()->Load("Res/PlayerModel/Attack3.mv1");
	GetAnimator()->Load("Res/PlayerModel/Run.mv1", true);

	pAnimator->Play(0);

	// 残像データ初期化
	for (int i = 0; i < AFTIMAGENUM; i++) {
		afterImagePos[i] = position;
		afterImageRotY[i] = rotation.y;
	}



	////	Z軸方向を正面に向かせる
	//rotation.y = 180;

}

/*
 *	@function	Update
 *	@breif		更新処理
 */
void Player::Update() {
	//	非表示だったら更新しない
	if (!isVisible)
		return;

	//	入力管理クラスの取得
	InputManager* input = InputManager::GetInstance();
#pragma region 移動入力処理


	//	入力ベクトル
	VECTOR inputVec = VZero;

	GetJoypadXInputState(DX_INPUT_PAD1, &XY);

	if (XY.ThumbLY >= 1000 || input->IsKey(KEY_INPUT_W))
		inputVec = VAdd(inputVec, VForward);
	if (XY.ThumbLY <= -1000 || input->IsKey(KEY_INPUT_S))
		inputVec = VAdd(inputVec, VBack);
	if (XY.ThumbLX >= 1000 || input->IsKey(KEY_INPUT_D))
		inputVec = VAdd(inputVec, VRight);
	if (XY.ThumbLX <= -1000 || input->IsKey(KEY_INPUT_A))
		inputVec = VAdd(inputVec, VLeft);

	/*if (input->IsKey(KEY_INPUT_Q))
		inputVec = VAdd(inputVec, VUp);
	if (input->IsKey(KEY_INPUT_E))
		inputVec = VAdd(inputVec, VDown);*/
#pragma endregion

#pragma region 攻撃入力処理
		// ===== 攻撃入力 =====
	bool isButtonDown = input->IsKeyDown(KEY_INPUT_E) || InputManager::GetInstance()->IsButtonDown(XINPUT_BUTTON_X);

	if (isButtonDown && !attackButtonPressed) {
		// ボタンが押された瞬間だけ処理
		attackButtonPressed = true;

		if (!isAttacking && !isBlinking) {
			// --- 1段目攻撃 ---
			isAttacking = true;
			attackIndex = 1;
			attackTimer = 0.0f;
			canNextAttack = false;
			pAnimator->Play(2); // 攻撃1モーション
		}
		else if (canNextAttack && attackIndex < 3) {
			// --- コンボ入力 ---
			attackIndex++;
			attackTimer = 0.0f;
			canNextAttack = false;
			pAnimator->Play(1 + attackIndex); // 攻撃2→3
		}
	}
	else if (!isButtonDown) {
		// ボタンを離したらフラグリセット
		attackButtonPressed = false;
	}

	// ===== 攻撃中のタイマー管理 =====
	if (isAttacking) {
		attackTimer += 1.0f / 60.0f;

		if (attackTimer > 0.2f && attackTimer < 0.6f) canNextAttack = true;

		// 攻撃判定生成
		if (attackIndex == 1 && attackTimer > 0.18f && attackTimer < 0.22f)
			CreateAttackHitbox(30.0f, 80.0f);
		if (attackIndex == 2 && attackTimer > 0.22f && attackTimer < 0.28f)
			CreateAttackHitbox(40.0f, 110.0f);
		if (attackIndex == 3 && attackTimer > 0.25f && attackTimer < 0.33f)
			CreateAttackHitbox(0.0f, 150.0f); // 周囲攻撃

		if (attackTimer > 0.8f) {
			isAttacking = false;
			canNextAttack = false;
			attackIndex = 0;
		}
	}

	// ヒットボックス更新
	for (auto it = CapsuleHitboxes.begin(); it != CapsuleHitboxes.end();) {
		CapsuleHitBox* h = *it;
		h->Update();
		if (h->IsDead()) {
			delete h;
			it = CapsuleHitboxes.erase(it);
		}
		else ++it;
	}

	for (auto it = SphereHitboxes.begin(); it != SphereHitboxes.end();) {
		SphereHitBox* h = *it;
		h->Update();
		if (h->IsDead()) {
			delete h;
			it = SphereHitboxes.erase(it);
		}
		else ++it;
	}
#pragma endregion

#pragma region 回避入力処理


	// ===== 回避入力 =====
	bool isEvasionButtonDown = input->IsKeyDown(KEY_INPUT_SPACE) || InputManager::GetInstance()->IsButtonDown(XINPUT_BUTTON_A);

	if (isEvasionButtonDown && !evasionButtonPressed && evasionCooldown <= 0.0f && VSize(inputVec) != 0) {
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
		evasionCooldown -= 1.0f / 60.0f; // 60fps前提、deltaTimeがあればそれでもOK

	/*if (pAnimator->GetCurrentAnimation() != 2) {
		isAttacking = false;
	}*/
#pragma endregion

	CheckWall();

	if (!isAttacking) {
		//	入力があれば
		if (VSquareSize(inputVec) >= 0.01f) {
			//	入力ベクトルの正規化
			inputVec = VNorm(inputVec);

			inputVec = VScale(inputVec, evasionSpeed);

			//	カメラからみた移動する方向ベクトル
			VECTOR moveDirection = VZero;

			MATRIX mRotY = MGetRotY(Deg2Rad(Camera::main->GetRotation().y));		//	カメラのY軸回転行列
			moveDirection = VTransform(inputVec, mRotY);

			//	自身のy軸回転を計算した値に変更する
			rotation.y = Rad2Deg(atan2f(moveDirection.x, moveDirection.z)) + 180.0f;

			//	計算した入力ベクトル
			position = VAdd(position, VScale(moveDirection, 10.0f));

			//	移動アニメーションを再生
			pAnimator->Play(1, 1.3f);
		}
		else {
			//	待機アニメーションを再生
			pAnimator->Play(0);
		}
	}

#pragma region 回避時処理

	// --- ブリンク中の処理 ---
	if (isBlinking && !isAttacking) {
		blinkTimer -= 1.0f / 60.0f;   // 1フレーム経過（60FPS想定）
		if (blinkTimer <= 0.0f) {
			isBlinking = false;
			evasionSpeed = 1;
			pCollider->SetEnable(true);
		}

		pAnimator->Play(5, 2);

		// 先に最新位置を入れる
		afterImagePos[0] = position;
		afterImageRotY[0] = rotation.y;

		// 古い残像を後ろにずらす
		for (int i = AFTIMAGENUM - 1; i > 0; i--) {
			afterImagePos[i] = afterImagePos[i - 1];
			afterImageRotY[i] = afterImageRotY[i - 1];
		}
		}
#pragma endregion

	pAnimator->Update();
	GameObject::Update();

#pragma region Slash処理

	for (auto it = slashes.begin(); it != slashes.end();) {
		Slash* s = *it;
		s->Update();

		if (s->IsDead()) {
			delete s;
			it = slashes.erase(it);
		}
		else {
			++it;
		}
	}
#pragma endregion

	////計算した座標、回転(オイラー角)、拡縮モデルに反映する
	//MV1SetPosition(modelHandle, position);
	//MV1SetRotationXYZ(modelHandle, VScale(rotation, DX_PI_F / 180.0f));
	//MV1SetScale(modelHandle, scale);
	MV1SetMatrix(modelHandle, matrix);

	//	武器の更新
	if (pCollider != nullptr && pWeapon != nullptr) {
		pWeapon->Update();
	}
	}

/*
 *	@function	Render
 *	@breif		描画処理
 */
void Player::Render() {
	//	非表示だったら描画しない
	if (!isVisible)
		return;

#pragma region 残像描画処理

	// --- 残像描画 ---
	if (isBlinking && !isAttacking) {
		for (int i = AFTIMAGENUM - 1; i >= 0; i -= 4) {
			int alpha = 255 - 255 * i / AFTIMAGENUM;

			// モデルの座標・回転設定
			MATRIX matRot = MGetRotY(Deg2Rad(afterImageRotY[i]));
			MATRIX matTrans = MGetTranslate(afterImagePos[i]);
			MATRIX mat = MMult(matRot, matTrans);

			MV1SetMatrix(modelHandle, mat);

			// 半透明描画
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
			MV1DrawModel(modelHandle);
		}
	}
#pragma endregion

	// --- 通常モデル描画 ---
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	MV1SetMatrix(modelHandle, matrix);
	MV1DrawModel(modelHandle);

	for (auto s : slashes) {
		s->Render();
	}

#pragma region 攻撃時の当たり判定描画処理


	// ヒットボックス描画（デバッグ）
	for (auto h : CapsuleHitboxes) {
		h->Render();
	}

	// ヒットボックス描画（デバッグ）
	for (auto h : SphereHitboxes) {
		h->Render();
	}

#pragma endregion

	//	武器の描画
	if (pCollider != nullptr && pWeapon != nullptr) {
		pWeapon->Render();
	}

	if (pCollider != nullptr) {
		pCollider->Render();
	}

}

/// <summary>
/// 斬撃
/// </summary>
void Player::AttackEnd() {
	VECTOR forward = VNorm(VGet(
		-sinf(Deg2Rad(rotation.y)),
		0.0f,
		-cosf(Deg2Rad(rotation.y))
	));

	// 最後の攻撃で斬撃発生
	Slash* s = new Slash(position, forward);
	slashes.push_back(s);
}

/// <summary>
/// 攻撃時の当たり判定
/// </summary>
/// <param name="length"></param>
/// <param name="radius"></param>
void Player::CreateAttackHitbox(float length, float radius) {
	//Unity座標系の前方
	VECTOR forward = VNorm(VGet(
		-sinf(Deg2Rad(rotation.y)),
		0.0f,
		-cosf(Deg2Rad(rotation.y))
	));

	VECTOR start, end;
	float life = 0.20f;

	if (attackIndex < 3) {
		// 1～2段目 前方カプセル
		start = VAdd(position, VScale(forward, 20.0f));
		end = VAdd(start, VScale(forward, length));
		CapsuleHitBox* hit = new CapsuleHitBox(this, start, end, radius, life);
		CapsuleHitboxes.push_back(hit);
	}
	else {
		// 3段目 周囲攻撃（球状に近いカプセル）
		life = 0.25f;
		// プレイヤー中心 + 少し前方にオフセット
		VECTOR center = VAdd(position, VScale(forward, 50.0f)); // 50.0f は前に出す距離
		// 少し高さを上げたい場合は Y 成分を足す
		center.y += 50.0f; // optional: 地面から少し上に

		// SphereHitBox を生成
		float life = 0.25f;
		VECTOR offset = VAdd(VScale(forward, 70.0f), VGet(0.0f, 100.0f, 0.0f));

		SphereHitBox* hit = new SphereHitBox(this, offset, radius, life);
		SphereHitboxes.push_back(hit);
	}

}

/// <summary>
/// 回避
/// </summary>
void Player::Evasion() {
	pCollider->SetEnable(false);

	// 瞬間移動
	evasionSpeed = 5;

	// 残像開始
	isBlinking = true;
	blinkTimer = 0.25f;


	// --- 履歴をすべて現在位置にリセット ---
	for (int i = 0; i < AFTIMAGENUM; i++) {
		afterImagePos[i] = position;
		afterImageRotY[i] = rotation.y;
	}
}


/*
 *	@function	OnTriggerEnter
 *	@brief		当たった瞬間
 *	@param[in]	Collider* _pCol
 */
void Player::OnTriggerEnter(Collider* _pCol) {
	//	当たった相手のタグが "Goblin" だったら
	if (_pCol->GetGameObject()->GetTag() == "Goblin") {
		//	当たった相手を非表示にする
		//_pCol->GetGameObject()->SetVisible(false);
	/*	EffectManager::GetInstance()->Load("Res/Effect/01.efk", "FireFlower", 50.0f);
		Effect* pEffe = EffectManager::GetInstance()->Instantiate("FireFlower", position);

		Camera::main->Shake(2, 0.5f);*/

		//SceneManager::GetInstance()->SetNext(SceneType::GameOver);
		FadeManager::GetInstance()->FadeOut();
	}
}

/*wsa
 *	@function	OnTriggerSaty
 *	@brief		当たっている間
 *	@param[in]	Collider* _pCol
 */
void Player::OnTriggerStay(Collider* _pCol) {
}

/*
 *	@function	OnTriggerExit
 *	@brief		離れた瞬間
 *	@param[in]	Collider* _pCol
 */
void Player::OnTriggerExit(Collider* _pCol) {
}

#include "PlayerAttack.h"
#include "Player.h"
#include "../../Weapon/Weapon.h"
#include "../../../Manager/TimeManager.h"
#include "../../../Manager/EffectManager.h"
#include <vector>
#include "DxLib.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="_player"></param>
/// <param name="_weapon"></param>
/// <param name="_playerMovement"></param>
PlayerAttack::PlayerAttack(Player* _player, Weapon* _weapon, PlayerMovement* _playerMovement)
	: pPlayer(_player)
	, pWeapon(_weapon)
	, input(&InputManager::GetInstance())
	, isAttacking(false)
	, attackIndex(0)
	, attackTimer(0.0f)
	, canNextAttack(false)
	, attackButtonPressed(false)
	, CapsuleHitboxes()
	, SphereHitboxes()
	, playerMovement(_playerMovement)
	, isDashAttack(false)
	, isCharging(false)
	, chargeTime(0.0f)
	, maxChargeTime(3.0f)
	, pSpherePool(nullptr)
	, pCapsulePool(nullptr)
	, addRadius()
	, currentChainCount()
	, maxChainCount() {
	pSpherePool = &BulletPool::GetInstance();
	pCapsulePool = &CapsuleHitPool::GetInstance();
	Start();
}

/// <summary>
/// 初期化
/// </summary>
void PlayerAttack::Start() {
	EffectManager::GetInstance().Load("Res/Effect/ChargeAttackEfk.efk", "ChargeBlad", 50.0f);

	AudioManager::GetInstance().Load("Res/Audio/SE/Player/ChargeAttackVoice.mp3", "chargeAttack", false);
	AudioManager::GetInstance().Load("Res/Audio/SE/Player/Sword.mp3", "Sword", false);
	AudioManager::GetInstance().Load("Res/Audio/SE/Player/GreatAttack1.mp3", "GAtk1", false);
	AudioManager::GetInstance().Load("Res/Audio/SE/Player/se_furi2.mp3", "furi", false);
}

/// <summary>
/// 更新処理
/// </summary>
void PlayerAttack::Update() {

	if (!pPlayer->GetIsDead())
		AttackInput();

	// 弾プールの更新
	pSpherePool->Update();

	//	Capsuleプール更新
	pCapsulePool->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void PlayerAttack::Render() {

}

/// <summary>
/// 攻撃入力・HitBox更新
/// </summary>
void PlayerAttack::AttackInput() {
#pragma region 入力処理
	//	攻撃入力
	bool isButtonDown = input->IsMouseDown(MOUSE_INPUT_LEFT) || input->IsButtonDown(XINPUT_GAMEPAD_X);
	bool isButton = input->IsMouse(MOUSE_INPUT_LEFT) || input->IsButton(XINPUT_GAMEPAD_X);
	bool isButtonUp = input->IsMouseUp(MOUSE_INPUT_LEFT) || input->IsButtonUp(XINPUT_GAMEPAD_X);

	//	ため攻撃入力
	bool isChargeButtonDown = input->IsMouseDown(MOUSE_INPUT_RIGHT) || input->IsButtonDown(XINPUT_GAMEPAD_RIGHT_SHOULDER);
	bool isChargeButton = input->IsMouse(MOUSE_INPUT_RIGHT) || input->IsButton(XINPUT_GAMEPAD_RIGHT_SHOULDER);
	bool isChargeButtonUp = input->IsMouseUp(MOUSE_INPUT_RIGHT) || input->IsButtonUp(XINPUT_GAMEPAD_RIGHT_SHOULDER);
#pragma endregion

#pragma region ため攻撃処理
	//	チャージ開始
	if (isChargeButtonDown && !isAttacking && !isCharging && !playerMovement->IsBlinking() && pWeapon->GetType() != 3) {
		isAttacking = true;
		if (pWeapon->GetType() == 1) {
			chargeTime = 0.0f;
			isCharging = true;
			pPlayer->GetAnimator()->Play("GreatCharge1", 1.3f);
		}
	}

	//	チャージ中
	if (isCharging) {
		chargeTime += TimeManager::GetInstance().deltaTime;
		Effect* pEffe = EffectManager::GetInstance().Instantiate("ChargeBlad", pPlayer->GetPosition());
		//	溜め中アニメーションに切り替え
		if (chargeTime >= 0.65f)
			pPlayer->GetAnimator()->Play("GreatCharge2", 1.3f);

		//	最大チャージで自動リリース
		if (chargeTime >= maxChargeTime) {
			isChargeButtonUp = true;
		}
	}

	//	チャージ終了
	if (isChargeButtonUp && isCharging) {
		isCharging = false;
		isAttacking = true;
		attackTimer = 0.0f;

		float ratio = chargeTime / maxChargeTime;
		float chargeRatio = (ratio < 1.0f) ? ratio : 1.0f;

		if (chargeRatio < 0.75f) {
			pPlayer->GetAnimator()->Play("GreatCharge3", 1.3f);
			attackIndex = 4;
			AudioManager::GetInstance().PlayOneShot("chargeAttack");
		}
		else if (chargeRatio < 1.2f) {
			pPlayer->GetAnimator()->Play("GreatCharge3", 1.3f);
			attackIndex = 4;
			AudioManager::GetInstance().PlayOneShot("chargeAttack");;
		}
		else {
			pPlayer->GetAnimator()->Play("GreatCharge3", 1.3f);
			attackIndex = 4;
			AudioManager::GetInstance().PlayOneShot("chargeAttack");
		}
	}
#pragma endregion

#pragma region コンボ処理
	if (isButtonDown && !attackButtonPressed && !playerMovement->IsBlinking()) {
		//	ボタンが押された瞬間だけ処理
		attackButtonPressed = true;

		//	Sword
		if (pWeapon->GetType() == 0) {
			std::string animName = "Atk" + std::to_string(attackIndex + 1);
			if (!isAttacking && playerMovement->IsDashState() && checkDashAttack) {
				isAttacking = true;
				isDashAttack = true;
				attackIndex = 4;
				attackTimer = 0.0f;

				// 攻撃開始時の向きを固定
				playerMovement->LockDirection();

				playerMovement->StopDash();

				pPlayer->GetAnimator()->Play("Atk3", pWeapon->GetAnimationSpeed(attackIndex - 1));
				AudioManager::GetInstance().PlayOneShot("Sword");
			}
			else if (!isAttacking) {
				//	1段目攻撃
				isAttacking = true;
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1)); // 攻撃1モーション
				AudioManager::GetInstance().PlayOneShot("Sword");
			}
			else if (canNextAttack && attackIndex < 3) {
				//	コンボ入力
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1)); // 攻撃2→3
				AudioManager::GetInstance().PlayOneShot("Sword");
			}
		}

		//	GreatSword
		else if (pWeapon->GetType() == 1) {
			std::string animName = "GreatAtk" + std::to_string(attackIndex + 1);
			//if (!isAttacking && playerMovement->IsDashState() && checkDashAttack) {
			//	isAttacking = true;
			//	isDashAttack = true;
			//	attackIndex = 4;
			//	attackTimer = 0.0f;

			//	// 攻撃開始時の向きを固定
			//	playerMovement->LockDirection();

			//	pPlayer->GetAnimator()->Play("GreatAtk4", pWeapon->GetAnimationSpeed(attackIndex - 1));
			//}
			if (!isAttacking) {
				//	1段目攻撃
				isAttacking = true;
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1)); // 攻撃1モーション
				AudioManager::GetInstance().PlayOneShot("GAtk1");
			}
			else if (canNextAttack && attackIndex < 3) {
				//	コンボ入力
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1));
				AudioManager::GetInstance().PlayOneShot("furi");
			}
		}

		//	Axe
		else if (pWeapon->GetType() == 2) {
			std::string animName = "AxeAtk" + std::to_string(attackIndex + 1);
			if (!isAttacking && playerMovement->IsDashState() && checkDashAttack) {
				isAttacking = true;
				isDashAttack = true;
				attackIndex = 4;
				attackTimer = 0.0f;

				// 攻撃開始時の向きを固定
				playerMovement->LockDirection();

				playerMovement->StopDash();

				pPlayer->GetAnimator()->Play("AxeAtk4", pWeapon->GetAnimationSpeed(attackIndex - 1));
				AudioManager::GetInstance().PlayOneShot("furi");
			}
			else if (!isAttacking) {
				//	1段目攻撃
				isAttacking = true;
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1)); // 攻撃1モーション
				AudioManager::GetInstance().PlayOneShot("furi");
			}
			else if (canNextAttack && attackIndex < 3) {
				//	コンボ入力
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1));
				AudioManager::GetInstance().PlayOneShot("furi");
			}
		}

		//	Lance
		else if (pWeapon->GetType() == 4) {
			std::string animName = "GreatAtk" + std::to_string(attackIndex + 1);
			if (!isAttacking && playerMovement->IsDashState() && checkDashAttack) {
				isAttacking = true;
				isDashAttack = true;   // フラグをON
				attackIndex = 4;
				attackTimer = 0.0f;

				// 攻撃開始時の向きを固定
				playerMovement->LockDirection();

				playerMovement->StopDash();

				pPlayer->GetAnimator()->Play("LanceAtk1", pWeapon->GetAnimationSpeed(attackIndex - 1));
			}
			else if (!isAttacking) {
				//	1段目攻撃
				isAttacking = true;
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1)); // 攻撃1モーション
				AudioManager::GetInstance().PlayOneShot("GAtk1");
			}
			else if (canNextAttack && attackIndex < 3) {
				//	コンボ入力
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1));
				AudioManager::GetInstance().PlayOneShot("furi");
			}
		}
	}
#pragma endregion

#pragma region 遠距離武器
	//	遠距離武器
	else if (isButtonUp && pWeapon->GetType() == 3) {
		if (!isAttacking) {
			isAttacking = true;
			attackIndex = 1;
			attackTimer = 0.0f;
			canNextAttack = false;

			// --- チャージ仕様の追加 ---
			// 例: 0.5秒以上溜めていたらチャージ攻撃、それ以外は通常攻撃
			if (addRadius >= 0.5f) {
				// チャージ攻撃（5点跳ね返り）
				// 引数にaddRadiusを渡して、CreateRangedHitBox内で威力や範囲を変えられるようにする
				CreateRangedHitBox();

				// チャージ攻撃は反動ダメージを大きくするなど
				pPlayer->Damage(5);
			}
			else {
				// 通常攻撃（まっすぐ飛ぶだけ、または跳ね返りなし）
				// 今回は既存のCreateRangedHitBoxを呼び出すが、中でaddRadiusを見て判別させる
				CreateRangedHitBox();

				pPlayer->Damage(1);
			}
		}
		// 離したのでリセット
		addRadius = 0.0f;
	}

	if (isButton && pWeapon->GetType() == 3) {
		addRadius += TimeManager::GetInstance().deltaTime;
	}
	else if (!isButtonDown) {
		// ボタンを離したらフラグリセット
		attackButtonPressed = false;
	}
#pragma endregion

	// AttackInput内のタイマー管理セクション
	if (isAttacking && !isCharging && !playerMovement->IsBlinking()) {
		attackTimer += TimeManager::GetInstance().deltaTime;

		// --- ダッシュ攻撃中の移動処理 ---
		if (isDashAttack) {
			// 攻撃の出だし（0.4秒間など）だけ前進
			if (attackTimer < 0.4f) {
				playerMovement->ForceMove(20.0f); // 速度は調整してください
			}
		}

#pragma region 攻撃判定生成
		//	Sword
		if (pWeapon->GetType() == 0) {

			if (attackTimer > 0.2f && attackTimer < 0.6f) canNextAttack = true;

			// 攻撃判定生成
			if (attackIndex == 1 && attackTimer > 0.18f && attackTimer < 0.22f)
				CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
			if (attackIndex == 2 && attackTimer > 0.22f && attackTimer < 0.28f)
				CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
			if (attackIndex == 3 && attackTimer > 0.25f && attackTimer < 0.33f)
				CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1)); // 周囲攻撃
			if (attackIndex == 4 && attackTimer > 0.28f && attackTimer < 0.33f)
				CreateAttackHitbox(pWeapon->GetColLength(2), pWeapon->GetColRadius(2)); // 周囲攻撃

			if (attackTimer > 0.8f) {
				isAttacking = false;
				isDashAttack = false;
				canNextAttack = false;
				attackIndex = 0;
			}
		}

		//	GreatSword
		else if (pWeapon->GetType() == 1) {

			if (attackTimer > 0.6f && attackTimer < 1.0f) canNextAttack = true;

			// 攻撃判定生成
			if (attackIndex == 1 && attackTimer > 0.25f && attackTimer < 0.3f)
				CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
			if (attackIndex == 2 && attackTimer > 0.35f && attackTimer < 0.45f)
				CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
			if (attackIndex == 3 && attackTimer > 1.3f && attackTimer < 2.2f)
				CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1)); // 周囲攻撃	
			if (attackIndex == 4 && attackTimer > 0.9f && attackTimer < 1.9f) {
				CreateAttackHitbox(pWeapon->GetColLength(2), pWeapon->GetColRadius(2)); // 周囲攻撃
			}
			if (attackIndex >= 3) {
				if (attackTimer > 2.78f) {
					isAttacking = false;
					isDashAttack = false;
					canNextAttack = false;
					attackIndex = 0;
				}
			}
			else {
				if (attackTimer > 1) {
					isAttacking = false;
					isDashAttack = false;
					canNextAttack = false;
					attackIndex = 0;
				}
			}
		}

		//	Axe
		else if (pWeapon->GetType() == 2) {

			if (attackTimer > 0.6f && attackTimer < 1.0f) canNextAttack = true;

			// 攻撃判定生成
			if (attackIndex == 1 && attackTimer > 0.25f && attackTimer < 0.30f)
				CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
			if (attackIndex == 2 && attackTimer > 0.35f && attackTimer < 0.40f)
				CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
			if (attackIndex == 3 && attackTimer > 0.35f && attackTimer < 0.50f)
				CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1)); // 周囲攻撃
			if (attackIndex == 4 && attackTimer > 0.36f && attackTimer < 0.48f)
				CreateAttackHitbox(pWeapon->GetColLength(2), pWeapon->GetColRadius(2)); // 周囲攻撃

			if (attackTimer > 1.2f) {
				isAttacking = false;
				isDashAttack = false;
				canNextAttack = false;
				attackIndex = 0;
			}
		}

		//	Lance
		else if (pWeapon->GetType() == 4) {

			if (attackTimer > 0.6f && attackTimer < 1.0f) canNextAttack = true;

			// 攻撃判定生成
			if (attackIndex == 1 && attackTimer > 0.25f && attackTimer < 0.30f)
				CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
			if (attackIndex == 2 && attackTimer > 0.35f && attackTimer < 0.40f)
				CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
			if (attackIndex == 3 && attackTimer > 0.35f && attackTimer < 0.50f)
				CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1)); // 周囲攻撃
			if (attackIndex == 4 && attackTimer > 0.32f && attackTimer < 0.35f)
				CreateAttackHitbox(pWeapon->GetColLength(2), pWeapon->GetColRadius(2)); // 周囲攻撃

			if (attackTimer > 1.2f) {
				isAttacking = false;
				isDashAttack = false;
				canNextAttack = false;
				attackIndex = 0;
			}
		}

		else if (pWeapon->GetType() == 3) {
			if (attackTimer > 0.4f && attackTimer < 1.0f) canNextAttack = true;
		}

		if (!pPlayer->GetAnimator()->IsPlaying()) {
			isAttacking = false;
			isDashAttack = false; // ★ここでもリセット
			canNextAttack = false;
			attackIndex = 0;
		}
	}
#pragma endregion

#pragma region ヒットボックス更新
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

	if (isAttacking) {
		if (!pPlayer->GetAnimator()->IsPlaying()) {   // もしくは IsEnd("Atk1") など
			isAttacking = false;
			attackIndex = 0;
			canNextAttack = false;
		}
	}

}

///<summary>
///攻撃時の当たり判定発生処理
///</summary>
///<param name="length"></param>
///<param name="radius"></param>
#pragma region 攻撃判定処理
/// <summary>
/// 遠距離武器用当たり判定
/// </summary>
void PlayerAttack::CreateRangedHitBox() {

	VECTOR forward = pPlayer->GetForward();
	VECTOR basePos = pPlayer->GetPosition();

	// --- チャージ量による変化 ---
	int pointCount = 5;
	if (addRadius < 0.5f) pointCount = 1;      // 溜めが短いと1箇所（直線的）
	else if (addRadius < 1.5f) pointCount = 3; // 中くらいなら3箇所
	else pointCount = 5;                      // 最大なら5箇所

	std::vector<VECTOR> points;
	float spreadAngle = 120.0f;
	float range = 500.0f;

	// pointCountの数だけ地点を生成
	for (int i = 0; i < pointCount; ++i) {
		// 分割数をpointCountに合わせる
		float angleRad = (i - (pointCount / 2)) * (spreadAngle / (pointCount > 1 ? (pointCount - 1) : 1)) * (3.14159265f / 180.0f);

		MATRIX rotMat = MGetRotY(angleRad);
		VECTOR rotDir = VTransform(forward, rotMat);

		VECTOR p = VAdd(basePos, VScale(rotDir, range));
		p.y += 40.0f;
		points.push_back(p);
	}

	// 2. 順番をランダムにシャッフル
	for (int i = 0; i < (int)points.size(); ++i) {
		int r = GetRand((int)points.size() - 1);
		VECTOR temp = points[i];
		points[i] = points[r];
		points[r] = temp;
	}

	// 3. 最初の移動先へ向けた初速を計算
	VECTOR startPos = VAdd(basePos, VScale(forward, 50.0f));
	VECTOR firstTarget = points[0];

	// VSub(目的地, 現在地) で方向を出し、VNormで正規化して速度を掛ける
	VECTOR diff = VSub(firstTarget, startPos);
	VECTOR dir = VNorm(diff);
	VECTOR vel = VScale(dir, 50.0f); // 弾の速度

	// 4. 弾の生成
	SphereHitBox* bullet = pSpherePool->BulletSpawn(pPlayer, startPos, vel, 30.0f, 3.0f, maxChainCount);

	if (bullet) {
		bullet->SetTargetList(points); // 5つの地点を教える
	}

	isAttacking = false;
	addRadius = 0.0f;
}

void PlayerAttack::CreateHitBox(VECTOR _pos, float _radius) {
	SphereHitBox* sphereBox = pSpherePool->Spawn(pPlayer, _pos, VGet(0, 0, 0), _radius, 0.25f);
}

void PlayerAttack::CreateAttackHitbox(float _length, float _radius) {
	VECTOR forward = pPlayer->GetForward();
	float life = 0.2f;

	if (attackIndex < 3) {
		//VECTOR startLocal = VScale(forward, 20.0f);           // ローカル座標
		//VECTOR endLocal = VScale(forward, 20.0f + _length); // ローカル座標
		VECTOR spawnPos = VAdd(pPlayer->GetPosition(), VAdd(VScale(forward, 100.0f), VGet(0, 100, 0)));
		SphereHitBox* sphereBox = pSpherePool->Spawn(pPlayer, spawnPos, VGet(0, 0, 0), _radius, 0.25f);
		//CapsuleHitBox* capsuleBox = pCapsulePool->Spawn(pPlayer, startLocal, endLocal, VGet(0, 0, 0), _radius, 0.25f);
	}
	else {
		VECTOR spawnPos = VAdd(pPlayer->GetPosition(), VAdd(VScale(forward, 100.0f), VGet(0, 100, 0)));
		SphereHitBox* sphereBox = pSpherePool->Spawn(pPlayer, spawnPos, VGet(0, 0, 0), _radius, 0.25f);
	}
}
#pragma endregion
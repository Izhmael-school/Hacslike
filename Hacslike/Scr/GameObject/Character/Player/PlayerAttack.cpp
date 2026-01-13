#include "PlayerAttack.h"
#include "Player.h"
#include "../../Weapon/Weapon.h"
#include "../../../Manager/TimeManager.h"
#include "../../../Manager/EffectManager.h"
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
		if (pWeapon->GetType() == 0) {
			std::string animName = "Atk" + std::to_string(attackIndex + 1);
			if (!isAttacking && playerMovement->IsDashState() && checkDashAttack) {
				isAttacking = true;
				isDashAttack = true;
				attackIndex = 3;
				attackTimer = 0.0f;

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

		else if (pWeapon->GetType() == 1) {
			std::string animName = "GreatAtk" + std::to_string(attackIndex + 1);
			if (!isAttacking && playerMovement->IsDashState() && checkDashAttack) {
				isAttacking = true;
				isDashAttack = true;
				attackIndex = 4;
				attackTimer = 0.0f;

				pPlayer->GetAnimator()->Play("GreatAtk4", pWeapon->GetAnimationSpeed(attackIndex - 1));
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

		else if (pWeapon->GetType() == 2) {
			std::string animName = "AxeAtk" + std::to_string(attackIndex + 1);
			if (!isAttacking && playerMovement->IsDashState() && checkDashAttack) {
				isAttacking = true;
				isDashAttack = true;
				attackIndex = 4;
				attackTimer = 0.0f;

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
			pPlayer->Damage(3);

			CreateRangedHitBox();   // ★ここを追加！
		}
	}

	else if (!isButtonDown) {
		// ボタンを離したらフラグリセット
		attackButtonPressed = false;
	}

	if (isButton && pWeapon->GetType() == 3) {
		addRadius += TimeManager::GetInstance().deltaTime;  // ★加算にする！
	}
	else {
		addRadius = 0.0f;  // 離したらリセットする場合
	}
#pragma endregion

	//	攻撃中のタイマー管理
	if (isAttacking && !isCharging && !playerMovement->IsBlinking()) {
		attackTimer += TimeManager::GetInstance().deltaTime;

#pragma region 攻撃判定生成
		if (pWeapon->GetType() == 0) {

			if (attackTimer > 0.2f && attackTimer < 0.6f) canNextAttack = true;

			// 攻撃判定生成
			if (attackIndex == 1 && attackTimer > 0.18f && attackTimer < 0.22f)
				CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
			if (attackIndex == 2 && attackTimer > 0.22f && attackTimer < 0.28f)
				CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
			if (attackIndex == 3 && attackTimer > 0.25f && attackTimer < 0.33f)
				CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1)); // 周囲攻撃
			if (attackIndex == 4)
				CreateAttackHitbox(pWeapon->GetColLength(2), pWeapon->GetColRadius(2)); // 周囲攻撃

			if (attackTimer > 0.8f) {
				isAttacking = false;
				canNextAttack = false;
				attackIndex = 0;
			}
		}

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
					canNextAttack = false;
					attackIndex = 0;
				}
			}
			else {
				if (attackTimer > 1) {
					isAttacking = false;
					canNextAttack = false;
					attackIndex = 0;
				}
			}
		}

		else if (pWeapon->GetType() == 2) {

			if (attackTimer > 0.6f && attackTimer < 1.0f) canNextAttack = true;

			// 攻撃判定生成
			if (attackIndex == 1 && attackTimer > 0.25f && attackTimer < 0.30f)
				CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
			if (attackIndex == 2 && attackTimer > 0.35f && attackTimer < 0.40f)
				CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
			if (attackIndex == 3 && attackTimer > 0.35f && attackTimer < 0.50f)
				CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1)); // 周囲攻撃
			if (attackIndex == 4 && attackTimer > 0.18f && attackTimer < 0.22f)
				CreateAttackHitbox(pWeapon->GetColLength(2), pWeapon->GetColRadius(2)); // 周囲攻撃

			if (attackTimer > 1.2f) {
				isAttacking = false;
				canNextAttack = false;
				attackIndex = 0;
			}
		}

		else if (pWeapon->GetType() == 3) {
			if (attackTimer > 0.4f && attackTimer < 1.0f) canNextAttack = true;
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
void PlayerAttack::CreateRangedHitBox() {
	VECTOR pos = VAdd(pPlayer->GetPosition(), VScale(pPlayer->GetForward(), 50.0f));
	VECTOR forward = pPlayer->GetForward();
	VECTOR vel = VScale(forward, 30.0f);
	float radius = 30.0f;
	float life = 0.5f;

	SphereHitBox* bullet = pSpherePool->BulletSpawn(pPlayer, pos, vel, radius, life, maxChainCount);
	if (!bullet) return;

	bullet->SetVelocity(vel);

	isAttacking = false;
}

void PlayerAttack::CreateHitBox(VECTOR _pos, float _radius) {
	SphereHitBox* sphereBox = pSpherePool->Spawn(pPlayer, _pos, VGet(0, 0, 0), _radius, 0.25f);
}

void PlayerAttack::CreateAttackHitbox(float _length, float _radius) {
	VECTOR forward = pPlayer->GetForward();
	float life = 0.2f;

	if (attackIndex < 3) {
		VECTOR startLocal = VScale(forward, 20.0f);           // ローカル座標
		VECTOR endLocal = VScale(forward, 20.0f + _length); // ローカル座標
		VECTOR spawnPos = VAdd(pPlayer->GetPosition(), VAdd(VScale(forward, 70.0f), VGet(0, 100, 0)));
		SphereHitBox* sphereBox = pSpherePool->Spawn(pPlayer, spawnPos, VGet(0, 0, 0), _radius, 0.25f);
		//CapsuleHitBox* capsuleBox = pCapsulePool->Spawn(pPlayer, startLocal, endLocal, VGet(0, 0, 0), _radius, 0.25f);
	}
	else {
		VECTOR spawnPos = VAdd(pPlayer->GetPosition(), VAdd(VScale(forward, 70.0f), VGet(0, 100, 0)));
		SphereHitBox* sphereBox = pSpherePool->Spawn(pPlayer, spawnPos, VGet(0, 0, 0), _radius, 0.25f);
	}
}
#pragma endregion
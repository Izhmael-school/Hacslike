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
	, hasGeneratedHitbox(false)
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
	, maxChainCount()
	, animName()
	, magnification(1){
	pSpherePool = &BulletPool::GetInstance();
	pCapsulePool = &CapsuleHitPool::GetInstance();
	Start();
}

/// <summary>
/// 初期化
/// </summary>
void PlayerAttack::Start() {
	EffectManager::GetInstance().Load("Res/Effect/CharegeAttackEfk.efk", "ChargeBlad", 30.0f);
	EffectManager::GetInstance().Load("Res/Effect/DA.efk", "DA", 10.0f);
	EffectManager::GetInstance().Load("Res/Effect/HIt.efk", "Hit", 20.0f);

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

	// 攻撃フラグが折れているなら、判定フラグも強制的に折る
	if (!isAttacking) {
		hasGeneratedHitbox = false;
	}

	pSpherePool->Update();
	pCapsulePool->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void PlayerAttack::Render() {}

/// <summary>
/// 攻撃の入力処理
/// </summary>
void PlayerAttack::AttackInput() {
#pragma region 入力処理
	bool isButtonDown = input->IsMouseDown(MOUSE_INPUT_LEFT) || input->IsButtonDown(XINPUT_GAMEPAD_RIGHT_SHOULDER);
	bool isButton = input->IsMouse(MOUSE_INPUT_LEFT) || input->IsButton(XINPUT_GAMEPAD_RIGHT_SHOULDER);
	bool isButtonUp = input->IsMouseUp(MOUSE_INPUT_LEFT) || input->IsButtonUp(XINPUT_GAMEPAD_RIGHT_SHOULDER);

	bool isChargeButtonDown = input->IsMouseDown(MOUSE_INPUT_RIGHT) || input->IsButtonDown(XINPUT_GAMEPAD_LEFT_SHOULDER);
	bool isChargeButton = input->IsMouse(MOUSE_INPUT_RIGHT) || input->IsButton(XINPUT_GAMEPAD_LEFT_SHOULDER);
	bool isChargeButtonUp = input->IsMouseUp(MOUSE_INPUT_RIGHT) || input->IsButtonUp(XINPUT_GAMEPAD_LEFT_SHOULDER);
#pragma endregion

#pragma region ため攻撃処理
	// 1. 溜め開始：右クリック(MOUSE_INPUT_RIGHT)が押された瞬間
	if (isChargeButtonDown && !isAttacking && !isCharging && !playerMovement->IsBlinking() && pWeapon->GetType() == 1) {
		isAttacking = true;
		isCharging = true;
		chargeTime = 0.0f;
		pPlayer->GetAnimator()->Play("GreatCharge1", 1.3f);
	}

	if (isCharging) {
		chargeTime += TimeManager::GetInstance().deltaTime;

		// 溜め中のアニメーション遷移
		if (chargeTime >= 0.65f) {
			pPlayer->GetAnimator()->Play("GreatCharge2", 1.3f);
		}

		// 2. 攻撃発動：ボタンを離すか、最大溜めに達したとき
		if (isChargeButtonUp || chargeTime >= maxChargeTime) {
			isCharging = false;
			isAttacking = true;
			attackTimer = 0.0f;
			hasGeneratedHitbox = false;

			// --- ここで attackIndex を溜め時間に応じて分岐 ---
			float ratio = chargeTime / maxChargeTime;

			if (ratio < 0.4f) {
				attackIndex = 4; // 短い溜め
			}
			else if (ratio < 0.8f) {
				attackIndex = 5; // 中間の溜め
			}
			else {
				attackIndex = 6; // 最大溜め
			}

			// アニメーション名は既存のまま
			pPlayer->GetAnimator()->Play("GreatCharge3", 1.3f);

			AudioManager::GetInstance().PlayOneShot("chargeAttack");
			EffectManager::GetInstance().Instantiate("ChargeBlad", pPlayer->GetPosition());
		}
	}
#pragma endregion

#pragma region コンボ処理
	if (isButtonDown && !attackButtonPressed && !playerMovement->IsBlinking()) {
		attackButtonPressed = true;

		// Sword
		if (pWeapon->GetType() == 0) {
			animName = "Atk" + std::to_string(attackIndex + 1);
			if (!isAttacking && playerMovement->IsDashState() && checkDashAttack) {
				isAttacking = true;
				attackIndex = 4;
				isDashAttack = true;
				attackTimer = 0.0f;
				hasGeneratedHitbox = false; // ★追加
				playerMovement->LockDirection();
				playerMovement->StopDash();
				pPlayer->GetAnimator()->Play("Atk3", pWeapon->GetAnimationSpeed(attackIndex - 1));
				AudioManager::GetInstance().PlayOneShot("Sword");
			}
			else if (!isAttacking) {
				isAttacking = true;
				AttackReset();
				AudioManager::GetInstance().PlayOneShot("Sword");
			}
			else if (canNextAttack && attackIndex < 3) {
				AttackReset();
				AudioManager::GetInstance().PlayOneShot("Sword");
			}
		}
		// GreatSword
		else if (pWeapon->GetType() == 1) {
			animName = "GreatAtk" + std::to_string(attackIndex + 1);
			if (!isAttacking) {
				isAttacking = true;
				AttackReset();
				AudioManager::GetInstance().PlayOneShot("GAtk1");
			}
			else if (canNextAttack && attackIndex < 3) {
				AttackReset();
				AudioManager::GetInstance().PlayOneShot("furi");
			}
		}
		// Axe
		else if (pWeapon->GetType() == 2) {
			animName = "AxeAtk" + std::to_string(attackIndex + 1);
			if (!isAttacking && playerMovement->IsDashState() && checkDashAttack) {
				isAttacking = true;
				isDashAttack = true;
				attackIndex = 4;
				attackTimer = 0.0f;
				hasGeneratedHitbox = false; // ★追加
				playerMovement->LockDirection();
				playerMovement->StopDash();
				pPlayer->GetAnimator()->Play("AxeAtk4", pWeapon->GetAnimationSpeed(attackIndex - 1));
				AudioManager::GetInstance().PlayOneShot("furi");
			}
			else if (!isAttacking) {
				isAttacking = true;
				AttackReset();
				AudioManager::GetInstance().PlayOneShot("furi");
			}
			else if (canNextAttack && attackIndex < 3) {
				AttackReset();
				AudioManager::GetInstance().PlayOneShot("furi");
			}
		}
		// Lance
		else if (pWeapon->GetType() == 4) {
			animName = "GreatAtk" + std::to_string(attackIndex + 1);
			if (!isAttacking && playerMovement->IsDashState() && checkDashAttack) {
				isAttacking = true;
				isDashAttack = true;
				attackIndex = 4;
				attackTimer = 0.0f;
				hasGeneratedHitbox = false; // ★追加
				playerMovement->LockDirection();
				playerMovement->StopDash();
				pPlayer->GetAnimator()->Play("LanceAtk1", pWeapon->GetAnimationSpeed(attackIndex - 1));
			}
			else if (!isAttacking) {
				isAttacking = true;
				AttackReset();
				AudioManager::GetInstance().PlayOneShot("GAtk1");
			}
			else if (canNextAttack && attackIndex < 3) {
				AttackReset();
				AudioManager::GetInstance().PlayOneShot("furi");
			}
		}
	}
#pragma endregion

	if (!isButtonDown) {
		attackButtonPressed = false;
	}

	if (isAttacking && !isCharging && !playerMovement->IsBlinking()) {
		attackTimer += TimeManager::GetInstance().deltaTime;

		if (isDashAttack && attackTimer < 0.4f) {
			playerMovement->ForceMove(20.0f);
		}

#pragma region 攻撃判定生成
		// Sword
		if (pWeapon->GetType() == 0) {
			if (attackTimer > 0.2f && attackTimer < 0.6f) canNextAttack = true;
			if (!hasGeneratedHitbox) { // ★追加
				if (attackIndex == 1 && attackTimer > 0.18f && attackTimer < 0.22f) {
					magnification = 1;
					HitBoxReset();
				}
				if (attackIndex == 2 && attackTimer > 0.22f && attackTimer < 0.28f) {
					magnification = 1.2f;
					HitBoxReset();
				}
				if (attackIndex == 3 && attackTimer > 0.25f && attackTimer < 0.33f) {
					magnification = 1.5;
					HitBoxReset();
				}
				if (attackIndex == 4 && attackTimer > 0.28f && attackTimer < 0.33f) {
					CreateAttackHitbox(pWeapon->GetColLength(2), pWeapon->GetColRadius(2));
					Effect* pEffe = EffectManager::GetInstance().Instantiate("DA", pPlayer->GetPosition());
					hasGeneratedHitbox = true;
				}
			}
			if (attackTimer > 0.8f) { isAttacking = false; isDashAttack = false; canNextAttack = false; attackIndex = 0; hasGeneratedHitbox = false; }
		}

		// GreatSword
		else if (pWeapon->GetType() == 1) {
			if (attackTimer > 0.6f && attackTimer < 1.0f) canNextAttack = true;
			if (!hasGeneratedHitbox) { // ★追加
				if (attackIndex == 1 && attackTimer > 0.25f && attackTimer < 0.3f) {
					HitBoxReset();
				}
				if (attackIndex == 2 && attackTimer > 0.35f && attackTimer < 0.45f) {
					HitBoxReset();
				}
				if (attackIndex == 3 && attackTimer > 1.3f && attackTimer < 2.2f) {
					HitBoxReset();
				}

				if (attackIndex == 4 && attackTimer > 0.9f && attackTimer < 1.9f) {
					CreateAttackHitbox(pWeapon->GetColLength(2), pWeapon->GetColRadius(2)); // 判定を大きくする例
					hasGeneratedHitbox = true;
				}
				// 溜め攻撃(中)の判定タイミング
				if (attackIndex == 5 && attackTimer > 0.9f && attackTimer < 1.9f) {
					CreateAttackHitbox(pWeapon->GetColLength(2) * 1.2f, pWeapon->GetColRadius(2) * 1.2f); // 判定を大きくする例
					hasGeneratedHitbox = true;
				}
				// 溜め攻撃(強)の判定タイミング
				if (attackIndex == 6 && attackTimer > 0.9f && attackTimer < 1.9f) {
					CreateAttackHitbox(pWeapon->GetColLength(2) * 1.8f, pWeapon->GetColRadius(2) * 1.8f); // 判定を大きくする例
					hasGeneratedHitbox = true;
				}
			}
			if (attackTimer > (attackIndex >= 3 ? 2.78f : 1.0f)) { isAttacking = false; isDashAttack = false; canNextAttack = false; attackIndex = 0; hasGeneratedHitbox = false; }
		}

		// Axe
		else if (pWeapon->GetType() == 2) {
			if (attackTimer > 0.6f && attackTimer < 1.0f) canNextAttack = true;
			if (!hasGeneratedHitbox) { // ★追加
				if (attackIndex == 1 && attackTimer > 0.25f && attackTimer < 0.30f) {
					HitBoxReset();
				}
				if (attackIndex == 2 && attackTimer > 0.35f && attackTimer < 0.40f) {
					HitBoxReset();
				}
				if (attackIndex == 3 && attackTimer > 0.35f && attackTimer < 0.50f) {
					HitBoxReset();
				}
				if (attackIndex == 4 && attackTimer > 0.36f && attackTimer < 0.48f) {
					CreateAttackHitbox(pWeapon->GetColLength(2), pWeapon->GetColRadius(2));
					hasGeneratedHitbox = true;
				}
			}
			if (attackTimer > 1.2f) { isAttacking = false; isDashAttack = false; canNextAttack = false; attackIndex = 0; hasGeneratedHitbox = false; }
		}

		// Lance
		else if (pWeapon->GetType() == 4) {
			if (attackTimer > 0.6f && attackTimer < 1.0f) canNextAttack = true;
			if (!hasGeneratedHitbox) { // ★追加
				if (attackIndex == 1 && attackTimer > 0.25f && attackTimer < 0.30f) {
					HitBoxReset();
				}
				if (attackIndex == 2 && attackTimer > 0.35f && attackTimer < 0.40f) {
					HitBoxReset();
				}
				if (attackIndex == 3 && attackTimer > 1.3f && attackTimer < 2.2f) {
					HitBoxReset();
				}
				if (attackIndex == 4 && attackTimer > 0.32f && attackTimer < 0.5f) {
					CreateAttackHitbox(pWeapon->GetColLength(2), pWeapon->GetColRadius(2));
					hasGeneratedHitbox = true;
				}
			}
			if (attackTimer > (attackIndex >= 3 ? 2.78f : 1.0f)) { isAttacking = false; isDashAttack = false; canNextAttack = false; attackIndex = 0; hasGeneratedHitbox = false; }
		}
		else if (pWeapon->GetType() == 3) {
			if (attackTimer > 0.4f && attackTimer < 1.0f) canNextAttack = true;
		}

		if (!pPlayer->GetAnimator()->IsPlaying()) {
			isAttacking = false;
			isDashAttack = false;
			canNextAttack = false;
			attackIndex = 0;
			hasGeneratedHitbox = false;
		}
	}
#pragma endregion

#pragma region ヒットボックス更新
	for (auto it = CapsuleHitboxes.begin(); it != CapsuleHitboxes.end();) {
		CapsuleHitBox* h = *it;
		h->Update();
		if (h->IsDead()) { delete h; it = CapsuleHitboxes.erase(it); }
		else ++it;
	}
	for (auto it = SphereHitboxes.begin(); it != SphereHitboxes.end();) {
		SphereHitBox* h = *it;
		h->Update();
		if (h->IsDead()) { delete h; it = SphereHitboxes.erase(it); }
		else ++it;
	}
#pragma endregion

#pragma region リセット
	if (isAttacking) {
		if (!pPlayer->GetAnimator()->IsPlaying()) {
			isAttacking = false;
			attackIndex = 0;
			canNextAttack = false;
			hasGeneratedHitbox = false;
		}
	}
#pragma endregion
}

#pragma region 攻撃判定処理
void PlayerAttack::CreateHitBox(VECTOR _pos, float _radius) {
	pSpherePool->Spawn(pPlayer, _pos, VGet(0, 0, 0), _radius, 0.25f);
}


void PlayerAttack::CreateAttackHitbox(float _length, float _radius) {
	VECTOR forward = pPlayer->GetForward();
	VECTOR spawnPos = VAdd(pPlayer->GetPosition(), VAdd(VScale(forward, 100.0f), VGet(0, 100, 0)));
	pSpherePool->Spawn(pPlayer, spawnPos, VGet(0, 0, 0), _radius, 0.25f, magnification);
	Effect* pEffe = EffectManager::GetInstance().Instantiate("Hit", spawnPos);
}
#pragma endregion

void PlayerAttack::AttackReset() {
	attackIndex++;
	attackTimer = 0.0f;
	canNextAttack = false;
	hasGeneratedHitbox = false;
	pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1));
}

void PlayerAttack::HitBoxReset() {
	CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
	hasGeneratedHitbox = true;
}

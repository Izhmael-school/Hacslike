#include "PlayerAttack.h"
#include "Player.h"
#include "../../Weapon/Weapon.h"
#include "../../../Manager/TimeManager.h"
#include "../../../Manager/EffectManager.h"
#include <vector>
#include "DxLib.h"

PlayerAttack::PlayerAttack(Player* _player, Weapon* _weapon, PlayerMovement* _playerMovement)
	: pPlayer(_player)
	, pWeapon(_weapon)
	, input(&InputManager::GetInstance())
	, isAttacking(false)
	, attackIndex(0)
	, attackTimer(0.0f)
	, canNextAttack(false)
	, attackButtonPressed(false)
	, hasGeneratedHitbox(false) // ★追加
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

void PlayerAttack::Start() {
	EffectManager::GetInstance().Load("Res/Effect/CharegeAttackEfk.efk", "ChargeBlad", 20.0f);
	EffectManager::GetInstance().Load("Res/Effect/DA.efk", "DA", 10.0f);
	EffectManager::GetInstance().Load("Res/Effect/HIt.efk", "Hit", 20.0f);

	AudioManager::GetInstance().Load("Res/Audio/SE/Player/ChargeAttackVoice.mp3", "chargeAttack", false);
	AudioManager::GetInstance().Load("Res/Audio/SE/Player/Sword.mp3", "Sword", false);
	AudioManager::GetInstance().Load("Res/Audio/SE/Player/GreatAttack1.mp3", "GAtk1", false);
	AudioManager::GetInstance().Load("Res/Audio/SE/Player/se_furi2.mp3", "furi", false);
}

void PlayerAttack::Update() {
	if (!pPlayer->GetIsDead())
		AttackInput();

	pSpherePool->Update();
	pCapsulePool->Update();
}

void PlayerAttack::Render() {}

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
	if (isChargeButtonDown && !isAttacking && !isCharging && !playerMovement->IsBlinking() && pWeapon->GetType() != 3) {
		isAttacking = true;
		if (pWeapon->GetType() == 1) {
			chargeTime = 0.0f;
			isCharging = true;
			pPlayer->GetAnimator()->Play("GreatCharge1", 1.3f);
		}
	}

	if (isCharging) {
		chargeTime += TimeManager::GetInstance().deltaTime;
		if (chargeTime >= 0.65f)
			pPlayer->GetAnimator()->Play("GreatCharge2", 1.3f);

		if (chargeTime >= maxChargeTime) {
			isChargeButtonUp = true;
		}
	}

	if (isChargeButtonUp && isCharging) {
		isCharging = false;
		isAttacking = true;
		attackTimer = 0.0f;
		hasGeneratedHitbox = false; // ★追加

		float ratio = chargeTime / maxChargeTime;
		float chargeRatio = (ratio < 1.0f) ? ratio : 1.0f;

		pPlayer->GetAnimator()->Play("GreatCharge3", 1.3f);
		attackIndex = 4;
		AudioManager::GetInstance().PlayOneShot("chargeAttack");
		Effect* pEffe = EffectManager::GetInstance().Instantiate("ChargeBlad", pPlayer->GetPosition());
	}
#pragma endregion

#pragma region コンボ処理
	if (isButtonDown && !attackButtonPressed && !playerMovement->IsBlinking()) {
		attackButtonPressed = true;

		// Sword
		if (pWeapon->GetType() == 0) {
			std::string animName = "Atk" + std::to_string(attackIndex + 1);
			if (!isAttacking && playerMovement->IsDashState() && checkDashAttack) {
				isAttacking = true;
				isDashAttack = true;
				attackIndex = 4;
				attackTimer = 0.0f;
				hasGeneratedHitbox = false; // ★追加
				playerMovement->LockDirection();
				playerMovement->StopDash();
				pPlayer->GetAnimator()->Play("Atk3", pWeapon->GetAnimationSpeed(attackIndex - 1));
				AudioManager::GetInstance().PlayOneShot("Sword");
			}
			else if (!isAttacking) {
				isAttacking = true;
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				hasGeneratedHitbox = false; // ★追加
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1));
				AudioManager::GetInstance().PlayOneShot("Sword");
			}
			else if (canNextAttack && attackIndex < 3) {
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				hasGeneratedHitbox = false; // ★追加
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1));
				AudioManager::GetInstance().PlayOneShot("Sword");
			}
		}
		// GreatSword
		else if (pWeapon->GetType() == 1) {
			std::string animName = "GreatAtk" + std::to_string(attackIndex + 1);
			if (!isAttacking) {
				isAttacking = true;
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				hasGeneratedHitbox = false; // ★追加
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1));
				AudioManager::GetInstance().PlayOneShot("GAtk1");
			}
			else if (canNextAttack && attackIndex < 3) {
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				hasGeneratedHitbox = false; // ★追加
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1));
				AudioManager::GetInstance().PlayOneShot("furi");
			}
		}
		// Axe
		else if (pWeapon->GetType() == 2) {
			std::string animName = "AxeAtk" + std::to_string(attackIndex + 1);
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
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				hasGeneratedHitbox = false; // ★追加
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1));
				AudioManager::GetInstance().PlayOneShot("furi");
			}
			else if (canNextAttack && attackIndex < 3) {
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				hasGeneratedHitbox = false; // ★追加
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1));
				AudioManager::GetInstance().PlayOneShot("furi");
			}
		}
		// Lance
		else if (pWeapon->GetType() == 4) {
			std::string animName = "GreatAtk" + std::to_string(attackIndex + 1);
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
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				hasGeneratedHitbox = false; // ★追加
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1));
				AudioManager::GetInstance().PlayOneShot("GAtk1");
			}
			else if (canNextAttack && attackIndex < 3) {
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				hasGeneratedHitbox = false; // ★追加
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1));
				AudioManager::GetInstance().PlayOneShot("furi");
			}
		}
	}
#pragma endregion

#pragma region 遠距離武器
	else if (isButtonUp && pWeapon->GetType() == 3) {
		if (!isAttacking) {
			isAttacking = true;
			attackIndex = 1;
			attackTimer = 0.0f;
			canNextAttack = false;
			hasGeneratedHitbox = false; // ★追加
			CreateRangedHitBox();
			pPlayer->Damage(addRadius >= 0.5f ? 5 : 1);
		}
		addRadius = 0.0f;
	}

	if (isButton && pWeapon->GetType() == 3) {
		addRadius += TimeManager::GetInstance().deltaTime;
	}
	else if (!isButtonDown) {
		attackButtonPressed = false;
	}
#pragma endregion

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
					CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
					hasGeneratedHitbox = true;
				}
				if (attackIndex == 2 && attackTimer > 0.22f && attackTimer < 0.28f) {
					CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
					hasGeneratedHitbox = true;
				}
				if (attackIndex == 3 && attackTimer > 0.25f && attackTimer < 0.33f) {
					CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
					hasGeneratedHitbox = true;
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
					CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
					hasGeneratedHitbox = true;
				}
				if (attackIndex == 2 && attackTimer > 0.35f && attackTimer < 0.45f) {
					CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
					hasGeneratedHitbox = true;
				}
				if (attackIndex == 3 && attackTimer > 1.3f && attackTimer < 2.2f) {
					CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
					hasGeneratedHitbox = true;
				}
				if (attackIndex == 4 && attackTimer > 0.9f && attackTimer < 1.9f) {
					CreateAttackHitbox(pWeapon->GetColLength(2), pWeapon->GetColRadius(2));
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
					CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
					hasGeneratedHitbox = true;
				}
				if (attackIndex == 2 && attackTimer > 0.35f && attackTimer < 0.40f) {
					CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
					hasGeneratedHitbox = true;
				}
				if (attackIndex == 3 && attackTimer > 0.35f && attackTimer < 0.50f) {
					CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
					hasGeneratedHitbox = true;
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
					CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
					hasGeneratedHitbox = true;
				}
				if (attackIndex == 2 && attackTimer > 0.35f && attackTimer < 0.40f) {
					CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
					hasGeneratedHitbox = true;
				}
				if (attackIndex == 3 && attackTimer > 0.35f && attackTimer < 0.50f) {
					CreateAttackHitbox(pWeapon->GetColLength(attackIndex - 1), pWeapon->GetColRadius(attackIndex - 1));
					hasGeneratedHitbox = true;
				}
				if (attackIndex == 4 && attackTimer > 0.32f && attackTimer < 0.35f) {
					CreateAttackHitbox(pWeapon->GetColLength(2), pWeapon->GetColRadius(2));
					hasGeneratedHitbox = true;
				}
			}
			if (attackTimer > 1.2f) { isAttacking = false; isDashAttack = false; canNextAttack = false; attackIndex = 0; hasGeneratedHitbox = false; }
		}
		else if (pWeapon->GetType() == 3) {
			if (attackTimer > 0.4f && attackTimer < 1.0f) canNextAttack = true;
		}

		if (!pPlayer->GetAnimator()->IsPlaying()) {
			isAttacking = false;
			isDashAttack = false;
			canNextAttack = false;
			attackIndex = 0;
			hasGeneratedHitbox = false; // ★追加
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

	if (isAttacking) {
		if (!pPlayer->GetAnimator()->IsPlaying()) {
			isAttacking = false;
			attackIndex = 0;
			canNextAttack = false;
			hasGeneratedHitbox = false; // ★追加
		}
	}
}

#pragma region 攻撃判定処理
void PlayerAttack::CreateRangedHitBox() {
	VECTOR forward = pPlayer->GetForward();
	VECTOR basePos = pPlayer->GetPosition();
	int pointCount = (addRadius < 0.5f) ? 1 : (addRadius < 1.5f ? 3 : 5);
	std::vector<VECTOR> points;
	float spreadAngle = 120.0f;
	float range = 500.0f;

	for (int i = 0; i < pointCount; ++i) {
		float angleRad = (i - (pointCount / 2)) * (spreadAngle / (pointCount > 1 ? (pointCount - 1) : 1)) * (3.14159265f / 180.0f);
		MATRIX rotMat = MGetRotY(angleRad);
		VECTOR rotDir = VTransform(forward, rotMat);
		VECTOR p = VAdd(basePos, VScale(rotDir, range));
		p.y += 40.0f;
		points.push_back(p);
	}

	for (int i = 0; i < (int)points.size(); ++i) {
		int r = GetRand((int)points.size() - 1);
		VECTOR temp = points[i];
		points[i] = points[r];
		points[r] = temp;
	}

	VECTOR startPos = VAdd(basePos, VScale(forward, 50.0f));
	VECTOR dir = VNorm(VSub(points[0], startPos));
	VECTOR vel = VScale(dir, 50.0f);
	SphereHitBox* bullet = pSpherePool->BulletSpawn(pPlayer, startPos, vel, 30.0f, 3.0f, maxChainCount);
	if (bullet) bullet->SetTargetList(points);
	isAttacking = false;
	addRadius = 0.0f;
}

void PlayerAttack::CreateHitBox(VECTOR _pos, float _radius) {
	pSpherePool->Spawn(pPlayer, _pos, VGet(0, 0, 0), _radius, 0.25f);
}

void PlayerAttack::CreateAttackHitbox(float _length, float _radius) {
	VECTOR forward = pPlayer->GetForward();
	VECTOR spawnPos = VAdd(pPlayer->GetPosition(), VAdd(VScale(forward, 100.0f), VGet(0, 100, 0)));
	pSpherePool->Spawn(pPlayer, spawnPos, VGet(0, 0, 0), _radius, 0.25f);
	Effect* pEffe = EffectManager::GetInstance().Instantiate("Hit", spawnPos);
}
#pragma endregion
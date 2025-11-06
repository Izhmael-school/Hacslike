#include "PlayerAttack.h"
#include "Player.h"
#include "../../../Manager/TimeManager.h"
#include "../../Weapon/Weapon.h"



PlayerAttack::PlayerAttack(Player* _player, Weapon* _weapon, PlayerMovement* _playerMovement)
	: pPlayer(_player)
	, pWeapon(_weapon)
	, input(&InputManager::GetInstance())
	, CapsuleHitboxes()
	, SphereHitboxes()
	, playerMovement(_playerMovement)
	, dashAttackDir(VZero)
	, isDashAttack(false) {
	Start();
}

void PlayerAttack::Start() {

}

void PlayerAttack::Update() {
	AttackInput();
}

/// <summary>
/// 攻撃入力・HitBox更新
/// </summary>
void PlayerAttack::AttackInput() {
	// ===== 攻撃入力 =====
	bool isButtonDown = input->IsMouseDown(MOUSE_INPUT_LEFT) || input->IsButtonDown(XINPUT_GAMEPAD_X);

	if (isButtonDown && !attackButtonPressed && !playerMovement->IsBlinking()) {
		// ボタンが押された瞬間だけ処理
		attackButtonPressed = true;
		if (pWeapon->GetType() == 0) {
			std::string animName = "Atk" + std::to_string(attackIndex + 1);
			if (!isAttacking && playerMovement->IsDashState()) {
				isAttacking = true;
				isDashAttack = true;
				attackIndex = 3;
				attackTimer = 0.0f;

				pPlayer->GetAnimator()->Play("Atk3", pWeapon->GetAnimationSpeed(attackIndex - 1));
			}
			else if (!isAttacking) {
				// --- 1段目攻撃 ---
				isAttacking = true;
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1)); // 攻撃1モーション
			}
			else if (canNextAttack && attackIndex < 3) {
				// --- コンボ入力 ---
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1)); // 攻撃2→3
			}
		}

		else if (pWeapon->GetType() == 1) {
			std::string animName = "GreatAtk" + std::to_string(attackIndex + 1);
			if (!isAttacking && playerMovement->IsDashState()) {
				isAttacking = true;
				isDashAttack = true;
				attackIndex = 4;
				attackTimer = 0.0f;

				pPlayer->GetAnimator()->Play("GreatAtk4", pWeapon->GetAnimationSpeed(attackIndex - 1));
			}
			else if (!isAttacking) {
				// --- 1段目攻撃 ---
				isAttacking = true;
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1)); // 攻撃1モーション
			}
			else if (canNextAttack && attackIndex < 3) {
				// --- コンボ入力 ---
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1));
			}
		}

		else if (pWeapon->GetType() == 2) {
			std::string animName = "AxeAtk" + std::to_string(attackIndex + 1);
			if (!isAttacking && playerMovement->IsDashState()) {
				isAttacking = true;
				isDashAttack = true;
				attackIndex = 4;
				attackTimer = 0.0f;

				pPlayer->GetAnimator()->Play("AxeAtk4", pWeapon->GetAnimationSpeed(attackIndex - 1));
			}
			else if (!isAttacking) {
				// --- 1段目攻撃 ---
				isAttacking = true;
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1)); // 攻撃1モーション
			}
			else if (canNextAttack && attackIndex < 3) {
				// --- コンボ入力 ---
				attackIndex++;
				attackTimer = 0.0f;
				canNextAttack = false;
				pPlayer->GetAnimator()->Play(animName.c_str(), pWeapon->GetAnimationSpeed(attackIndex - 1));
			}
		}
	}

	else if (!isButtonDown) {
		// ボタンを離したらフラグリセット
		attackButtonPressed = false;
	}

	// ===== 攻撃中のタイマー管理 =====
	if (isAttacking && !playerMovement->IsBlinking()) {
		attackTimer += TimeManager::GetInstance().deltaTime;


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
			if (attackIndex == 4 /*&& attackTimer > 0.18f && attackTimer < 0.22f*/)
				CreateAttackHitbox(pWeapon->GetColLength(2), pWeapon->GetColRadius(2)); // 周囲攻撃

			if (attackIndex == 3) {
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
}

///<summary>
///攻撃時の当たり判定発生処理
///</summary>
///<param name="length"></param>
///<param name="radius"></param>
void PlayerAttack::CreateAttackHitbox(float length, float radius) {
	//Unity座標系の前方
	VECTOR forward = VNorm(VGet(
		-sinf(Deg2Rad(pPlayer->GetRotation().y)),
		0.0f,
		-cosf(Deg2Rad(pPlayer->GetRotation().y))
	));

	VECTOR start, end;
	float life = 0.20f;

	if (attackIndex < 3) {
		// 1～2段目 前方カプセル
		start = VAdd(pPlayer->GetPosition(), VScale(forward, 20.0f));
		end = VAdd(start, VScale(forward, length));
		CapsuleHitBox* CapHit = new CapsuleHitBox(pPlayer, start, end, radius, life);
		CapHit->CreateCollider();
		CapsuleHitboxes.push_back(CapHit);
	}
	else {
		// 3段目 周囲攻撃（球状に近いカプセル）
		life = 0.25f;

		// SphereHitBox を生成
		float life = 0.25f;
		VECTOR offset = VAdd(VScale(forward, 70.0f), VGet(0.0f, 100.0f, 0.0f));

		SphereHitBox* Shit = new SphereHitBox(pPlayer, offset, radius, life);
		Shit->CreateCollider();
		SphereHitboxes.push_back(Shit);
	}

}
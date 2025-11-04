#pragma once
#include "../../../Component/Collider/SphereHitBox.h"
#include "../../../Component/Collider/CapsuleHitBox.h"
#include "PlayerMovement.h"

class Player;
class Weapon;
class InputManager;

class PlayerAttack {
private:
	Player* pPlayer;
	Weapon* pWeapon;

	InputManager* input;

	bool isAttacking = false;
	int attackIndex = 0;
	float attackTimer = 0.0f;
	bool canNextAttack = false;
	bool attackButtonPressed = false;

	std::vector<CapsuleHitBox*> CapsuleHitboxes;
	std::vector<SphereHitBox*> SphereHitboxes;

	PlayerMovement* playerMovement;

public:
	PlayerAttack(Player* _player, Weapon* _weapon);

	void Start();
	void Update();
	void AttackInput();
	void CreateAttackHitbox(float length, float radius);

	inline bool IsAttacking() const { return isAttacking; }

	void SetWeapon(Weapon* _weapon) {
		pWeapon = _weapon;
	}
};
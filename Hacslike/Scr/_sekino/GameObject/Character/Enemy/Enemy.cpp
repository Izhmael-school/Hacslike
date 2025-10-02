#include "Enemy.h"
#include "../../../Component/Collider.h"

Enemy::Enemy() 
{
	Start();
}

Enemy::~Enemy() {}

void Enemy::Start() {}

void Enemy::Update() {
	if (!isVisible) return;

	MV1SetMatrix(modelHandle, matrix);

	// 当たり判定の更新
	if (pCollider != nullptr) {
		pCollider->SetMatrix(matrix);
		pCollider->Update();
	}
}

void Enemy::Render() {
	if (!isVisible) return;

	// アニメーターの更新
	pAnimator->Update();

	MV1DrawModel(modelHandle);

	if (pCollider != nullptr)
		pCollider->Render();
}

void Enemy::IsDead() {
	if (hp > 0) return;

	hp = 0;
	isVisible = false;
}

void Enemy::Vision() {

	VECTOR pos = position;
	pos.y = 200;
	VECTOR startPos = pos;
	pos.z += 1000;
	VECTOR endPos = pos;

	MV1_COLL_RESULT_POLY hitPoly = MV1CollCheck_Line(modelHandle, -1, startPos, endPos);

	if (hitPoly.HitFlag == 1) {
		endPos = hitPoly.HitPosition;
	}

	DrawLine3D(startPos, endPos, GetColor(255, 255, 0));
}

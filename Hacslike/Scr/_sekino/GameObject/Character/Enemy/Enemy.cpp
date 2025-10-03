#include "Enemy.h"
#include "../../../Component/Collider.h"
#include "../../../Manager/TimeManager.h"
#include "../../../../Definition.h"

Enemy::Enemy()
	:rayAngle(45.0f)
	,rayCount(15)
	,rayLenght(1000)
	,raySpan(0.5f)
	,rayTime(raySpan)
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
	// レイの更新
	if (rayTime >= raySpan)
		rayTime = 0;
	else {
		rayTime += TimeManager::GetInstance()->deltaTime;
		return;
	}

	float startAngle = -rayAngle / 2;
	float angleStep = rayAngle / (rayCount - 1);

	for (int i = 0; i < rayCount; i++) {
		// 今の角度を求める
		float currentAngle = startAngle + (angleStep * i);
		// キャラが回転しても正面に出す
		float totalAngle = rotation.y + currentAngle;
		VECTOR start = position;
		float rad = Deg2Rad(totalAngle);
		VECTOR dir = VGet(sinf(rad), 0, cosf(rad));
		VECTOR end = VAdd(start, VScale(dir, rayLenght));

		MV1_COLL_RESULT_POLY ray = MV1CollCheck_Line(-1, -1, start, end);

		// ヒットした場合
		if (ray.HitFlag == 1) {
			end = ray.HitPosition;
		}

#if Debug
		DrawLine3D(start, end, yellow);
#endif
	}	
}

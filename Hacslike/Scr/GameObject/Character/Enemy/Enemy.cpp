#include "Enemy.h"
#include "../../../Manager/TimeManager.h"

Enemy::Enemy()
	:rayAngle(45.0f)
	, rayCount(15)
	, rayLenght(1000)
	, raySpan(0.5f)
	, rayTime(raySpan) 
	, moveSpeed(1) 
{
	Start();
}

Enemy::~Enemy() {}

void Enemy::Start() {
	// タグの設定
	SetTag("Enemy");
}

void Enemy::Update() {
	if (!isVisible) return;

	GameObject::Update();
	MV1SetMatrix(modelHandle, matrix);

	if (pAnimator != nullptr) {
		pAnimator->Update();
	}
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

	MV1SetMatrix(modelHandle, matrix);
	MV1DrawModel(modelHandle);

#if _DEBUG
	if (pCollider != nullptr)
		pCollider->Render();
#endif
}

void Enemy::IsDead() {
	if (hp > 0) return;

	hp = 0;

	pAnimator->Play("dead");
}

/// <summary>
/// MV1CollCheck_Lineを用いたRay
/// 結構重い
/// </summary>
bool Enemy::Vision_Ray() {
	// レイの更新
	if (rayTime >= raySpan)
		rayTime = 0;
	else {
		rayTime += TimeManager::GetInstance().deltaTime;
		return false;
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

		MV1_COLL_RESULT_POLY ray = MV1CollCheck_Line(PLAYER_MODEL_HANDLE, -1, start, end);

		// ヒットした場合
		if (ray.HitFlag == 1) {
			end = ray.HitPosition;
			return true;
		}

#if Debug
		DrawLine3D(start, end, yellow);
#endif
		return false;
	}
}

/// <summary>
/// MV1CollCheck_Sphereを用いたRay
/// 1体なら重くないが重なると重いかも
/// </summary>
/// <param name="r">半径</param>
bool Enemy::Vision_Circle(float r) {
	// レイの更新
	if (rayTime >= raySpan)
		rayTime = 0;
	else {
		rayTime += TimeManager::GetInstance().deltaTime;
		return false;
	}

	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(PLAYER_MODEL_HANDLE, -1, position, r);

	if (result.Dim == nullptr) return false;

	if (result.Dim->HitFlag == 1) {
		DrawString(0, 0, "視界に入っている", red);
		return true;
	}
	return false;
}
bool Enemy::Vision_Fan(VECTOR targetPos) {
	// レイの更新
	//if (rayTime >= raySpan)
	//	rayTime = 0;
	//else {
	//	rayTime += TimeManager::GetInstance()->deltaTime;
	//	return false;
	//}

	point.position = targetPos;

	fan.position = position;
	fan.directionDegree = rotation.y;
	fan.length = rayLenght;
	fan.rangeDegree = rayAngle;

	// 点と扇のベクトル
	VECTOR vecFanToPoint = {
		point.position.x - fan.position.x,
		0,
		point.position.z - fan.position.z,
	};

	// ベクトルの長さを算出
	float vecLength = sqrtf(pow(vecFanToPoint.x, 2) + pow(vecFanToPoint.z, 2));

	// ベクトルと扇の名側の比較
	if (fan.length < vecLength) return false; // 当たってない

	// 扇を２等分する線のベクトルを求める
	float dirRad = Deg2Rad(fan.directionDegree);
	VECTOR fanDir = VGet(cosf(dirRad), 0, sinf(dirRad));

	// 扇と点のベクトルを単位ベクトルにする
	VECTOR normalFanToPoint = {
		vecFanToPoint.x / vecLength,
		0,
		vecFanToPoint.z / vecLength
	};

	// 内積計算
	float dot = normalFanToPoint.x * fanDir.x + normalFanToPoint.z * fanDir.z;

	// 扇の範囲をcosにする
	float fanCos = cosf(Deg2Rad(fan.rangeDegree / 2));

	// 点が扇の範囲内にあるか比較
	if (fanCos < dot) return false; // 当たってない

	return true;
}
void Enemy::LookTarget(VECTOR targetPos, VECTOR axis) {
	VECTOR dir = VSub(targetPos, position);

	rotation.y = Rad2Deg(atan2f(dir.x, dir.z));

	return;

	float dot = Dot(axis, dir);
	float theta = acosf(dot);
	VECTOR cross = Cross(axis, targetPos);
	cross = Normalize(cross);
	theta = theta / 2;
}

/// <summary>
/// 追跡
/// </summary>
void Enemy::Tracking() {
	VECTOR targetPos = GetPlayer()->GetPosition();
	if (!Vision_Fan(targetPos)) return;

	LookTarget(targetPos);

	Move(targetPos);
}

void Enemy::Move(VECTOR targetPos) {
	pAnimator->Play("walk");
	VECTOR dir = VSub(targetPos, position);
	float d = TimeManager::GetInstance().deltaTime;
	VECTOR pos = VAdd(position, VGet(dir.x * d * moveSpeed, 0, dir.z * d * moveSpeed));
	SetPosition(pos);
}

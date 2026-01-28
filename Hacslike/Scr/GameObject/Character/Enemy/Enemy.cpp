#include <iostream>
#include "Enemy.h"
#include "../../../Manager/TimeManager.h"
#include "../../../Manager/CollisionManager.h"
#include "../../../Component/Collider/Collider.h"
#include "../../../CommonModule.h"
#include "../../../ExpansionMethod.h"
#include "../../../Manager/ItemDropManager.h"
#include "../../../GameSystem/GameSystem.h"
#include "../../Coin/Coin.h"

Enemy::Enemy()
	: moveSpeed(1)
	, isTouch(false)
	, rayAnswer(false)
	, atkTime(0)
	, atkSpan(4)
	, goalPos(VGet(-1, -1, -1))
	, nextWanderSpan(Random(1, 4))
	, nextWanderTime(nextWanderSpan)
	, currentRoot(position)
	, prevRoot(currentRoot) {}

Enemy::~Enemy() {
	attackAnimationList.clear();
	attackAnimationList.shrink_to_fit();
	area.DeleteObject();
}

void Enemy::Start() {
	// タグの設定
	SetTag("Enemy");
	Setup();
	// アニメーションモデルハンドルの設定
	pAnimator->SetModelHandle(modelHandle);
	// 大きすぎるため1/10
	SetScale(VGet(0.1f, 0.1f, 0.1f));
	// アニメーションイベントの設定
	if (isBoss)
		pAnimator->GetAnimation("dead")->SetEvent([this]() {EnemyManager::GetInstance().DeleteEnemy(this); }, pAnimator->GetTotalTime("dead"));
	else {
		SetAnimEvent("dead", [this]() {EffectManager::GetInstance().Instantiate("",VZero); }, pAnimator->GetTotalTime("dead"));
		SetAnimEvent("dead", [this]() {EnemyManager::GetInstance().UnuseEnemy(this); }, pAnimator->GetTotalTime("dead"));
	}

	SetAnimEvent("idle01", [this]() {SetAttacking(false); });
	// 攻撃中の移動制御

	if (pAnimator->GetAnimation("attack01") != nullptr) {
		SetAnimEvent("attack01", [this]() { SetAttacking(true); });
		SetAnimEvent("attack01", [this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack01"));
	}
	if (pAnimator->GetAnimation("attack02") != nullptr) {
		SetAnimEvent("attack02", [this]() { SetAttacking(true); });
		SetAnimEvent("attack02", [this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack02"));
	}
	if (pAnimator->GetAnimation("attack03") != nullptr) {
		SetAnimEvent("attack03", [this]() { SetAttacking(true); });
		SetAnimEvent("attack03", [this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack03"));
	}
}

void Enemy::Setup() {
	hp = maxHp;
	rotation.y = Random(0, 359);
	isTouch = false;
	rayAnswer = false;
	area.SetOwner(this);
	atking = false;
	isDead = false;
	goalPos = VMinus;
	nextWanderSpan = Random(1, 4);
	nextWanderTime = nextWanderSpan;
	atkSpan = 4;
	atkTime = 0;
	isBack = false;
	moveRoots.clear();
	currentRoot = position;
	prevRoot = currentRoot;
	SetVisible(true);
}

void Enemy::Teardown() {
	SetVisible(false);
	CollisionManager::GetInstance().UnRegister(pCollider);
}

void Enemy::Update() {
	if (!isVisible || !GameSystem::GetInstance()->IsPlayable()) return;
	// 座標の更新等
	GameObject::Update();
	MV1SetMatrix(modelHandle, matrix);

	// アニメーションの更新
	if (pAnimator != nullptr)
		pAnimator->Update();

	if (IsDead())
		return;

	if (atking) {
		area.Update();
		// 攻撃当たり判定の更新
		for (auto c : attackColliderList) {
			if (c->GetCollider() == nullptr) continue;

			c->Update();
		}
	}


	// 当たり判定の更新
	if (pCollider != nullptr) {
		pCollider->SetMatrix(matrix);
		pCollider->Update();
	}

	// 攻撃当たり判定の削除
	for (auto itr = attackColliderList.begin(); itr != attackColliderList.end(); ) {
		SphereHitBox* c = *itr;
		if (c->GetActive()) {
			++itr;
			continue;
		}

		CollisionManager::GetInstance().UnRegister(c->GetCollider());
		delete c;
		itr = attackColliderList.erase(itr); // eraseの戻り値を使って次の要素へ
	}

	if (isAttack()) return;

	// レイの更新
	WallDetectionVision_Fan(GetPlayer()->GetPosition());
	// 追跡行動
	Tracking();
	// 徘徊行動
	Wander();


	if (rayAnswer && !isTouch)
		pAnimator->Play("run");
	if (rayAnswer && isTouch)
		Attack();


	// 攻撃のリキャスト
	if (atkTime >= atkSpan)
		atkTime = atkSpan;
	else
		atkTime += TimeManager::GetInstance().deltaTime;
}

void Enemy::Render() {
	if (!isVisible) return;

	// 攻撃範囲の描画
	if (!IsDead()) {
		area.Render();
	}

#if _DEBUG
	// 攻撃当たり判定の更新
	for (auto c : attackColliderList) {
		if (c->GetCollider() == nullptr) continue;

		c->Render();
	}

	DrawVisionFanDebug();
#endif

	MV1SetMatrix(modelHandle, matrix);
	MV1DrawModel(modelHandle);
}



void Enemy::SetStatusData(int enemyID) {
	auto enemies = LoadJsonFile("Scr/Data/EnemyData.json");

	for (auto e : enemies) {

		if (e["id"] != enemyID) continue;

		maxHp = e["hp"];
		atk = e["atk"];
		def = e["def"];
		exp = e["exp"];
		name = e["name"];
		moveSpeed = e["spd"];
		mPath = e["mPath"];
		criticalHitRate = e["cRate"];
		criticalDamage = e["cDamageRate"];
		vision.rayAngle = e["rAngle"];
		vision.rayCount = e["rCount"];
		vision.rayLenght = e["rLenght"];
		break;
	}

	LoadAnimation();
}

void Enemy::LoadAnimation() {
	if (mPath == "") return;

	// アニメーションの読み込み
	pAnimator->Load(mPath + "idle01.mv1", "idle01", true, true);
	pAnimator->Load(mPath + "idle02.mv1", "idle02", true, true);
	pAnimator->Load(mPath + "attack01.mv1", "attack01", false, false);
	pAnimator->Load(mPath + "attack02.mv1", "attack02", false, false);
	pAnimator->Load(mPath + "attack03.mv1", "attack03", false, false);
	pAnimator->Load(mPath + "damage.mv1", "damage", false, false);
	pAnimator->Load(mPath + "dead.mv1", "dead", false, false);
	pAnimator->Load(mPath + "walk.mv1", "walk", true, true);
	pAnimator->Load(mPath + "run.mv1", "run", true, true);
	pAnimator->Load(mPath + "dropdown.mv1", "dropdown", false, false);
	pAnimator->Load(mPath + "situp.mv1", "situp", false, false);

	int animNum = pAnimator->GetAnimationIndex("attack01");
	if (animNum != -1)
		attackAnimationList.push_back(animNum);
	animNum = pAnimator->GetAnimationIndex("attack02");
	if (animNum != -1)
		attackAnimationList.push_back(animNum);
	animNum = pAnimator->GetAnimationIndex("attack03");
	if (animNum != -1)
		attackAnimationList.push_back(animNum);
}

void Enemy::DeadExecute() {
	if (IsDead()) return;

	hp = 0;
	isDead = true;

	// 経験値の増加
	Character::player->AddExp(exp);
	// アイテムのドロップ
	ItemDropManager* manager = &ItemDropManager::GetInstance();
	manager->TryDropItem(manager->GetItemDropRate(), position);
	Coin::GetInstance()->SpawnCoin(VGet(position.x, 5.0f, position.z));

	pAnimator->Play("dead");
}

/// <summary>
/// MV1CollCheck_Lineを用いたRay
/// 結構重い
/// </summary>
bool Enemy::Vision_Ray() {
	// レイの更新
	if (vision.rayTime >= vision.raySpan)
		vision.rayTime = 0;
	else {
		vision.rayTime += TimeManager::GetInstance().deltaTime;
		return false;
	}

	float startAngle = -vision.rayAngle / 2;
	float angleStep = vision.rayAngle / (vision.rayCount - 1);

	for (int i = 0; i < vision.rayCount; i++) {
		// 今の角度を求める
		float currentAngle = startAngle + (angleStep * i);
		// キャラが回転しても正面に出す
		float totalAngle = rotation.y + currentAngle;
		VECTOR start = position;
		float rad = Deg2Rad(totalAngle);
		VECTOR dir = VGet(sinf(rad), 0, cosf(rad));
		VECTOR end = VAdd(start, VScale(dir, vision.rayLenght));

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
	if (vision.rayTime >= vision.raySpan)
		vision.rayTime = 0;
	else {
		vision.rayTime += TimeManager::GetInstance().deltaTime;
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
	point.position = targetPos;

	fan.position = position;
	fan.directionDegree = rotation.y;
	fan.length = vision.rayLenght;
	fan.rangeDegree = vision.rayAngle;

	// 点と扇のベクトル
	VECTOR vecFanToPoint = {
		point.position.x - fan.position.x,
		0,
		point.position.z - fan.position.z,
	};

	// ベクトルの長さを算出
	float vecLength = sqrtf(pow(vecFanToPoint.x, 2) + pow(vecFanToPoint.z, 2));

	// ベクトルと扇の名側の比較
	if (fan.length < vecLength) return rayAnswer = false; // 当たってない

	// 扇を２等分する線のベクトルを求める
	float dirRad = Deg2Rad(fan.directionDegree);
	VECTOR fanDir = VGet(sinf(dirRad), 0, cosf(dirRad));

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
	if (fanCos > dot) return rayAnswer = false; // 当たってない

	return rayAnswer = true;
}

bool Enemy::WallDetectionVision_Fan(VECTOR targetPos) {
	// まず扇形の視界判定。視界外なら即 false
	if (!Vision_Fan(targetPos)) return false;

	// 座標をマップデータの座標に変換
	VECTOR targetMapPos = ChangeMapPos(targetPos);
	VECTOR myMapPos = ChangeMapPos(position);

	// 始点と終点の整数座標を取得
	int x0 = (int)myMapPos.x;
	int z0 = (int)myMapPos.z;
	int x1 = (int)targetMapPos.x;
	int z1 = (int)targetMapPos.z;

	// Bresenham の直線アルゴリズム用の差分と進行方向
	VECTOR dir = VGet(abs(x1 - x0), 0, abs(z1 - z0));
	VECTOR scale = VGet((x0 < x1) ? 1 : -1, 0, (z0 < z1) ? 1 : -1);
	int err = dir.x - dir.z;           // 誤差項

	StageManager* manager = &StageManager::GetInstance();

	while (true) {
		// 通過するタイルが壁なら視界は遮られている
		if ((ObjectType)manager->GetMapData(x0, z0) == Wall) {
			return rayAnswer = false;
		}

		// ターゲット位置に到達したら終了
		if (x0 == x1 && z0 == z1)
			break;

		// Bresenham の誤差計算で次のセルへ進む
		int e2 = 2 * err;
		if (e2 > -dir.z) { err -= dir.z; x0 += scale.x; }
		if (e2 < dir.x) { err += dir.x; z0 += scale.z; }
	}

	// 最後まで壁に当たらなければ true
	return rayAnswer = true;
}

void Enemy::Wander() {
	if (rayAnswer || isAttack()) return;

	// ゴールがなければ
	if (goalPos.x == -1 || goalPos.z == -1) {

		if (nextWanderTime < nextWanderSpan) {
			nextWanderTime += TimeManager::GetInstance().deltaTime;
			pAnimator->Play("idle01");
			return;
		}

		int x = std::round(position.x / CellSize);
		int z = std::round(position.z / CellSize);
		// 今いる部屋の番号を取得する
		int roomNum = StageManager::GetInstance().GetNowRoomNum(VGet(x, 0, z));
		// 部屋じゃなかったら
		if (roomNum == -1) {
			VECTOR g = moveRoots.back();
			moveRoots.pop_back();
			goalPos = g;
			isBack = true;
		}
		else {
			// 部屋の番号から部屋のサイズを取得する
			int rx = StageManager::GetInstance().GetRoomStatus(roomNum, RoomStatus::rx);
			int ry = StageManager::GetInstance().GetRoomStatus(roomNum, RoomStatus::ry);
			int rw = StageManager::GetInstance().GetRoomStatus(roomNum, RoomStatus::rw);
			int rh = StageManager::GetInstance().GetRoomStatus(roomNum, RoomStatus::rh);

			int moveX = Random(rx, rx + rw - 1);
			int moveY = Random(ry, ry + rh - 1);

			goalPos = VGet(moveX * CellSize, 0, moveY * CellSize);
		}
	}

	pAnimator->Play("walk");

	LookTarget(goalPos);
	Move(goalPos);

	//ゴール地点に近づいたら
	if (position.x >= goalPos.x - 50 && position.x < goalPos.x + 50 &&
		position.z >= goalPos.z - 50 && position.z < goalPos.z + 50) {

		VECTOR mapPos = ChangeMapPos(position);

		if (StageManager::GetInstance().GetMapData(mapPos.x, mapPos.z) == (int)Room) {
			nextWanderTime = 0;
			moveRoots.clear();
			isBack = false;
			goalPos = VGet(-1, -1, -1);
		}
		else {
			goalPos = VGet(-1, -1, -1);
		}
	}
}

void Enemy::SetAnimEvent(std::string animName, std::function<void()> func, float time) {
	auto anim = pAnimator->GetAnimation(animName);

	if (anim == nullptr || anim->animationHandle == -1) return;

	anim->SetEvent(func, time);
}

void Enemy::SetAnimEventForAttackCollider(std::string animName, float colliderspawnTime, float colliderLifeTime, float radius, float dis, float mag) {
	float speed = pAnimator->GetAnimSpeed(animName);
	SetAnimEvent(animName, [this, radius, speed, colliderspawnTime, dis, colliderLifeTime, mag]() {area.CreateArea(radius, colliderspawnTime, VAdd(AttackAreaPos(dis), position), speed,
		[this, radius, dis, colliderLifeTime, mag]() { attackColliderList.push_back(new SphereHitBox(this, AttackAreaPos(dis), radius, colliderLifeTime / GetFPS(), mag));
	EffectManager::GetInstance().Instantiate("Hit", VAdd(position, AttackAreaPos(dis)));
		}); });
}

void Enemy::SetAnimEventForAttackCollider(std::string animName, float colliderspawnTime, float colliderLifeTime, float radius, VECTOR pos, float dis, float mag) {
	float speed = pAnimator->GetAnimSpeed(animName);
	SetAnimEvent(animName, [this, radius, speed, colliderspawnTime, dis, colliderLifeTime, pos, mag]() {area.CreateArea(radius, colliderspawnTime, VAdd(AttackAreaPos(pos, dis), position), speed,
		[this, radius, dis, colliderLifeTime, mag]() { attackColliderList.push_back(new SphereHitBox(this, AttackAreaPos(dis), radius, colliderLifeTime / GetFPS(), mag));
	EffectManager::GetInstance().Instantiate("Hit", VAdd(position, AttackAreaPos(dis)));
		}); });
}

VECTOR Enemy::AttackAreaPos(float dis) {
	VECTOR dir = VGet(sinf(Deg2Rad(rotation.y)), 0, cosf(Deg2Rad(rotation.y)));
	VECTOR nDir = VNorm(dir);
	return VScale(nDir, dis);
}

VECTOR Enemy::AttackAreaPos(VECTOR pos, float dis) {
	float rad = Deg2Rad(rotation.y);

	VECTOR dir = VGet(pos.x * cosf(rad) + pos.z * sinf(rad), 0, -pos.x * sinf(rad) + pos.z * cosf(rad));
	VECTOR nDir = VNorm(dir);
	return VScale(nDir, dis);
}

void Enemy::LookTarget(VECTOR targetPos, VECTOR axis) {
	VECTOR dir = VSub(targetPos, position);
	dir = Normalize(dir);
	dir.y = 0;

	float targetYaw = atan2f(dir.x, dir.z);
	float currentYaw = Deg2Rad(rotation.y);

	float diff = targetYaw - currentYaw;

	while (diff > DX_PI_F) diff -= DX_TWO_PI_F;
	while (diff < -DX_PI_F) diff += DX_TWO_PI_F;

	// 
	float rotateSpeed = Deg2Rad(180.0f) * TimeManager::GetInstance().deltaTime;
	diff = std::clamp(diff, -rotateSpeed, rotateSpeed);

	rotation.y += Rad2Deg(diff);
}

/// <summary>
/// 追跡
/// </summary>
void Enemy::Tracking() {
	// レイに引っかかってない、プレイヤーに触れている、攻撃中は処理しない
	if (!rayAnswer || isTouch || isAttack()) return;
	VECTOR targetPos = GetPlayer()->GetPosition();

	LookTarget(targetPos);

	goalPos = VGet(-1, -1, -1);
	nextWanderTime = 0;

	if (pAnimator->Play("run") == -1)
		pAnimator->Play("walk");

	Move(targetPos);
}

void Enemy::Move(VECTOR targetPos) {
	VECTOR dir = VSub(targetPos, position);
	VECTOR nDir = VNorm(dir);
	float d = TimeManager::GetInstance().deltaTime;
	float move = moveSpeed * d;
	VECTOR pos = VAdd(position, VScale(nDir, move));
	// 壁の判定を確認して移動する
	VECTOR wPos = CheckWallToWallRubbing(pos);

	if (CompareVECTOR(wPos, VZero)) return;

	SetPosition(wPos);
	prevRoot = currentRoot;
	currentRoot = position;

	if (CompareVECTOR(prevRoot, VZero)) return;

	VECTOR mapPos = ChangeMapPos(position);

	if (StageManager::GetInstance().GetMapData(mapPos.x, mapPos.z) == (int)Room || isBack) return;
	moveRoots.push_back(prevRoot);
}

void Enemy::DrawVisionFanDebug() {
	float halfAngle = Deg2Rad(vision.rayAngle / 2.0f);
	float dirRad = Deg2Rad(rotation.y);

	// 中心方向ベクトル
	VECTOR dir = VGet(sinf(dirRad), 0, cosf(dirRad));

	// 左右の端方向ベクトル
	VECTOR leftDir = VGet(sinf(dirRad - halfAngle), 0, cosf(dirRad - halfAngle));
	VECTOR rightDir = VGet(sinf(dirRad + halfAngle), 0, cosf(dirRad + halfAngle));

	// 先端座標
	VECTOR centerEnd = VAdd(position, VScale(dir, vision.rayLenght));
	VECTOR leftEnd = VAdd(position, VScale(leftDir, vision.rayLenght));
	VECTOR rightEnd = VAdd(position, VScale(rightDir, vision.rayLenght));

	// 色
	unsigned int col = GetColor(255, 255, 0); // 黄色

	// 中心線
	DrawLine3D(position, centerEnd, col);

	// 左右の扇端
	DrawLine3D(position, leftEnd, col);
	DrawLine3D(position, rightEnd, col);

	// 扇の外周（円弧）を描画
	const int div = 20; // 円弧の分割数
	for (int i = 0; i < div; i++) {
		float a1 = (float)i / div;
		float a2 = (float)(i + 1) / div;

		float rad01 = dirRad - halfAngle + vision.rayAngle * a1 * DX_PI_F / 180.0f;
		float rad02 = dirRad - halfAngle + vision.rayAngle * a2 * DX_PI_F / 180.0f;

		VECTOR p1 = VAdd(position, VScale(VGet(sinf(rad01), 0, cosf(rad01)), vision.rayLenght));
		VECTOR p2 = VAdd(position, VScale(VGet(sinf(rad02), 0, cosf(rad02)), vision.rayLenght));

		DrawLine3D(p1, p2, col);
	}
}

void Enemy::Attack() {
	if (atkTime >= atkSpan) {
		atkTime = 0;
		pAnimator->Play(Random(attackAnimationList[0], attackAnimationList[attackAnimationList.size() - 1]));
	}
}

void Enemy::OnTriggerEnter(Collider* _pOther) {
	if (IsDead()) return;

	if (_pOther->GetGameObject()->CompareTag("Player")) {
		isTouch = true;

		if (!isAttack())
			pAnimator->Play("idle01");
	}
}

void Enemy::OnTriggerStay(Collider* _pOther) {
	GameObject* _pObj = _pOther->GetGameObject();
	if (_pObj->GetTag() != "Player" || atking || isDead) return;
	LookTarget(_pObj->GetPosition());
}

void Enemy::OnTriggerExit(Collider* _pOther) {
	if (_pOther->GetGameObject()->CompareTag("Player")) {
		isTouch = false;
	}
}
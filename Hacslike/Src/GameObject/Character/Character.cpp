#include "Character.h"
#include "../../UI/DamagePopup.h"


Character::Character(VECTOR _pos, std::string tag, int _Lv, int _Exp, int _speed)
	:GameObject(_pos, tag)
	, modelHandle(INVALID)
	, pAnimator(new Animator())
	, hp(0)
	, maxHp(0)
	, atk(0)
	, atking(false)
	, blocking(false)
	, speed(_speed)
	, criticalHitRate(1.0f)
	, criticalDamage(1.0f)
	, Lv(10)
	, exp(0)
	, prevPos(VGet(position.x, position.z, 0))
	, wallCheckPos(prevPos)
	, baseAttack(5)
	, proximityCorrection(1.0)
	, rangedCorrection(1.0) {}

Character::~Character() {
	delete pAnimator;
	MV1DeleteModel(modelHandle);
}

void Character::Damage(Character* attacker, int rawDamage) {
	if (IsDead()) return;

	int damage = rawDamage - def;
	if (damage < 1) damage = 1;

	// 攻撃者(attacker)が存在すれば、その瞬間のクリティカルフラグをもらう
	bool critFlag = false;
	if (attacker != nullptr) {
		critFlag = attacker->IsCritical();
	}

	// 第3引数にフラグを渡す新しいCreateを呼ぶ
	DamagePopup::Create(this, damage, critFlag);

	if (hp - damage > 0)
		hp -= damage;
	else {
		hp = 0;
		DeadExecute();
	}
}

void Character::CheckWall() {
	if (StageManager::GetInstance().GetMapData((int)((int)(position.x + CellCorrection) / CellSize), (int)((int)(position.z + CellCorrection) / CellSize)) != ObjectType::Wall) {
		prevPos = VGet(position.x, 0, position.z);
		if (wallCheckPos.x != (int)prevPos.x || wallCheckPos.y != (int)prevPos.z)
			wallCheckPos = prevPos;
		return;
	}
	SetPosition(prevPos.x, 0, prevPos.z);

	prevPos = VGet(position.x, 0, position.z);
	if (wallCheckPos.x != (int)prevPos.x || wallCheckPos.y != (int)prevPos.y)
		wallCheckPos = prevPos;

}

VECTOR Character::CheckWallToWallRubbing(VECTOR dir) {
	const float correctionAmount = 50.0f; // 押し戻し距離
	const float smoothFactor = 0.2f;     // 線形補間係数
	VECTOR correctedPosition = position;

	// 現在のマスと移動先のマスを算出
	int currentX = (int)((position.x + CellCorrection) / CellSize);
	int currentZ = (int)((position.z + CellCorrection) / CellSize);
	int targetX = (int)((dir.x + CellCorrection) / CellSize);
	int targetZ = (int)((dir.z + CellCorrection) / CellSize);

	// 進行方向のマップデータを取得
	int nextData = StageManager::GetInstance().GetMapData(targetX, targetZ);
	if (nextData != ObjectType::Wall) {
		// 壁がないならそのまま移動
		return dir;
	}

	// --- 個別にスライド判定を行う ---
	bool canSlideX = false, canSlideZ = false;
	VECTOR slideX = VGet(dir.x, 0, position.z); // X方向のみにスライド
	VECTOR slideZ = VGet(position.x, 0, dir.z); // Z方向のみにスライド

	// X方向の判定
	int slideTargetX = (int)((slideX.x + CellCorrection) / CellSize);
	if (StageManager::GetInstance().GetMapData(slideTargetX, currentZ) != ObjectType::Wall) {
		canSlideX = true;
	}

	// Z方向の判定
	int slideTargetZ = (int)((slideZ.z + CellCorrection) / CellSize);
	if (StageManager::GetInstance().GetMapData(currentX, slideTargetZ) != ObjectType::Wall) {
		canSlideZ = true;
	}

	// --- 判定に基づいて補正処理を実施 ---
	if (canSlideX && !canSlideZ) {
		correctedPosition = slideX; // X方向へ移動
	}
	else if (!canSlideX && canSlideZ) {
		correctedPosition = slideZ; // Z方向へ移動
	}
	else if (!canSlideX && !canSlideZ) {
		// 両方向が壁の場合、滑らかに押し戻し
		float adjustX = (dir.x > position.x) ? -correctionAmount : correctionAmount;
		float adjustZ = (dir.z > position.z) ? -correctionAmount : correctionAmount;
		correctedPosition.x += adjustX * smoothFactor;
		correctedPosition.z += adjustZ * smoothFactor;
	}
	else {
		// どちらも動ける場合はそのままスライド（円滑な処理）
		correctedPosition = VGet(dir.x, 0, dir.z);
	}

	// 線形補間で結果を滑らかに適用
	VECTOR finalPos;
	finalPos.x = position.x + (correctedPosition.x - position.x) * 1;
	finalPos.y = position.y;  // 高さには補正を加えない
	finalPos.z = position.z + (correctedPosition.z - position.z) * 1;

	return finalPos;
}


Character* Character::player;

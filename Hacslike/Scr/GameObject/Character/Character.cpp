#include "Character.h"


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
	, Lv(1)
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

void Character::Damage(int rawDamage) {
	int damage = rawDamage - def;

	if (damage <= 0) damage = 1;

	hp -= damage;

	IsDead();
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
	// 壁に埋まりすぎないように補正を掛ける
	float correntionX, correntionZ = 0;
	if (dir.x > position.x) correntionX = 50;
	else correntionX = -50;
	if (dir.z > position.z) correntionZ = 50;
	else correntionZ = -50;

	// 現在いるマス目を取得
	int x = (int)((int)(dir.x + CellCorrection + correntionX) / CellSize);
	int z = (int)((int)(dir.z + CellCorrection + correntionZ) / CellSize);

	int d = StageManager::GetInstance().GetMapData(x, z);

	if (d == -1) return VZero;

	// 進む先が壁じゃなかったら
	if (d != ObjectType::Wall) {
		// 値をそのまま返す
		return dir;
	}

	// x軸にスライドするか
	VECTOR slideX = VGet(dir.x, 0, position.z);
	x = (int)((int)(slideX.x + CellCorrection) / CellSize);
	z = (int)((int)(slideX.z + CellCorrection) / CellSize);
	bool canSlideX = (StageManager::GetInstance().GetMapData(x, z) != ObjectType::Wall);

	// z軸にスライドするか
	VECTOR slideZ = VGet(position.x, 0, dir.z);
	x = (int)((int)(slideZ.x + CellCorrection) / CellSize);
	z = (int)((int)(slideZ.z + CellCorrection) / CellSize);
	bool canSlideZ = (StageManager::GetInstance().GetMapData(x, z) != ObjectType::Wall);

	return VGet(canSlideX ? dir.x : position.x, 0, canSlideZ ? dir.z : position.z);
}


Character* Character::player;

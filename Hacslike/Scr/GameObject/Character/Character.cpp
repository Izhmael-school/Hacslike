#include "Character.h"


Character::Character(VECTOR _pos, std::string tag, int _Lv, int _Exp, int _speed)
	:GameObject(_pos,tag)
	,modelHandle(INVALID)
	, pAnimator(new Animator())
	,hp(0)
	,maxHp(0)
	,atk(0)
	,atking(false)
	,blocking(false)
	,speed(_speed)
	,criticalHitRate(1.0f)
	,criticalDamage(1.0f)
	,Lv(1)
	,Exp(0)
	,prevPos(VGet(position.x,position.z,0))
	,wallCheckPos(prevPos)
{}

Character::~Character() {
	delete pAnimator;
	MV1DeleteModel(modelHandle);
}

void Character::CheckWall() {
<<<<<<< HEAD
<<<<<<< HEAD
	if (StageManager::GetInstance().GetMapData((int)((int)(position.x + CellCorrection) / CellSize), (int)((int)(position.z + CellCorrection) / CellSize)) != ObjectType::Wall) {
=======
<<<<<<< Updated upstream
	if (StageManager::GetInstance()->GetMapData((int)((int)(position.x + CellCorrection) / CellSize), (int)((int)(position.z + CellCorrection) / CellSize)) != ObjectType::Wall) {
=======
	if (StageManager::GetInstance().GetMapData((int)((int)(position.x + CellCorrection) / CellSize),
											   (int)((int)(position.z + CellCorrection) / CellSize)) != ObjectType::Wall) {
>>>>>>> Stashed changes
>>>>>>> Sekino
=======
	if (StageManager::GetInstance()->GetMapData((int)((int)(position.x + CellCorrection) / CellSize), (int)((int)(position.z + CellCorrection) / CellSize)) != ObjectType::Wall) {
>>>>>>> parent of 536f9b8 (Add)
		prevPos = VGet(position.x,0, position.z);
		if (wallCheckPos.x != (int)prevPos.x || wallCheckPos.y != (int)prevPos.z)
			wallCheckPos = prevPos;
		return;
	}
	SetPosition(prevPos.x, 0, prevPos.z);

	prevPos = VGet(position.x, 0, position.z);
	if(wallCheckPos.x != (int)prevPos.x || wallCheckPos.y != (int)prevPos.y)
		wallCheckPos = prevPos;

}
	

Character* Character::player;

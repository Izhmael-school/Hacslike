#include "Character.h"


Character::Character(VECTOR _pos , std::string tag, int _Lv, int _Exp) 
	:GameObject(_pos,tag)
	,modelHandle(INVALID)
	, pAnimator(new Animator())
	,hp(100)
	,atk(0)
	,atking(false)
	,blocking(false)
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
	if (StageManager::GetInstance()->GetMapData((int)((int)(position.x + CellCorrection) / CellSize), (int)((int)(position.z + CellCorrection) / CellSize)) != ObjectType::Wall) {
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

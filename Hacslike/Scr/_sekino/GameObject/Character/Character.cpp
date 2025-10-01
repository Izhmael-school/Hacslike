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
{}

Character::~Character() {
	delete pAnimator;
	MV1DeleteModel(modelHandle);
}



#include "GameObject.h"
#include "../Component/Collider/Collider.h"

// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
GameObject::GameObject(VECTOR _pos,std::string _tag) 
	:isVisible(true)
	,position(_pos)
	,rotation(VZero)
	,scale(VOne)
	,matrix(MGetIdent())
	,tag(_tag)
	,layer(-1)
	,pCollider(nullptr)
{
}

// ƒfƒXƒgƒ‰ƒNƒ^
GameObject::~GameObject() {}

void GameObject::Update() {
	// ”ñ•\Ž¦‚È‚ç•\Ž¦‚µ‚È‚¢
	if (!isVisible)
		return;

	// À•WA‰ñ“]AŠgk‚©‚çs—ñ‚ð‹‚ß‚é
	MATRIX mRotX = MGetRotX(Deg2Rad(rotation.x)); // XŽ²‰ñ“]s—ñ
	MATRIX mRotY = MGetRotY(Deg2Rad(rotation.y)); // YŽ²‰ñ“]s—ñ
	MATRIX mRotZ = MGetRotZ(Deg2Rad(rotation.z)); // ZŽ²‰ñ“]s—ñ

	// X->Y->Z ‚Ì‡‚Å‰ñ“]s—ñ‚ðì¬‚·‚é
	MATRIX mRotXYZ = MMult(MMult(mRotX, mRotY), mRotZ);

	// Šgks—ñ‚ðŽæ“¾‚·‚é
	MATRIX mScale = MGetScale(scale);

	// •½sˆÚ“®s—ñ‚ðŽæ“¾‚·‚é
	MATRIX mTranslate = MGetTranslate(position);

	// s—ñ‚ÌæŽZ‚Í‡¬
	// ‰ñ“]s—ñ ¨ Šgks—ñ ¨ •½sˆÚ“®s—ñ‚Ì‡‚É‚©‚¯‡‚í‚¹‚é
	matrix = MMult(MMult(mRotXYZ, mScale), mTranslate);

	// “–‚½‚è”»’è‚ª”º‚¤ê‡ matrix ‚Ìî•ñ‚ð—^‚¦‚é
	if (pCollider != nullptr) {
		pCollider->SetMatrix(matrix);
	}
}

bool GameObject::CompareTag(std::string _tagName) {
	if (tag == _tagName) return true;
	return false;
}

void GameObject::OnTriggerEnter(Collider* _pOther) {

}

void GameObject::OnTriggerStay(Collider* _pOther) {

}

void GameObject::OnTriggerExit(Collider* _pOther) {

}

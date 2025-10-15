#include "StageCell.h"

StageCell::StageCell(int _modelHandle, ObjectType _type, VECTOR position)
	:modelHandle(_modelHandle)
	,type(_type)
	, GameObject(position,"Stage")
	,isTouch(false)
{
	Start();
}

StageCell::~StageCell() {
	MV1DeleteModel(modelHandle);
}

void StageCell::Start() {
	if (type != Stair) return;

	pCollider = new SphereCollider(this,VZero,100);
	CollisionManager::GetInstance()->Register(pCollider);
}

void StageCell::Update() {
	GameObject::Update();

	MV1SetMatrix(modelHandle, matrix);

	if (isTouch && InputManager::GetInstance()->IsButtonDown(XINPUT_BUTTON_X)) {
		StageManager::GetInstance()->GenerateStage();
		isTouch = false;
		return;
	}

	

	if (pCollider != nullptr)
		pCollider->Update();
}

void StageCell::Render() {
	if (modelHandle == -1) return;

	MV1DrawModel(modelHandle);

	if (pCollider != nullptr)
		pCollider->Render();
}

void StageCell::OnTriggerEnter(Collider* _pOther) {
	if (_pOther->GetGameObject()->GetTag() != "Player") return;

	isTouch = true;
}

void StageCell::OnTriggerExit(Collider* _pOther) {
	if (_pOther->GetGameObject()->GetTag() != "Player") return;

	isTouch = false;
}

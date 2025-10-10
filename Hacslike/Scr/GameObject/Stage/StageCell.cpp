#include "StageCell.h"

StageCell::StageCell(int _modelHandle, ObjectType _type, VECTOR position)
	:modelHandle(_modelHandle)
	,type(_type)
	, GameObject(position,"Stage")
{
	Start();
}

StageCell::~StageCell() {
	MV1DeleteModel(modelHandle);
}

void StageCell::Start() {
	if (type != Stair) return;

	pCollider = new SphereCollider(this,VZero,100);
}

void StageCell::Update() {
	GameObject::Update();

	MV1SetMatrix(modelHandle, matrix);

	if (pCollider != nullptr)
		pCollider->Update();
}

void StageCell::Render() {
	if (modelHandle == -1) return;

	MV1DrawModel(modelHandle);

	if (pCollider != nullptr)
		pCollider->Render();
}

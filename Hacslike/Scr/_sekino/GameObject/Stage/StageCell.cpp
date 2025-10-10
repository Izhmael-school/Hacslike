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

	pCollider = nullptr;
}

void StageCell::Update() {
	GameObject::Update();

	MV1SetMatrix(modelHandle, matrix);
}

void StageCell::Render() {
	if (modelHandle == -1) return;

	MV1DrawModel(modelHandle);
}

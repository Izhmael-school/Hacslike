#include "StageCell.h"
#include "../../Manager/FadeManager.h"
#include "../../Manager/StageManager.h"
#include "../../Manager/AudioManager.h"
#include "../../Component/Collider/Collider.h"

StageCell::StageCell(int _modelHandle, ObjectType _type, VECTOR position)
	:modelHandle(_modelHandle)
	, type(_type)
	, GameObject(position, "Stage")
	, isTouch(false)
	, dataPos() {
	Start();
}

StageCell::~StageCell() {
	MV1DeleteModel(modelHandle);
}

void StageCell::Start() {
	if (type != Stair) return;

	pCollider = new SphereCollider(this, VZero, 100);
}

void StageCell::Update() {
	GameObject::Update();

	MV1SetMatrix(modelHandle, matrix);

	if (isTouch && (InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_B) || InputManager::GetInstance().IsKeyDown(KEY_INPUT_X))) {

		if (FadeManager::GetInstance().GetFadeState() != FadeState::FadeEnd) return;

		AudioManager::GetInstance().PlayOneShot("FloorDawn");
		StageManager::GetInstance().Generate();
		isTouch = false;
		return;
	}



	if (pCollider != nullptr)
		pCollider->Update();
}

void StageCell::Render() {
	if (modelHandle == -1) return;

	if (isTouch) {
		int StartX = (WINDOW_WIDTH / 2) - 200;
		int StartY = (WINDOW_HEIGHT)-200;
		int GoalX = (WINDOW_WIDTH / 2) + 200;
		int GoalY = (WINDOW_HEIGHT)-150;
		int textX = StartX + 80;
		int textY = StartY + 17;

		DrawBox(StartX, StartY, GoalX, GoalY, gray, TRUE);
		DrawBox(StartX + 2, StartY + 2, GoalX - 2, GoalY - 2, white, FALSE);
		DrawFormatStringToHandle(textX + 10, textY, black,MainFont, "ƒL[/  ƒ{ƒ^ƒ“:ŠK‘wˆÚ“®‚·‚é");
		DrawFormatStringToHandle(textX, textY, white,MainFont, "X");
		DrawFormatStringToHandle(textX + 53, textY, white,MainFont, "B");
	}

	MV1DrawModel(modelHandle);

#if _DEBUG
	if (pCollider != nullptr)
		pCollider->Render();
#endif
}

void StageCell::OnTriggerEnter(Collider* _pOther) {
	if (_pOther->GetGameObject()->GetTag() != "Player") return;

	isTouch = true;
}

void StageCell::OnTriggerExit(Collider* _pOther) {
	if (_pOther->GetGameObject()->GetTag() != "Player") return;

	isTouch = false;
}

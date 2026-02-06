#include "StageObject.h"
#include "../../Component/Collider/Collider.h"
#include "../../Manager/CollisionManager.h"
#include "../../Manager/InputManager.h"
#include "../../CommonModule.h"
#include "../../GameSystem/GameSystem.h"

StageObject::StageObject(int _mHandle)
	:modelHandle(_mHandle)
	, isHitPlayer(false)
	, menuOpenState(Close) {
	Start();
}

StageObject::~StageObject() {
	MV1DeleteModel(modelHandle);

	if (pCollider) {
		CollisionManager::GetInstance().UnRegister(pCollider);
		delete pCollider;
	}
}

void StageObject::Start() {
	pCollider = new SphereCollider(this, position, CellSize / 2);
}

void StageObject::Update() {
	if (!isVisible) return;

	GameObject::Update();

	switch (menuOpenState) {
	case Open:
		if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_ESCAPE) || InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_A))
			CloseExecute();
		break;
	case Close:
		if (InputManager::GetInstance().IsKeyDown(KEY_INPUT_F) || InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_B))
			OpenExecute();
		break;
	}

	if (pCollider)
		pCollider->Update();
}

void StageObject::Render() {
	if (!isVisible) return;

	MV1DrawModel(modelHandle);

#if _DEBUG
	if (pCollider)
		pCollider->Render();
#endif
}

void StageObject::OnTriggerEnter(Collider* _pOther) {
	if (_pOther->GetCharacter()->GetTag() != "Player") return;

	isHitPlayer = true;
}

void StageObject::OnTriggerExit(Collider* _pOther)
{
}

void StageObject::OpenExecute() {
	GameSystem::GetInstance()->SetGameStatus(GameStatus::Stop); // ƒQ[ƒ€’âŽ~
	menuOpenState = Open;
	Setup();
}


void StageObject::CloseExecute() {
	GameSystem::GetInstance()->SetGameStatus(GameStatus::Playing); // ƒQ[ƒ€ÄŠJ
	menuOpenState = Close;
	Teardown();
}

void StageObject::Setup() {}

void StageObject::Teardown() {}

//void StageObject::OnTriggerEnter(Collider* _pOther) {
//	if (_pOther->GetCharacter()->GetTag() != "Player") return;
//
//	isHitPlayer = false;
//}
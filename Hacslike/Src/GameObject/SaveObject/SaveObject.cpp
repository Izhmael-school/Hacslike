#include "SaveObject.h"
#include"../../Manager/CollisionManager.h"
#include"../../Manager/InputManager.h"
#include"../../Component/Collider/Collider.h"
#include "../Character/Player/Player.h"

SaveObject* SaveObject::pInstance = nullptr;

SaveObject::SaveObject(VECTOR _pos)
	:pCol(nullptr)
	,modelHandle(INVALID)
	,hitObject(false)
	,openSaveMenu(false){
	position = _pos;
	Start();
}

SaveObject::~SaveObject()
{
	MV1DeleteModel(modelHandle);
	if (pCol) {
		CollisionManager::GetInstance().UnRegister(pCol);
		delete pCol;
	}
}

void SaveObject::CreateInstance()
{
	if (pInstance == nullptr) {
		pInstance = new SaveObject(VZero);
	}
}

SaveObject* SaveObject::GetInstance()
{
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

void SaveObject::DestroyInstance()
{
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

void SaveObject::Start()
{
	modelHandle = MV1LoadModel("Res/Model/SaveObject/SaveObject.mv1");
	
	if(!pCol){
		pCol = new CapsuleCollider(this, VZero, VGet(0, 20, 0), 80.0f);
		//CollisionManager::GetInstance().Register(pCol);
	}
	SetScale(1, 1, 1);
}

void SaveObject::Update()
{
	// コライダー更新
	if (pCol != nullptr)
	{
		pCol->SetMatrix(matrix);
		pCol->Update();
	}



	auto& input = InputManager::GetInstance();
	if(hitObject){
		if (input.IsKeyDown(KEY_INPUT_F)|| input.IsButtonDown(XINPUT_GAMEPAD_B)) {
			openSaveMenu = true;
			Player::GetInstance()->SetIsOpenMenu(true);
		}
		if(openSaveMenu){
			if (input.IsKeyDown(KEY_INPUT_ESCAPE) || input.IsButtonDown(XINPUT_GAMEPAD_A)) {
				openSaveMenu = false;
				Player::GetInstance()->SetIsOpenMenu(false);

			}
		}
		
	}
	else { openSaveMenu = false; }
	
	if(openSaveMenu){
		if (!pSaveMenu) {
			// menuIndex によって Save/Load を切り替えたいならここで判定
			pSaveMenu = new MenuSaveLoad(MenuSaveLoad::SaveMode);
			pSaveMenu->Open();
		}
		// メニューの入力処理等を行う
		pSaveMenu->Update();
	}
	else {
		// メニューを閉じたら破棄して状態をリセット
		if (pSaveMenu) {
			delete pSaveMenu;
			pSaveMenu = nullptr;
		}
	}
	GameObject::Update();
}

void SaveObject::Render()
{
	if (isVisible) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		MV1SetMatrix(modelHandle, matrix);
		MV1DrawModel(modelHandle);
	}
#if _DEBUG
	if (pCol != nullptr) {
		pCol->Render();
	}
#endif
	int StartX = (WINDOW_WIDTH / 2) - 200;
	int StartY = (WINDOW_HEIGHT)-200;
	int GoalX = (WINDOW_WIDTH / 2) + 200;
	int GoalY = (WINDOW_HEIGHT)-150;
	int textX = StartX + 80;
	int textY = StartY + 17;
	if (hitObject) {
		// Zバッファ（奥行き）をチェックせずに描画する
		SetUseZBuffer3D(FALSE);
		// Zバッファに書き込みもしない（後の描画に影響を与えない）
		SetWriteZBuffer3D(FALSE);

		DrawBox(StartX, StartY, GoalX, GoalY, gray, TRUE);
		DrawBox(StartX + 2, StartY + 2, GoalX - 2, GoalY - 2, white, FALSE);
		DrawFormatStringToHandle(textX + 10, textY, black,MainFont, "キー/  ボタン:セーブ/ロード");
		DrawFormatStringToHandle(textX, textY, white,MainFont, "F");
		DrawFormatStringToHandle(textX + 53, textY, white,MainFont, "B");

		// Zバッファ（奥行き）をチェックせずに描画する
		SetUseZBuffer3D(TRUE);
		// Zバッファに書き込みもしない（後の描画に影響を与えない）
		SetWriteZBuffer3D(TRUE);
	}
	
		
	


	if(openSaveMenu && hitObject){
		pSaveMenu->Render();
	}
}

void SaveObject::OnTriggerStay(Collider* _pCol)
{
	if (_pCol->GetGameObject()->GetTag() == "Player")
		hitObject = true;
}

void SaveObject::OnTriggerExit(Collider* _pCol)
{
	if (_pCol->GetGameObject()->GetTag() == "Player") hitObject = false;
}

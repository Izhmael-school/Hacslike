#include "TitleReturner.h"
#include "../Character/Player/Player.h"
#include "../../Manager/CollisionManager.h"
#include "../../Manager/InputManager.h"
#include "../../GameSystem/GameSystem.h"
#include "../../Manager/StageManager.h"
#include"../../Manager/TimeManager.h"

TitleReturner* TitleReturner::pInstance = nullptr;

void TitleReturner::CreateInstance() {
	if (pInstance == nullptr) {
		pInstance = new TitleReturner();
	}
}

TitleReturner* TitleReturner::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

void TitleReturner::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

TitleReturner::TitleReturner() : GameObject() {

	this->tag = "ReturnerObject";
	this->canReturner = false;
	this->pCollider = nullptr;
	Start();
}

TitleReturner::~TitleReturner() {
	if (pCollider) {
		CollisionManager::GetInstance().UnRegister(pCollider);
		delete pCollider;
	}
}

void TitleReturner::Start() {
	modelHandle = MV1LoadModel("Res/Model/Stage/MagicCircle.mv1");
	if (!pCollider) {
		pCollider = new SphereCollider(this, VZero, 100);
	}
	SetScale(VGet(0.2, 0.2, 0.2));
}

void TitleReturner::Update() {
	if (!isVisible) return;
	GameObject::Update();

	if (pCollider) pCollider->Update();


	if (canReturner) {
		auto& input = InputManager::GetInstance();
		if (input.IsKeyDown(KEY_INPUT_X) || input.IsButtonDown(XINPUT_GAMEPAD_B)) {
			StageManager::GetInstance().ResetFloorCount();
			StageManager::GetInstance().Generate();
			EnemyManager::GetInstance().DeleteAllEnemy();
			SetVisible(false);
			isShowResetUI = true;
			return;
		}
	}



}

void TitleReturner::Render() {
	if (!isVisible) return;
	if (modelHandle != -1) {
		if (isVisible) {
			MV1SetMatrix(modelHandle, matrix);
			MV1DrawModel(modelHandle);
		}
	}

	// Zバッファ（奥行き）をチェックせずに描画する
	SetUseZBuffer3D(FALSE);
	// Zバッファに書き込みもしない（後の描画に影響を与えない）
	SetWriteZBuffer3D(FALSE);

	if (canReturner) {
		VECTOR screenPos = ConvWorldPosToScreenPos(VAdd(position, VGet(0, 100, 0)));
		int StartX = (WINDOW_WIDTH / 2) - 200;
		int StartY = (WINDOW_HEIGHT)-200;
		int GoalX = (WINDOW_WIDTH / 2) + 200;
		int GoalY = (WINDOW_HEIGHT)-150;
		int textX = StartX + 80;
		int textY = StartY + 17;

		DrawBox(StartX, StartY, GoalX, GoalY, gray, TRUE);
		DrawBox(StartX + 2, StartY + 2, GoalX - 2, GoalY - 2, white, FALSE);
		DrawFormatStringToHandle(textX + 40, textY, black, MainFont, "キー/  ボタン:0階に戻る");
		DrawFormatStringToHandle(textX + 30, textY, white, MainFont, "X");
		DrawFormatStringToHandle(textX + 83, textY, white, MainFont, "B");
	}

	// 3. 他の3Dオブジェクトの描画のために設定を元に戻す
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
}

void TitleReturner::ShowFloorResetUI()
{

	TimeManager* time = &TimeManager::GetInstance();

	// UI表示後のタイマー処理
	if (isShowResetUI) {
		resetUIDuration -= time->deltaTime;  // フレームごとの経過時間を減算
		if (resetUIDuration <= 0.0f) {
			isShowResetUI = false;  // UIを非表示
			resetUIDuration = 5.0f;

		}
	}

	int screenWidth = WINDOW_WIDTH;
	int screenHeight = WINDOW_HEIGHT;

	// 背景ボックス描画
	DrawBox(0, screenHeight / 2 - 50, screenWidth, screenHeight / 2 + 50, black, TRUE);

	// メインメッセージ描画
	DrawStringToHandle(screenWidth / 2 - 50, screenHeight / 2 - 30, "帰還", blue, MainFont_Bold);
	DrawStringToHandle(screenWidth / 2 - 80, (screenHeight / 2 - 30)+60, "フロア0に戻りました", white, MainFont);



}

void TitleReturner::OnTriggerStay(Collider* _pCol) {
	if (_pCol->GetGameObject()->GetTag() == "Player")
		canReturner = true;
}

void TitleReturner::OnTriggerExit(Collider* _pCol) {
	if (_pCol->GetGameObject()->GetTag() == "Player") canReturner = false;
}


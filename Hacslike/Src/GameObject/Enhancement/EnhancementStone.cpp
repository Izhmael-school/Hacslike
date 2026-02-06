#include "EnhancementStone.h"
#include "../Character/Player/Player.h"
#include "../../Manager/CollisionManager.h"
#include "../../Manager/InputManager.h"
#include "../../GameSystem/GameSystem.h"

EnhancementStone* EnhancementStone::pInstance = nullptr;

void EnhancementStone::CreateInstance()
{
    if (pInstance == nullptr) {
        pInstance = new EnhancementStone();
    }
}

EnhancementStone* EnhancementStone::GetInstance()
{
    if (pInstance == nullptr)
        CreateInstance();

    return pInstance;
}

void EnhancementStone::DestroyInstance()
{
    if (pInstance != nullptr) {
        delete pInstance;
        pInstance = nullptr;
    }
}

EnhancementStone::EnhancementStone() : GameObject() {
   
    this->tag = "EnhanceObject";
    this->canEnhance = false;
    this->isMenuOpen = false;
    this->pCollider = nullptr;
    this->pMenu = new StatusEnhancement(); // インスタンス生成
    Start();
}

EnhancementStone::~EnhancementStone() {
    if (pCollider) {
        CollisionManager::GetInstance().UnRegister(pCollider);
        delete pCollider;
    }
    delete pMenu; // メモリ解放
}

void EnhancementStone::Start() {
    modelHandle = MV1LoadModel("Res/Model/EnhanceObject/EnhanceObject.mv1");
    if (!pCollider) {
        pCollider = new SphereCollider(this, VZero, 80);
        //CollisionManager::GetInstance().Register(pCollider);
    }
    pMenu->Start(); // メニューの初期化
}

void EnhancementStone::Update() {
    this->matrix = MGetTranslate(this->position);

    if (isMenuOpen) {
        // メニューが開いている間の更新処理
        // pMenu->Update() が true を返すと「閉じる」合図
        if (pMenu->Update()) {
            CloseEnhanceMenu();
        }
        return; // メニュー表示中は石自体のUpdate（入力待ちなど）は行わない
    }

    if (pCollider) pCollider->Update();

    // プレイヤーが範囲内にいて、Eキーかボタンが押されたら開く
    if (canEnhance) {
        auto& input = InputManager::GetInstance();
        if (CheckHitKey(KEY_INPUT_F) == 1 || input.IsButtonDown(XINPUT_GAMEPAD_B)) {
            OpenEnhanceMenu();
        }
    }

    GameObject::Update();
}

void EnhancementStone::Render() {
    if (modelHandle != -1 ) {
        if (isVisible) {
            MV1SetMatrix(modelHandle, matrix);
            MV1DrawModel(modelHandle);
        }
    }

    // Zバッファ（奥行き）をチェックせずに描画する
    SetUseZBuffer3D(FALSE);
    // Zバッファに書き込みもしない（後の描画に影響を与えない）
    SetWriteZBuffer3D(FALSE);

    if (isMenuOpen) {
        pMenu->Render(); // これでメニューが一番手前にくる
    }
    else if (canEnhance) {
        VECTOR screenPos = ConvWorldPosToScreenPos(VAdd(position, VGet(0, 100, 0)));
        int StartX = (WINDOW_WIDTH / 2) - 200;
        int StartY = (WINDOW_HEIGHT)-200;
        int GoalX = (WINDOW_WIDTH / 2) + 200;
        int GoalY = (WINDOW_HEIGHT)-150;
        int textX = StartX + 80;
        int textY = StartY + 17;

        DrawBox(StartX, StartY, GoalX, GoalY, gray, TRUE);
        DrawBox(StartX + 2, StartY + 2, GoalX - 2, GoalY - 2, white, FALSE);
        DrawFormatStringToHandle(textX + 40, textY, black,MainFont, "キー/  ボタン:強化する");
        DrawFormatStringToHandle(textX + 30, textY, white,MainFont, "F");
        DrawFormatStringToHandle(textX + 83, textY, white,MainFont, "B");
    }

    // 3. 他の3Dオブジェクトの描画のために設定を元に戻す
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
}

void EnhancementStone::OpenEnhanceMenu() {
    isMenuOpen = true;
    GameSystem::GetInstance()->SetGameStatus(GameStatus::Stop); // ゲーム停止
    Player::GetInstance()->SetIsOpenMenu(true);
    SetMouseDispFlag(TRUE);
}

void EnhancementStone::CloseEnhanceMenu() {
    isMenuOpen = false;
    GameSystem::GetInstance()->SetGameStatus(GameStatus::Playing); // ゲーム再開
    Player::GetInstance()->SetIsOpenMenu(false);
    SetMouseDispFlag(FALSE);
}

// 判定処理（以前のまま）
void EnhancementStone::OnTriggerStay(Collider* _pCol) {
    if (_pCol->GetGameObject()->GetTag() == "Player")
        canEnhance = true;
}
void EnhancementStone::OnTriggerExit(Collider* _pCol) {
    if (_pCol->GetGameObject()->GetTag() == "Player") canEnhance = false;
}
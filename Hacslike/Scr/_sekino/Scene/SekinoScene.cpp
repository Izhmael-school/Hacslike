#include "SekinoScene.h"
#include "../GameObject/Camera/Camera.h"
#include "../GameObject/Character/Enemy/Goblin/Goblin.h"
#include "../../_tanaka/Player/Player.h"
#include "../Manager/StageManager.h"

SekinoScene::SekinoScene()
	:goblinModel(-1)
{
	Start();
}

SekinoScene::~SekinoScene() {
	// オブジェクトの開放
	for (auto pGameObject : pGameObjectArray) {
		delete pGameObject;
		pGameObject = nullptr;
	}
}

void SekinoScene::Start() {
	Player* pPlayer = new Player();
	pGameObjectArray.push_back(pPlayer);

	Camera* pCamera = new Camera(VGet(0, 400.0f, -800.0f));
	pGameObjectArray.push_back(pCamera);

	Goblin* enemy = new Goblin();

	/*Goblin* e2 = new Goblin();
	e2->SetPosition(VGet(0, 0, 200));*/

	pCamera->SetTarget(pPlayer);
	pGameObjectArray.push_back(enemy);
	//pGameObjectArray.push_back(e2);

	StageManager::GetInstance()->generator->StageGenerate();
	StageManager::GetInstance()->generator->SetGameObjectRandomPos(pPlayer);
}

void SekinoScene::Update() {
	for (auto pObj : pGameObjectArray) {
		pObj->Update();
	}
}

void SekinoScene::Render() {

	for (auto pObj : pGameObjectArray) {
		pObj->Render();
	}

#if _DEBUG 線
	// オブジェクトの位置関係がわかるように地面にラインを描画する
	{
		VECTOR pos1, pos2;

		// XZ平面 100.0f毎に1本ライン引き
		{
			pos1 = VGet(-5000.0f, 0, -5000.0f);
			pos2 = VGet(-5000.0f, 0, 5000.0f);

			for (int i = 0; i < 100; i++) {
				DrawLine3D(pos1, pos2, gray);

				pos1.x += 100.0f;
				pos2.x += 100.0f;
			}

			pos1 = VGet(-5000.0f, 0, -5000.0f);
			pos2 = VGet(5000.0f, 0, -5000.0f);
			for (int i = 0; i < 100; i++) {
				DrawLine3D(pos1, pos2, gray);

				pos1.z += 100.0f;
				pos2.z += 100.0f;
			}
		}

		// X軸
		{
			pos1 = VZero;
			pos2 = VScale(VRight, 5000);	// VRight * 5000 をしてる
			DrawLine3D(pos1, pos2, red);
		}

		// Y軸
		{
			pos1 = VZero;
			pos2 = VScale(VUp, 5000);	    // VUp * 5000 をしてる
			DrawLine3D(pos1, pos2, green);
		}

		// Z軸
		{
			pos1 = VZero;
			pos2 = VScale(VForward, 5000);	// VRight * 5000 をしてる
			DrawLine3D(pos1, pos2, blue);
		}
	}
#endif
}

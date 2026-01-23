#include "SekinoScene.h"
#include "../GameObject/Camera/Camera.h"
#include "../GameObject/Character/Enemy/Goblin/EnemyGoblin.h"
#include "../GameObject/Character/Enemy/Spider/EnemySpider.h"
#include "../GameObject/Character/Enemy/Wolf/EnemyWolf.h"
#include "../GameObject/Character/Enemy/Troll/EnemyTroll.h"
#include "../GameObject/Character/Enemy/Zombie/EnemyZombie.h"
#include "../GameObject/Character/Enemy/HellHound/EnemyHellHound.h"
#include "../GameObject/Character/Enemy/Ouger/EnemyOuger.h"
#include "../GameObject/Character/Enemy/Ketbleperz/EnemyKetbleperz.h"
#include "../GameObject/Character/Enemy/Durahan/EnemyDurahan.h"
#include "../GameObject/Character/Player/Player.h"
#include "../Manager/StageManager.h"
#include "../Manager/EnemyManager.h"

SekinoScene::SekinoScene()
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
	//

	/*pGameObjectArray.push_back(pPlayer);
	pPlayer->AddMaxHp(10000000);
	pPlayer->AddHp(pPlayer->GetMaxHp());*/
	Weapon* pWeapon = new Weapon("sabel");
	Enemy* e = EnemyManager().GetInstance().UseEnemy(Spider);
	pGameObjectArray.push_back(e);
	e->SetPosition(VGet(200, 0, 800));
	Camera* pCamera = new Camera(VGet(0, 400.0f, -800.0f));
	pGameObjectArray.push_back(pCamera);

	//pCamera->SetTarget(pPlayer);

	StageManager::GetInstance().NoFadeGenerate();
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

void SekinoScene::Setup() {}

void SekinoScene::Teardown() {}

#include "GameScene.h"
#include "../GameObject/Camera/Camera.h"
#include "../GameObject/Character/Enemy/Goblin/EnemyGoblin.h"
#include "../GameObject/Character/Player/Player.h"
#include "../Manager/StageManager.h"
#include"../Manager/ItemDropManager.h"
#include"../GameObject/Coin/Coin.h"
#include"../GameObject/TreasureChest/StartTreasureChest.h"
#include "../GameSystem/GameSystem.h"


GameScene::GameScene() {
	Start();
}

GameScene::~GameScene() {
	// オブジェクトの開放
	for (auto pGameObject : pGameObjectArray) {
		delete pGameObject;
		pGameObject = nullptr;
	}
}

void GameScene::Start() {
	Player* pPlayer = new Player();
	PlayerMovement::CreateInstance(pPlayer);
	pGameObjectArray.push_back(pPlayer);
	//Weapon* pWeapon = new Weapon("sabel");

	Camera* pCamera = new Camera(VGet(0, 400.0f, -800.0f));
	pGameObjectArray.push_back(pCamera);

	pCamera->SetTarget(pPlayer);

	AudioManager::GetInstance().Load("Res/Audio/BGM/MainGame/NormalFloor.mp3", "NormalFloor", false);
	AudioManager::GetInstance().ChangeVolume(0.1f, "NormalFloor");
	EffectManager::GetInstance().Load("Res/Effect/Item.efkefc", "Item", 10.0f);
	EffectManager::GetInstance().Load("Res/Effect/Explosion.efkefc", "Explosion", 20.0f);


	StartTreasureChest* pChest = new StartTreasureChest();
	pGameObjectArray.push_back(pChest);
	StageManager::GetInstance().SetGameObject(VGet(4, 0, 4), pChest);
	//アイテムのセット
	ItemFactory::Instance().InitializeDefaultItems();
}

void GameScene::Update() {



#pragma region プロト用スキルとアイテム
	InputManager* input = &InputManager::GetInstance();
	if (!isSelectingArtifact) {
		if (input->IsKeyDown(KEY_INPUT_Y)) {
			artifactChioces = ArtifactManager::GetInstance().GenerateArtifactChoices();
			artifactUI.StartSelection();
			isSelectingArtifact = true;
		}
		StageManager::GetInstance().Update();
		EnemyManager::GetInstance().Update();
		for (auto pObj : pGameObjectArray) {
			pObj->Update();
		}
	}
	else {
		// ★スキル選択中の処理
		//int Selected = artifactUI.UpdateSelection();
		//if (Selected != -1) {

		//	// プレイヤー取得
		//	Player* player = nullptr;
		//	for (auto p : pGameObjectArray) {
		//		player = dynamic_cast<Player*>(p);
		//		if (player) break;
		//	}

		//	if (player && Selected >= 0 && Selected < (int)artifactChioces.size()) {
		//		ArtifactManager::GetInstance().ApplySelectedArtifact(player, artifactChioces[Selected]);
		//	}

		//	isSelectingArtifact = false;
		//}
	}



	EffectManager::GetInstance().Update();
	ItemDropManager::GetInstance().Update();

#if _DEBUG アイテムドロップテスト
	// --- アイテムドロップテスト ---
	if (input->IsKeyDown(KEY_INPUT_E) || input->IsButtonDown(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		VECTOR spawnPos = Character::player->GetPosition();
		ItemDropManager::GetInstance().TryDropItem(ItemDropManager::GetInstance().GetItemDropRate(), VGet(spawnPos.x, 5.0f, spawnPos.z));
	}
	// --- アイテムドロップテスト ---
	if (input->IsKeyDown(KEY_INPUT_Q)) {
		VECTOR spawnPos = Character::player->GetPosition();

		// コインを生成
		Coin::GetInstance()->SpawnCoin(VGet(spawnPos.x + 70, 5.0f, spawnPos.z + 70));

	}
#endif

	Coin::GetInstance()->UpdateAll();

#pragma endregion



}


void GameScene::Render() {


	StageManager::GetInstance().Render();
	EnemyManager::GetInstance().Render();
	for (auto pObj : pGameObjectArray) {
		pObj->Render();
	}

	Coin::GetInstance()->RenderAll();
	// ★スキル選択画面の描画
	if (isSelectingSkill) {
		skillUI.Render(skillChoices);
	}
	if (isSelectingArtifact) {
		artifactUI.Render(artifactChioces);
	}
	// エフェクトの描画
	EffectManager::GetInstance().Render();


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

void GameScene::Setup() {
	StageManager::GetInstance().ResetFloorCount();
	StageManager::GetInstance().NoFadeGenerate();
	Player::GetInstance()->PlayerSetUp();
}

void GameScene::Teardown() {
	EnemyManager::GetInstance().UnuseAllEnemy();
	AudioManager::GetInstance().Stop("all");
}

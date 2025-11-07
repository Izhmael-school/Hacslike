#include "GameScene.h"
#include "../GameObject/Camera/Camera.h"
#include "../GameObject/Character/Enemy/Goblin/EnemyGoblin.h"
#include "../GameObject/Character/Player/Player.h"
#include "../Manager/StageManager.h"
#include"../Manager/ItemDropManager.h"
#include"../GameObject/Coin/Coin.h"


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
	pGameObjectArray.push_back(pPlayer);
	//Weapon* pWeapon = new Weapon("sabel");

	Camera* pCamera = new Camera(VGet(0, 400.0f, -800.0f));
	pGameObjectArray.push_back(pCamera);

	pCamera->SetTarget(pPlayer);

	EffectManager::GetInstance()->Load("Res/Effect/Item.efkefc", "Item", 10.0f);

	//アイテムのセット
	ItemFactory::Instance().InitializeDefaultItems();
	StageManager::GetInstance().GenerateStage();
}

void GameScene::Update() {
#pragma region プロト用スキルとアイテム
	// ★スキル選択中でなければ通常処理を行う
	if (!isSelectingSkill)
	{
		InputManager* input = &InputManager::GetInstance();

		EffectManager::GetInstance()->Update();
		ItemDropManager::GetInstance()->Update();

		// --- Hキーでスキル選択画面を開く ---
		if (input->IsKeyDown(KEY_INPUT_H) || input->IsButtonDown(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			skillChoices = SkillManager::GetInstance()->GenerateSkillChoices();
			skillUI.StartSelection();
			isSelectingSkill = true;
		}

		// --- アイテムドロップテスト ---
		if (input->IsKeyDown(KEY_INPUT_E) || input->IsButtonDown(XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			VECTOR spawnPos = Character::player->GetPosition();
			ItemDropManager::GetInstance()->TryDropItem(0.4f, VGet(spawnPos.x, 5.0f, spawnPos.z));
		}
		// --- アイテムドロップテスト ---
		if (input->IsKeyDown(KEY_INPUT_G))
		{
			VECTOR spawnPos = Character::player->GetPosition();

			// コインを生成
			Coin::GetInstance()->SpawnCoin(VGet(spawnPos.x + 70, 5.0f, spawnPos.z + 70));

		}
		StageManager::GetInstance().Update();
		for (auto pObj : pGameObjectArray) {
			pObj->Update();
		}
		EnemyManager::GetInstance().Update();
	}
	else
	{
		// ★スキル選択中の処理
		int selected = skillUI.UpdateSelection();
		if (selected != -1)
		{
			// プレイヤー取得
			Player* player = nullptr;
			for (auto p : pGameObjectArray)
			{
				player = dynamic_cast<Player*>(p);
				if (player) break;
			}

			if (player && selected >= 0 && selected < (int)skillChoices.size())
			{
				SkillManager::GetInstance()->ApplySelectedSkill(player, skillChoices[selected]);
			}

			isSelectingSkill = false;
		}
	}
#pragma endregion
	
	Coin::GetInstance()->UpdateAll();

	
}

void GameScene::Render() {


	StageManager::GetInstance().Render();
	for (auto pObj : pGameObjectArray) {
		pObj->Render();
	}
	EnemyManager::GetInstance().Render();

	Coin::GetInstance()->RenderAll();
	// ★スキル選択画面の描画
	if (isSelectingSkill)
	{
		skillUI.Render(skillChoices);
	}
	// エフェクトの描画
	EffectManager::GetInstance()->Render();
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

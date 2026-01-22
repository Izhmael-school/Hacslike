#include "TitleScene.h"
#include "../Manager/SceneManager.h"
#include "../Manager/AudioManager.h"
#include "../Manager/SaveManager.h"
#include "../GameObject/Item/ItemFactory.h"
#include "../GameObject/Character/Player/Player.h"
#include "../Manager/StageManager.h"
#include "../Manager/ArtifactManager.h"
#include "../Manager/SkillManager.h"
#include "../Manager/EnemyManager.h"
#include "../Manager/WeaponManager.h"
#include "../GameSystem/GameSystem.h"
#include "../Manager/FadeManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/TimeManager.h"
#include <ctime>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <iomanip>

TitleScene::TitleScene() 
{
	Start();
}

TitleScene::~TitleScene() {}

void TitleScene::Start() {
	AudioManager::GetInstance().Load("Res/Audio/BGM/Title/TitleBGM.mp3", "Title", false);
	SaveManager::GetInstance().RegisterSavers();
	// 追加: アイテムテンプレートを先に登録しておく（Load 前に必須）
	ItemFactory::Instance().InitializeDefaultItems();

	titleHandle = LoadGraph("Res/Title/Hacslike_title.png");

	titleMenuIndex = 0;
	inLoadMenu = false;
	selectedSlot = 0;
	messageFramesLeft = 0;
	memset(messageBuf, 0, sizeof(messageBuf));
}

void TitleScene::Update() {
#pragma region テスト用
#if _DEBUG
	if (InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_A) || InputManager::GetInstance().IsMouseDown(MOUSE_INPUT_LEFT)) {

		SceneManager::GetInstance().ChangeScene(SceneType::Game);
	}
	return;
#endif
#pragma endregion

	// Basic input helpers (match usage elsewhere in project)
	auto& input = InputManager::GetInstance();

	// Show title menu
	if (!inLoadMenu) {
		// Navigate menu
		if (input.IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN) || input.IsKeyDown(KEY_INPUT_DOWN)) {
			titleMenuIndex = (titleMenuIndex + 1) % 3;
			AudioManager::GetInstance().PlayOneShot("Select");
		}
		if (input.IsButtonDown(XINPUT_GAMEPAD_DPAD_UP) || input.IsKeyDown(KEY_INPUT_UP)) {
			titleMenuIndex = (titleMenuIndex + 2) % 3;
			AudioManager::GetInstance().PlayOneShot("Select");
		}

		// Select
		if (input.IsButtonDown(XINPUT_GAMEPAD_A) || input.IsMouseDown(MOUSE_INPUT_LEFT) || input.IsKeyDown(KEY_INPUT_RETURN)) {
			AudioManager::GetInstance().PlayOneShot("Decision");
			if (titleMenuIndex == 0) {
				
				// Change to game scene
				SceneManager::GetInstance().ChangeScene(SceneType::Game);
			}
			else if (titleMenuIndex == 1) {
				// Open load menu
				inLoadMenu = true;
				selectedSlot = 0;
			}
			else if (titleMenuIndex == 2) {
				SceneManager::GetInstance().SetEnd(true);
			}
		}
	}
	else {
		// Load menu: list save slots, allow Load (A) or Delete (X), Back (B)
		const auto& slots = SaveManager::GetInstance().GetSlots();

		if (input.IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN) || input.IsKeyDown(KEY_INPUT_DOWN)) {
			selectedSlot = (selectedSlot + 1) % 10;
			AudioManager::GetInstance().PlayOneShot("Select");
		}
		if (input.IsButtonDown(XINPUT_GAMEPAD_DPAD_UP) || input.IsKeyDown(KEY_INPUT_UP)) {
			selectedSlot = (selectedSlot + 9) % 10;
			AudioManager::GetInstance().PlayOneShot("Select");
		}

		// Load
		if (input.IsButtonDown(XINPUT_GAMEPAD_A) || input.IsKeyDown(KEY_INPUT_RETURN) || input.IsMouseDown(MOUSE_INPUT_LEFT)) {
			if (!slots[selectedSlot].exists) {
				// show "no save data"
				snprintf(messageBuf, sizeof(messageBuf), "セーブデータがありません (Slot %02d)", selectedSlot + 1);
				messageFramesLeft = 120; // show for ~2 seconds at 60fps
				AudioManager::GetInstance().PlayOneShot("Select");
			}
			else {
				// Ensure Player instance exists before load handlers run
				/*Player::DestroyInstance();*/
				//Player* p = Player::CreateInstance(VZero);
				/*if (p) {*/
					// Load into managers and player
					if (SaveManager::GetInstance().Load(selectedSlot)) {
						// After successful load, go to game scene
						SceneManager::GetInstance().ChangeScene(SceneType::Game);
						return;
					}
					else {
						snprintf(messageBuf, sizeof(messageBuf), "ロードに失敗しました (Slot %02d)", selectedSlot + 1);
						messageFramesLeft = 120;
					}
				//}
			}
		}

		// Delete
		if (input.IsButtonDown(XINPUT_GAMEPAD_X) || input.IsKeyDown(KEY_INPUT_DELETE)) {
			// Confirm deletion quickly (for simplicity, immediate)
			bool ok = SaveManager::GetInstance().Delete(selectedSlot);
			if (ok) {
				snprintf(messageBuf, sizeof(messageBuf), "セーブデータを削除しました (Slot %02d)", selectedSlot + 1);
				messageFramesLeft = 120;
			}
			else {
				snprintf(messageBuf, sizeof(messageBuf), "削除に失敗しました (Slot %02d)", selectedSlot + 1);
				messageFramesLeft = 120;
			}
		}

		// Back
		if (input.IsButtonDown(XINPUT_GAMEPAD_B) || input.IsKeyDown(KEY_INPUT_ESCAPE)) {
			inLoadMenu = false;
			AudioManager::GetInstance().PlayOneShot("Decision");
		}
	}

	// Message timer decrement
	if (messageFramesLeft > 0) --messageFramesLeft;


}

static std::string FormatTime(std::time_t t) {
	if (t == 0) return std::string("----/--/-- --:--");
	std::tm tm{};
	localtime_s(&tm, &t);
	std::ostringstream ss;
	ss << std::setfill('0') << std::setw(4) << (tm.tm_year + 1900) << '/'
		<< std::setw(2) << (tm.tm_mon + 1) << '/'
		<< std::setw(2) << tm.tm_mday << ' '
		<< std::setw(2) << tm.tm_hour << ':' << std::setw(2) << tm.tm_min;
	return ss.str();
}

void TitleScene::Render() {
	// Title
	DrawGraph(0, 0, titleHandle, TRUE);

	if (!inLoadMenu) {
		// Title menu
		DrawString(550, 400, (titleMenuIndex == 0) ? "> ニューゲーム" : "  ニューゲーム", white);
		DrawString(550, 440, (titleMenuIndex == 1) ? "> ロード" : "  ロード", white);
		DrawString(550, 480, (titleMenuIndex == 2) ? "> 終了" : "  終了", white);

		// Hint
		DrawFormatString(550, 540, white, "A: 決定  /  上下: 選択");
	}
	else {
		// Load menu - display 10 slots
		DrawString(500, 120, "ロードメニュー", white);
		const auto& slots = SaveManager::GetInstance().GetSlots();
		int startY = 180;
		for (int i = 0; i < 10; ++i) {
			int y = startY + i * 36;
			char buf[256];
			if (slots[i].exists) {
				// Show level, floor, timestamp or description if present
				std::string tmStr = FormatTime(slots[i].timestamp);
				if (slots[i].desc[0]) {
					snprintf(buf, sizeof(buf), "%02d: %s  (%s)", i + 1, slots[i].desc, tmStr.c_str());
				}
				else {
					snprintf(buf, sizeof(buf), "%02d: Lv%d 階層:%d  %s", i + 1, slots[i].playerLevel, slots[i].floor, tmStr.c_str());
				}
			}
			else {
				snprintf(buf, sizeof(buf), "%02d: --- セーブなし ---", i + 1);
			}

			// highlight selected
			if (i == selectedSlot) {
				DrawString(460, y, ">", yellow);
				DrawString(480, y, buf, white);
			}
			else {
				DrawString(480, y, buf, gray);
			}
		}

		// Instructions
		DrawFormatString(480, startY + 10 * 36 + 20, white, "A: ロード   X: 削除   B: 戻る");
	}

	// Message area
	if (messageFramesLeft > 0) {
		int alpha = 255;
		DrawBox(400, 820, 1200, 880, black, TRUE);
		DrawBox(402, 822, 1198, 878, white, FALSE);
		DrawFormatString(420, 830, white, "%s", messageBuf);
	}
	/*DrawString(600,200,"Hacslike",red);
	DrawString(600, 600, "左クリックかAボタン",red);*/
}

void TitleScene::Setup() {
	AudioManager::GetInstance().PlayBGM("Title");
	//アイテムのセット
	ItemFactory::Instance().InitializeDefaultItems();
}

void TitleScene::Teardown() {
	AudioManager::GetInstance().Stop("Title");
}

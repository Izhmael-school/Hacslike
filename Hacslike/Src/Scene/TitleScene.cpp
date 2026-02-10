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
	fontHandle = FontManager::GetInstance().UseFontHandle("MainFont");
	logoHandle = LoadGraph("Res/Title/TeamLogo.png");
	// ★修正: descFontHandle を別途取得するか、fontHandle をコピー
	descFontHandle = FontManager::GetInstance().UseFontHandle("MainFont");
	descriptionAlpha = 255.0f;  // ★変更: 最初から255にして即座に表示
	pendingStartMode = GameStartMode::None;

	// メニュー説明文の初期化
	InitializeMenuDescriptions();
}


void TitleScene::InitializeMenuDescriptions()
{
	// ニューゲーム
	menuDescriptions[0].title = "【 ニューゲーム 】";
	menuDescriptions[0].description[0] = "最初から冒険を始めます。";
	menuDescriptions[0].description[1] = "全てのステータスが初期値にリセットされ、";
	menuDescriptions[0].description[2] = "新たな挑戦が待ち受けています。";
	menuDescriptions[0].description[3] = "初めてプレイする方はこちらを選択してください。";
	menuDescriptions[0].lineCount = 4;

	// 強くてニューゲーム
	menuDescriptions[1].title = "【 強くてニューゲーム 】";
	menuDescriptions[1].description[0] = "これまでの強化を引き継いで冒険を始めます。";
	menuDescriptions[1].description[1] = "コイン、ステータス強化";
	menuDescriptions[1].description[2] = "進行状況を保持したまま1階層から再挑戦できます。";
	menuDescriptions[1].description[3] = "アイテム、スキル、アーティファクトは無くなり。";
	menuDescriptions[1].lineCount = 4;

	// ロード
	menuDescriptions[2].title = "【 ロード 】";
	menuDescriptions[2].description[0] = "セーブデータから冒険を再開します。";
	menuDescriptions[2].description[1] = "最大10個のセーブスロットから選択できます。";
	menuDescriptions[2].description[2] = "中断した階層、レベル、アイテムなどが";
	menuDescriptions[2].description[3] = "そのまま復元されます。";
	menuDescriptions[2].lineCount = 4;

	// 終了
	menuDescriptions[3].title = "【 ゲーム終了 】";
	menuDescriptions[3].description[0] = "ゲームを終了します。";
	menuDescriptions[3].description[1] = "セーブした内容はセーブデータに保存されています。";
	menuDescriptions[3].description[2] = "またのご来訪をお待ちしております。";
	menuDescriptions[3].description[3] = "";
	menuDescriptions[3].lineCount = 3;
}

void TitleScene::Update() {
#pragma region テスト用
#if _DEBUG
	//if (InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_A) || InputManager::GetInstance().IsMouseDown(MOUSE_INPUT_LEFT)) {

	//	SceneManager::GetInstance().ChangeScene(SceneType::Sekino);
	//}
	//return;
#endif
#pragma endregion

	// Basic input helpers (match usage elsewhere in project)
	auto& input = InputManager::GetInstance();

	// Show title menu
	if (!inLoadMenu) {
		// Navigate menu - メニュー項目が4つに増えたので範囲を変更
		if (input.IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN) || input.IsKeyDown(KEY_INPUT_DOWN)) {
			titleMenuIndex = (titleMenuIndex + 1) % 4;
			AudioManager::GetInstance().PlayOneShot("Select");
			// ★追加: メニュー変更時にアルファ値をリセット
			descriptionAlpha = 0.0f;
		}
		if (input.IsButtonDown(XINPUT_GAMEPAD_DPAD_UP) || input.IsKeyDown(KEY_INPUT_UP)) {
			titleMenuIndex = (titleMenuIndex + 3) % 4;
			AudioManager::GetInstance().PlayOneShot("Select");
			// ★追加: メニュー変更時にアルファ値をリセット
			descriptionAlpha = 0.0f;
		}

		// ★追加: フェードイン効果
		if (descriptionAlpha < 255.0f) {
			descriptionAlpha += 15.0f;
			if (descriptionAlpha > 255.0f) descriptionAlpha = 255.0f;
		}

		// Select
		if (input.IsButtonDown(XINPUT_GAMEPAD_A) || input.IsMouseDown(MOUSE_INPUT_LEFT) || input.IsKeyDown(KEY_INPUT_RETURN)) {
			AudioManager::GetInstance().PlayOneShot("Decision");
			if (titleMenuIndex == 0) {
				// ニューゲーム（完全リセット）
				Player::RequestFullReset(); // フラグを立てる
				if (Player::GetInstance()) {
					Player::GetInstance()->ResetUIStates();
					GameSystem::GetInstance()->SetGameStatus(GameStatus::Playing);
				}
				SceneManager::GetInstance().ChangeScene(SceneType::Game);
			}
			else if (titleMenuIndex == 1) {
				// 強くてニューゲーム（ステータス引き継ぎ）
				Player::ClearFullResetFlag(); // フラグを下ろす
				if (Player::GetInstance()) {
					Player::GetInstance()->ResetUIStates();
					GameSystem::GetInstance()->SetGameStatus(GameStatus::Playing);
				}
				SceneManager::GetInstance().ChangeScene(SceneType::Game);
			}
			else if (titleMenuIndex == 2) {
				// ロードメニューを開く
				inLoadMenu = true;
				selectedSlot = 0;
			}
			else if (titleMenuIndex == 3) {
				// 終了
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
				// Load into managers and player
				if (SaveManager::GetInstance().Load(selectedSlot)) {
					Player::GetInstance()->ResetUIStates();
					pendingStartMode = GameStartMode::Load;
					// After successful load, go to game scene
					SceneManager::GetInstance().ChangeScene(SceneType::Game);
					return;
				}
				else {
					snprintf(messageBuf, sizeof(messageBuf), "ロードに失敗しました (Slot %02d)", selectedSlot + 1);
					messageFramesLeft = 120;
				}
			}
		}

		// Delete
		if (input.IsButtonDown(XINPUT_GAMEPAD_X) || input.IsKeyDown(KEY_INPUT_DELETE)) {
			// Confirm deletion quickly (for simplicity, immediate)
			bool ok = SaveManager::GetInstance().Delete(selectedSlot);
			if (ok) {
				AudioManager::GetInstance().PlayOneShot("Decision");
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

void TitleScene::RenderMenuDescription()
{
	if (titleMenuIndex < 0 || titleMenuIndex >= 4) return;

	const MenuDescription& desc = menuDescriptions[titleMenuIndex];

	// メニュー項目の位置に合わせて説明ボックスを配置
	int menuStartY = 520;
	int menuItemHeight = 40;
	int selectedMenuY = menuStartY + titleMenuIndex * menuItemHeight;

	// 説明文ボックスの位置とサイズ（メニューの右側に配置）
	int boxX = 1100;  // メニューの左側に配置
	int boxY = selectedMenuY - 10;  // 選択中のメニュー項目に合わせる
	int boxWidth = 460;
	int boxHeight = 180;

	// ★変更: アルファ値を固定で255に
	int alpha = 255;

	// 背景ボックス（半透明の黒）
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);  // 0.8f * 255 = 204 -> 200に固定
	DrawBox(boxX, boxY, boxX + boxWidth, boxY + boxHeight, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 外枠（選択中の色で光らせる）
	int frameColor = GetColor(100, 200, 255);
	if (titleMenuIndex == 1) frameColor = GetColor(255, 200, 100); // 強くてニューゲームは金色
	else if (titleMenuIndex == 2) frameColor = GetColor(100, 255, 100); // ロードは緑色
	else if (titleMenuIndex == 3) frameColor = GetColor(255, 100, 100); // 終了は赤色

	// ★変更: 外枠は透明度なしで描画
	DrawBox(boxX - 2, boxY - 2, boxX + boxWidth + 2, boxY + boxHeight + 2, frameColor, FALSE);
	DrawBox(boxX - 3, boxY - 3, boxX + boxWidth + 3, boxY + boxHeight + 3, frameColor, FALSE);

	// タイトル（見出し）
	DrawStringToHandle(boxX + 20, boxY + 15, desc.title, GetColor(255, 255, 100), fontHandle);

	// 区切り線
	DrawLine(boxX + 20, boxY + 45, boxX + boxWidth - 20, boxY + 45, GetColor(150, 150, 150));

	// 説明文（各行）
	int lineY = boxY + 55;
	int lineHeight = 28;
	for (int i = 0; i < desc.lineCount; ++i) {
		if (desc.description[i] && strlen(desc.description[i]) > 0) {
			DrawStringToHandle(boxX + 20, lineY + i * lineHeight, desc.description[i], white, descFontHandle);
		}
	}
}


void TitleScene::Render() {
	// Title
	DrawGraph(0, 0, titleHandle, TRUE);

	DrawExtendGraph(WINDOW_WIDTH - 140, WINDOW_HEIGHT - 50, WINDOW_WIDTH, WINDOW_HEIGHT, logoHandle, true);

	if (!inLoadMenu) {
		// メニュー項目
		int menuStartY = 520;
		int menuItemHeight = 40;
		DrawStringToHandle(860, menuStartY + 0 * menuItemHeight, (titleMenuIndex == 0) ? "> ニューゲーム" : "  ニューゲーム",
			titleMenuIndex == 0 ? GetColor(255, 255, 100) : white, fontHandle);
		DrawStringToHandle(860, menuStartY + 1 * menuItemHeight, (titleMenuIndex == 1) ? "> 強くてニューゲーム" : "  強くてニューゲーム",
			titleMenuIndex == 1 ? GetColor(255, 200, 100) : white, fontHandle);
		DrawStringToHandle(860, menuStartY + 2 * menuItemHeight, (titleMenuIndex == 2) ? "> ロード" : "  ロード",
			titleMenuIndex == 2 ? GetColor(100, 255, 100) : white, fontHandle);
		DrawStringToHandle(860, menuStartY + 3 * menuItemHeight, (titleMenuIndex == 3) ? "> 終了" : "  終了",
			titleMenuIndex == 3 ? GetColor(255, 100, 100) : white, fontHandle);

		// 操作説明
		DrawFormatStringToHandle(800, 740, GetColor(200, 200, 200), descFontHandle, "A/Enter/左クリック: 決定  /  ↑↓: 選択");
		// メニュー説明文の描画
		RenderMenuDescription();
	}
	else {
		// Load menu - display 10 slots
		DrawStringToHandle(860, 500, "ロードメニュー", white, fontHandle);
		const auto& slots = SaveManager::GetInstance().GetSlots();
		int startY = 560;
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
					snprintf(buf, sizeof(buf), "%02d: Lv%d 階層:%d  %s", i + 1, slots[i].playerLevel, slots[i].floor - 1, tmStr.c_str());
				}
			}
			else {
				snprintf(buf, sizeof(buf), "%02d: --- セーブなし ---", i + 1);
			}

			// highlight selected
			if (i == selectedSlot) {
				DrawStringToHandle(840, y, ">", yellow, fontHandle);
				DrawStringToHandle(860, y, buf, white, fontHandle);
			}
			else {
				DrawStringToHandle(860, y, buf, gray, fontHandle);
			}
		}

		// Instructions
		DrawFormatStringToHandle(800, startY + 10 * 36 + 20, white, fontHandle, "A/Enter: ロード   X/DEL: 削除   B/ESC: 戻る");
	}

	// Message area
	if (messageFramesLeft > 0) {
		int alpha = 255;
		DrawBox(400, 820, 1200, 880, black, TRUE);
		DrawBox(402, 822, 1198, 878, white, FALSE);
		DrawFormatStringToHandle(420, 830, white, fontHandle, "%s", messageBuf);
	}
}

void TitleScene::Setup() {
	AudioManager::GetInstance().PlayBGM("Title");
}

void TitleScene::Teardown() {
	AudioManager::GetInstance().Stop("Title");
}




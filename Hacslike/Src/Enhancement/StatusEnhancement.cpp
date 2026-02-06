#include "StatusEnhancement.h"
#include "DxLib.h"
#include <cmath>
#include "../Manager/InputManager.h"
#include "../GameObject/Character/Player/Player.h"
#include"../Save/SaveIO.h"

// グローバル参照用ポインタ定義（クラス内 static の定義）
StatusEnhancement* StatusEnhancement::g_instance = nullptr;

// プログラム開始時に必ず存在する実インスタンスを作成する
// （翻訳単位スコープの静的オブジェクトとして置く）
static StatusEnhancement g_globalStatusEnhancement;

static int oldMouse = 0;

StatusEnhancement::StatusEnhancement()
	: timer(0.0f)
	, allMax(false)
	, playerCoins(0)
	, selectedIndex(0) {
	// コンストラクタでグローバル参照を設定
	g_instance = this;
}

StatusEnhancement::~StatusEnhancement() {
	if (g_instance == this) g_instance = nullptr;
}

void StatusEnhancement::Start() {
	if (!stats.empty()) return;

	AudioManager::GetInstance().Load("Res/SE/決定ボタンを押す2.mp3", "SelectSkill", false);

	allMax = false;
	selectedIndex = 0;

	unsigned int defaultColor = GetColor(200, 200, 200);

	// 各ステータスの初期化（名前, レベル, 累積加算値, 色）
	stats.push_back({ "H P",      0, 0, defaultColor });
	stats.push_back({ "攻撃力",   0, 0, defaultColor });
	stats.push_back({ "防御力",   0, 0, defaultColor });
	stats.push_back({ "会心率",   0, 0, defaultColor });
	stats.push_back({ "会心ダメ", 0, 0, defaultColor });
}

bool StatusEnhancement::Update() {
	timer += 0.05f;
	SetMouseDispFlag(TRUE);

	Player* player = Player::GetInstance();
	if (player) playerCoins = player->GetCoinValue();

	InputManager* input = &InputManager::GetInstance();
	if (!input) return false;

	// --- レイアウト基準（Renderと完全に同期させる） ---
	int centerX = WINDOW_WIDTH / 2;

	// 【1200x800基準で再計算】
	int curW = (int)(WINDOW_WIDTH * 0.0541f);    // 1200pxで65相当
	int curGap = (int)(WINDOW_WIDTH * 0.0058f);  // 1200pxで7相当
	int curSkew = (int)(curW * 0.276f);          // w=65で18相当
	int totalGaugeWidth = (curW * 10) + (curGap * 9) + curSkew;

	int gaugeStartX = centerX - (totalGaugeWidth / 2);
	int baseY = (int)(WINDOW_HEIGHT * 0.225f);    // 800pxで180相当
	int itemGap = (int)(WINDOW_HEIGHT * 0.1375f); // 800pxで110相当

	// パッド（十字キー）による選択移動
	if (input->IsButtonDown(XINPUT_GAMEPAD_DPAD_UP)) {
		selectedIndex--;
		AudioManager::GetInstance().PlayOneShot("SelectSkill");
		if (selectedIndex < 0) selectedIndex = (int)stats.size() - 1;
	}
	if (input->IsButtonDown(XINPUT_GAMEPAD_DPAD_DOWN)) {
		selectedIndex++;
		AudioManager::GetInstance().PlayOneShot("SelectSkill");
		if (selectedIndex >= (int)stats.size()) selectedIndex = 0;
	}

	// マウスによる選択更新（拡大したゲージの当たり判定）
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	int previousIndex = selectedIndex;

	for (int i = 0; i < (int)stats.size(); i++) {
		int x = gaugeStartX;
		int y = baseY + i * itemGap;

		// 判定範囲：項目全体をカバー
		if (mouseX >= x - 150 && mouseX <= x + totalGaugeWidth + 100 && mouseY >= y && mouseY <= y + (int)(itemGap * 0.75f)) {
			selectedIndex = i;
		}
	}
	if (selectedIndex != previousIndex) {
		AudioManager::GetInstance().PlayOneShot("SelectSkill");
	}

#if _DEBUG
	// デバッグコマンド: B + I + A + N + C + O キー同時押しで【全ステータス】をLvMAX
	char keyStates[256];
	GetHitKeyStateAll(keyStates); // 全256キーの状態を取得

	// 1. 指定の6キーがすべて押されているか
	bool isBiancoPressed = keyStates[KEY_INPUT_B] && keyStates[KEY_INPUT_I] &&
		keyStates[KEY_INPUT_A] && keyStates[KEY_INPUT_N] &&
		keyStates[KEY_INPUT_C] && keyStates[KEY_INPUT_O];

	// 2. それ以外のキーが押されていないかチェック
	bool noOtherKeys = true;
	for (int i = 0; i < 256; i++) {
		// チェック対象のキー（BIANCO）はスキップ
		if (i == KEY_INPUT_B || i == KEY_INPUT_I || i == KEY_INPUT_A ||
			i == KEY_INPUT_N || i == KEY_INPUT_C || i == KEY_INPUT_O) {
			continue;
		}

		// もし一つでも押されているキーがあればfalse
		if (keyStates[i] != 0) {
			noOtherKeys = false;
			break;
		}
	}

	// 最終判定
	if (isBiancoPressed && noOtherKeys) {
		// stats配列の全要素（全項目）をループで回す
		for (int i = 0; i < (int)stats.size(); ++i) {

			// すでにLv50ならスキップ
			if (stats[i].level >= 50) continue;

			int remainingLevels = 50 - stats[i].level;
			float boostValue = 0;

			// インデックス(i)に応じて上昇値を設定
			switch (i) {
			case 0: boostValue = 10;  break; // HP
			case 1: boostValue = 2;   break; // 攻撃
			case 2: boostValue = 1;   break; // 防御
			case 3: boostValue = 0.8f;   break; // 会心率
			case 4: boostValue = 500; break; // 会心ダメ
			}

			int totalBoost = boostValue * remainingLevels;

			// UI表示用のデータを更新
			stats[i].totalBonus += totalBoost;
			stats[i].level = 50;

			// Playerクラスの実数値に一括反映
			if (player) {
				switch (i) {
				case 0:
					player->SetMaxHp(player->GetMaxHp() + (float)totalBoost);
					player->SetHp(player->GetMaxHp());
					break;
				case 1:
					player->SetBaseAtk(player->GetBaseAtk() + totalBoost);
					player->UpdateAtkFromEquipment();

					break;
				case 2:
					player->SetDef(player->GetDef() + (float)totalBoost);
					break;
				case 3:
					player->SetCriticalHitRate(player->GetCriticalHitRate() + totalBoost);
					break;
				case 4:
					player->SetCriticalDamage(player->GetCriticalDamage() + (totalBoost / 100.0f));
					break;
				}
			}
		}
	}
#endif

	// --- 強化実行処理 ---
	if (input->IsMouseDown(MOUSE_INPUT_LEFT) || input->IsButtonDown(XINPUT_GAMEPAD_B)) {
		if (!stats.empty() && stats[selectedIndex].level < 50) {

			// コスト計算
			int cost = 3 + (stats[selectedIndex].level * 4);

			// コインが足りるかチェック
			if (playerCoins >= cost) {
				// コイン消費
				playerCoins -= cost;
				if (player) player->SetCoinValue(playerCoins);

				// 各ステータスの上昇幅
				float boostValue = 0;
				switch (selectedIndex) {
				case 0: boostValue = 10; break; // HP +10
				case 1: boostValue = 2;  break; // 攻撃 +2
				case 2: boostValue = 1;  break; // 防御 +1
				case 3: boostValue = 0.8f;  break; // 会心率 +1
				case 4: boostValue = 500;  break; // 会心ダメ +5
				}

				stats[selectedIndex].totalBonus += boostValue;
				stats[selectedIndex].level++;

				// Playerクラスのステータスに反映
				if (player) {
					switch (selectedIndex) {
					case 0: // HP
						if (player != nullptr) { // ヌルチェック（逆参照エラー防止）
							// 今の最大HPに、今回増えた分（boostValue = 10）だけを足す
							float newMax = player->GetMaxHp() + (float)boostValue;

							player->SetMaxHp(newMax);
							player->SetHp(newMax); // 回復もさせる場合
						}
						break;
					case 1: // 攻撃力
						player->SetBaseAtk(player->GetBaseAtk() + (int)boostValue);
						player->UpdateAtkFromEquipment();
						break;
					case 2: // 防御力
						player->SetDef(player->GetDef() + (float)boostValue);
						break;
					case 3: // 会心率
						player->SetCriticalHitRate(player->GetCriticalHitRate() + boostValue);
						break;
					case 4: // 会心ダメ
						player->SetCriticalDamage(player->GetCriticalDamage() + (boostValue / 100.0f));
						break;
					}
				}
			}
		}
	}

	// 全ステータスMax判定
	bool check = !stats.empty();
	for (const auto& s : stats) {
		if (s.level < 50) { check = false; break; }
	}
	allMax = check;

	// 閉じる処理
	if (input->IsButtonDown(XINPUT_GAMEPAD_A) || input->IsKeyDown(KEY_INPUT_ESCAPE)) {
		return true;
	}

	return false;
}

void StatusEnhancement::Render() {
	if (stats.empty()) {
		Start();
		return;
	}

	SetUseZBuffer3D(FALSE);
	SetWriteZBuffer3D(FALSE);
	ClearDrawScreenZBuffer();
	SetUseLighting(FALSE);

	// --- 【1200x800基準でレスポンシブ計算】 ---
	int centerX = WINDOW_WIDTH / 2;
	int curW = (int)(WINDOW_WIDTH * 0.0541f);
	int curGap = (int)(WINDOW_WIDTH * 0.0058f);
	int curSkew = (int)(curW * 0.276f);
	int totalGaugeWidth = (curW * 10) + (curGap * 9) + curSkew;

	int gaugeStartX = centerX - (totalGaugeWidth / 2);
	int baseY = (int)(WINDOW_HEIGHT * 0.225f);
	int itemGap = (int)(WINDOW_HEIGHT * 0.1375f);

	// 背景パネル
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(5, 5, 10), TRUE);

	// 所持コイン表示
	unsigned int coinColor = GetColor(255, 215, 0);
	DrawFormatStringToHandle(WINDOW_WIDTH - 280, (int)(WINDOW_HEIGHT * 0.075f), coinColor, MainFont, "COINS: %d", playerCoins);

	for (int i = 0; i < (int)stats.size(); i++) {
		int x = gaugeStartX;
		int y = baseY + i * itemGap;

		// 選択カーソル
		if (i == selectedIndex) {
			int animX = (int)(sin(GetNowCount() / 150.0f) * 5.0f);
			DrawString(x - 175 + animX, y + 15, "->", GetColor(255, 255, 255));
		}

		// 項目名・レベル・ボーナス
		DrawStringToHandle(x - 140, y + 5, stats[i].name.c_str(), GetColor(255, 255, 255), MainFont);
		DrawFormatStringToHandle(x - 140, y + 32, (stats[i].level >= 50 ? GetColor(255, 215, 0) : GetColor(150, 150, 150)), MainFont, "Lv.%d", stats[i].level);
		if (stats[i].name == "会心ダメ")
			DrawFormatStringToHandle(x - 140, y + 62, GetColor(0, 255, 150), MainFont, "+%d%%", (int)stats[i].totalBonus / 100);
		else if (stats[i].name == "会心率")
			DrawFormatStringToHandle(x - 140, y + 62, GetColor(0, 255, 150), MainFont, "+%.1f%%", stats[i].totalBonus);
		else
			DrawFormatStringToHandle(x - 140, y + 62, GetColor(0, 255, 150), MainFont, "+%d", (int)stats[i].totalBonus);

		// ゲージ描画
		DrawParallelGauge(x, y, stats[i].level, stats[i].color);

		// コスト表示
		int costX = x + totalGaugeWidth + 20;
		if (stats[i].level < 50) {
			int cost = 3 + (stats[i].level * 4);
			unsigned int cCol = (playerCoins >= cost) ? GetColor(255, 255, 255) : GetColor(255, 50, 50);
			DrawFormatStringToHandle(costX, y + 15, cCol, MainFont, "COST: %d", cost);
		}
		else {
			DrawStringToHandle(costX, y + 15, "MAX", GetColor(255, 215, 0), MainFont);
		}
	}

	if (allMax) {
		DrawStringToHandle(centerX - 200, WINDOW_HEIGHT - 80, "ULTIMATE RAINBOW ACHIEVED", GetColor(255, 255, 255), MainFont);
	}

	int StartX = (WINDOW_WIDTH / 2) - 200;
	int StartY = (WINDOW_HEIGHT)-100;
	int GoalX = (WINDOW_WIDTH / 2) + 200;
	int GoalY = (WINDOW_HEIGHT)-50;
	int textX = StartX + 80;
	int textY = StartY + 17;

	DrawBox(StartX, StartY, GoalX, GoalY, gray, TRUE);
	DrawBox(StartX + 2, StartY + 2, GoalX - 2, GoalY - 2, white, FALSE);
	DrawFormatStringToHandle(textX + 50, textY, black, MainFont, "キー/  ボタン:閉じる");
	DrawFormatStringToHandle(textX + 20, textY, white, MainFont, "ESC");
	DrawFormatStringToHandle(textX + 93, textY, white, MainFont, "A");

	SetUseLighting(TRUE);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
}

void StatusEnhancement::DrawParallelGauge(int x, int y, int level, unsigned int baseColor) {
	// 【1200x800基準でサイズ計算】
	int w = (int)(WINDOW_WIDTH * 0.0541f);    // 1200で65
	int h = (int)(WINDOW_HEIGHT * 0.05625f); // 800で45
	int skew = (int)(w * 0.276f);             // w=65で18
	int gap = (int)(WINDOW_WIDTH * 0.0058f);  // 1200で7

	float systemTime = GetNowCount() / 1000.0f;

	int loopCount = (level - 1) / 10;
	if (level <= 0) loopCount = 0;
	int currentLoopLevel = level % 10;
	if (currentLoopLevel == 0 && level > 0) currentLoopLevel = 10;

	unsigned int loopColors[] = {
		GetColor(180, 180, 180), GetColor(0, 110, 220), GetColor(220, 180, 0),
		GetColor(140, 0, 230), GetColor(200, 0, 50)
	};

	for (int i = 0; i < 10; i++) {
		float offsetY = allMax ? sin(systemTime * 4.0f - (i * 0.5f)) * 12.0f : 0.0f;
		int curY = y + (int)offsetY;

		unsigned int contentCol, outerBorder, innerLight;
		bool isFilled = (i < currentLoopLevel);

		if (allMax) {
			float h_val = systemTime * 3.0f - (i * 0.4f);
			contentCol = GetColor((int)(128 + 127 * sin(h_val)), (int)(128 + 127 * sin(h_val + 2.09f)), (int)(128 + 127 * sin(h_val + 4.18f)));
			outerBorder = GetColor(255, 215, 0);
			innerLight = GetColor(220, 220, 180);
		}
		else {
			if (loopCount >= 1) contentCol = isFilled ? loopColors[loopCount] : loopColors[loopCount - 1];
			else contentCol = isFilled ? loopColors[0] : GetColor(30, 30, 35);

			int effectiveLoop = isFilled ? loopCount : (loopCount - 1);
			if (effectiveLoop < 0) effectiveLoop = -1;

			if (effectiveLoop >= 4) { outerBorder = GetColor(255, 215, 0); innerLight = GetColor(255, 255, 180); }
			else if (effectiveLoop >= 2) { outerBorder = GetColor(180, 180, 190); innerLight = GetColor(255, 255, 255); }
			else if (effectiveLoop >= 0) { outerBorder = GetColor(100, 100, 100); innerLight = GetColor(140, 140, 140); }
			else { outerBorder = GetColor(50, 50, 50); innerLight = GetColor(70, 70, 70); }

			bool isNextTarget = (i == (level % 10)) && (level < 50);
			if (isNextTarget) {
				float pulse = (sin(systemTime * 6.0f) + 1.0f) / 2.0f;
				outerBorder = GetColor((int)(100 + 155 * pulse), (int)(100 + 155 * pulse), (int)(100 + 155 * pulse));
				innerLight = GetColor(255, 255, 255);
			}
		}

		int x1 = x + i * (w + gap) + skew; int y1 = curY;
		int x2 = x1 + w;                   int y2 = curY;
		int x3 = x + i * (w + gap) + w;    int y3 = curY + h;
		int x4 = x + i * (w + gap);        int y4 = curY + h;

		DrawQuadrangle(x1, y1, x2, y2, x3, y3, x4, y4, contentCol, TRUE);
		DrawQuadrangle(x1, y1, x2, y2, x3, y3, x4, y4, outerBorder, FALSE);

		if (allMax || loopCount > 0 || isFilled || (i == (level % 10) && level < 50)) {
			float shineIdx = sin(systemTime * 3.5f - (i * 0.4f));
			unsigned int finalLight = innerLight;
			if (shineIdx > 0.85f) finalLight = GetColor(255, 255, 255);
			DrawQuadrangle(x1 + 2, y1 + 2, x2 - 1, y2 + 2, x3 - 2, y3 - 2, x4 + 1, y4 - 2, finalLight, FALSE);
		}
	}
}

void StatusEnhancement::SaveTo(BinaryWriter& w) {
	if (!g_instance) {
		uint32_t zero = 0; w.WritePOD(zero);
		bool am = false; int si = 0; w.WritePOD(am); w.WritePOD(si);
		return;
	}
	uint32_t count = static_cast<uint32_t>(g_instance->stats.size());
	w.WritePOD(count);
	for (const auto& s : g_instance->stats) {
		w.WriteString(s.name); w.WritePOD(s.level); w.WritePOD(s.totalBonus); w.WritePOD(s.color);
	}
	w.WritePOD(g_instance->allMax); w.WritePOD(g_instance->selectedIndex);
}

void StatusEnhancement::LoadFrom(BinaryReader& r, uint32_t ver) {
	uint32_t count = 0; r.ReadPOD(count);
	g_instance->stats.clear();
	for (uint32_t i = 0; i < count; ++i) {
		StatData sd; sd.name = r.ReadString();
		r.ReadPOD(sd.level); r.ReadPOD(sd.totalBonus); r.ReadPOD(sd.color);
		g_instance->stats.push_back(sd);
	}
	r.ReadPOD(g_instance->allMax); r.ReadPOD(g_instance->selectedIndex);
}

void StatusEnhancement::StatusSetUp() {
	for (auto& s : stats) { s.level = 0; s.totalBonus = 0; }
	allMax = false; selectedIndex = 0;
}
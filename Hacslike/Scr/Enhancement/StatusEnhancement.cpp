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
	// ゲージサイズ拡大(w:65, gap:7)に基づき、10個分の幅を考慮して中央寄せ
	int gaugeStartX = centerX - 370;
	int baseY = 180;

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
		int y = baseY + i * 110; // 項目の縦間隔

		// 判定範囲：名前(x-150)からコスト表示(x+850)まで、高さはゲージ高さ(45)付近をカバー
		if (mouseX >= x - 150 && mouseX <= x + 850 && mouseY >= y && mouseY <= y + 80) {
			selectedIndex = i;
		}
	}
	if (selectedIndex != previousIndex) {
		AudioManager::GetInstance().PlayOneShot("SelectSkill");
	}

	// --- 強化実行処理 ---
	if (input->IsMouseDown(MOUSE_INPUT_LEFT) || input->IsButtonDown(XINPUT_GAMEPAD_B)) {
		if (!stats.empty() && stats[selectedIndex].level < 50) {

			// コスト計算
			int cost = 3 + (stats[selectedIndex].level * 2);
			
			// コインが足りるかチェック
			if (playerCoins >= cost) {
				// コイン消費
				playerCoins -= cost;
				if (player) player->SetCoinValue(playerCoins);

				// 各ステータスの上昇幅
				int boostValue = 0;
				switch (selectedIndex) {
				case 0: boostValue = 10; break; // HP +10
				case 1: boostValue = 2;  break; // 攻撃 +2
				case 2: boostValue = 1;  break; // 防御 +1
				case 3: boostValue = 1;  break; // 会心率 +1
				case 4: boostValue = 5;  break; // 会心ダメ +5
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
					case 1: // 攻撃力
						// 1. 基礎攻撃力を更新（永続保存用）
						player->SetBaseAtk(player->GetBaseAtk() + (int)boostValue);

						// 2. 現在の攻撃力（武器込みの数値）にも、上がった分だけを足す
						// これなら武器を装備していても、その数値に上乗せされるだけなので消えません！
						player->SetAtk(player->GetAtk() + (int)boostValue);
						break;
					case 2: // 防御力
						player->SetDef(player->GetDef() + (float)boostValue);
						break;
					case 3: // 会心率
						player->SetCriticalHitRate(player->GetCriticalHitRate() + (float)boostValue);
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

	// --- 中央揃えのための計算 ---
	int centerX = WINDOW_WIDTH / 2;
	int gaugeStartX = centerX - 370;
	int baseY = 180;

	// 背景パネル
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(5, 5, 10), TRUE);

	// タイトル (中央寄せ)
	unsigned int titleColor = allMax ? GetColor(255, 255, 255) : GetColor(200, 200, 200);
	DrawString(centerX - 150, 60, "STATUS ENHANCEMENT", titleColor);

	// 所持コイン表示 (右上)
	unsigned int coinColor = GetColor(255, 215, 0);
	DrawFormatString(WINDOW_WIDTH - 280, 60, coinColor, "COINS: %d", playerCoins);

	for (int i = 0; i < (int)stats.size(); i++) {
		int x = gaugeStartX;
		int y = baseY + i * 110;

		// 選択カーソル (y方向のオフセット微調整)
		if (i == selectedIndex) {
			int animX = (int)(sin(GetNowCount() / 150.0f) * 5.0f);
			DrawString(x - 175 + animX, y + 15, "->", GetColor(255, 255, 255));
		}

		// 項目名・レベル・ボーナス (ゲージの左側)
		DrawString(x - 140, y + 5, stats[i].name.c_str(), GetColor(255, 255, 255));
		DrawFormatString(x - 140, y + 32, (stats[i].level >= 50 ? GetColor(255, 215, 0) : GetColor(150, 150, 150)), "Lv.%d", stats[i].level);
		DrawFormatString(x - 140, y + 62, GetColor(0, 255, 150), "+%d", stats[i].totalBonus);

		// ゲージ描画 (ここで大きいサイズが適用される)
		DrawParallelGauge(x, y, stats[i].level, stats[i].color);

		// コスト表示 (ゲージが大きくなった分、xのオフセットを 640 -> 750 に拡大)
		if (stats[i].level < 50) {
			int cost = 3 + (stats[i].level * 2);
			unsigned int cCol = (playerCoins >= cost) ? GetColor(255, 255, 255) : GetColor(255, 50, 50);
			DrawFormatString(x + 750, y + 15, cCol, "COST: %d", cost);
		}
		else {
			DrawString(x + 750, y + 15, "MAX", GetColor(255, 215, 0));
		}
	}

	if (allMax) {
		DrawString(centerX - 200, WINDOW_HEIGHT - 80, "ULTIMATE RAINBOW ACHIEVED", GetColor(255, 255, 255));
	}

	int StartX = (WINDOW_WIDTH / 2) - 200;
	int StartY = (WINDOW_HEIGHT)-100;
	int GoalX = (WINDOW_WIDTH / 2) + 200;
	int GoalY = (WINDOW_HEIGHT)-50;
	int textX = StartX + 80;
	int textY = StartY + 17;

	DrawBox(StartX, StartY, GoalX, GoalY, gray, TRUE);
	DrawBox(StartX + 2, StartY + 2, GoalX - 2, GoalY - 2, white, FALSE);
	DrawFormatString(textX + 50, textY, black, "キー/ ボタン:閉じる");
	DrawFormatString(textX + 20, textY, white, "ESC");
	DrawFormatString(textX + 93, textY, white, "A");

	SetUseLighting(TRUE);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
}

void StatusEnhancement::DrawParallelGauge(int x, int y, int level, unsigned int baseColor) {
	// --- ゲージのサイズ設定（拡大版） ---
	int w = 65;    // 1コマの幅
	int h = 45;    // 1コマの高さ
	int skew = 18; // 斜めの傾き
	int gap = 7;   // コマ同士の隙間

	float systemTime = GetNowCount() / 1000.0f;

	int loopCount = (level - 1) / 10;
	if (level <= 0) loopCount = 0;
	int currentLoopLevel = level % 10;
	if (currentLoopLevel == 0 && level > 0) currentLoopLevel = 10;

	unsigned int loopColors[] = {
		GetColor(180, 180, 180), // 1周目：シルバー
		GetColor(0, 110, 220),   // 2周目：ブルー
		GetColor(220, 180, 0),   // 3周目：イエロー
		GetColor(140, 0, 230),   // 4周目：パープル
		GetColor(200, 0, 50)     // 5周目：レッド
	};

	for (int i = 0; i < 10; i++) {
		float offsetY = allMax ? sin(systemTime * 4.0f - (i * 0.5f)) * 12.0f : 0.0f;
		int curY = y + (int)offsetY;

		unsigned int contentCol;
		unsigned int outerBorder;
		unsigned int innerLight;

		bool isFilled = (i < currentLoopLevel);

		if (allMax) {
			float h_val = systemTime * 3.0f - (i * 0.4f);
			contentCol = GetColor(
				(int)(128 + 127 * sin(h_val)),
				(int)(128 + 127 * sin(h_val + 2.09f)),
				(int)(128 + 127 * sin(h_val + 4.18f))
			);
			outerBorder = GetColor(255, 215, 0);
			innerLight = GetColor(220, 220, 180);
		}
		else {
			if (loopCount >= 1) {
				contentCol = isFilled ? loopColors[loopCount] : loopColors[loopCount - 1];
			}
			else {
				contentCol = isFilled ? loopColors[0] : GetColor(30, 30, 35);
			}

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

		// 四角形の4頂点を計算
		int x1 = x + i * (w + gap) + skew; int y1 = curY;
		int x2 = x1 + w;                   int y2 = curY;
		int x3 = x + i * (w + gap) + w;    int y3 = curY + h;
		int x4 = x + i * (w + gap);        int y4 = curY + h;

		// 塗りつぶし描画
		DrawQuadrangle(x1, y1, x2, y2, x3, y3, x4, y4, contentCol, TRUE);
		// 枠線描画
		DrawQuadrangle(x1, y1, x2, y2, x3, y3, x4, y4, outerBorder, FALSE);

		// 光沢・エフェクト
		if (allMax || loopCount > 0 || isFilled || (i == (level % 10) && level < 50)) {
			float shineIdx = sin(systemTime * 3.5f - (i * 0.4f));
			unsigned int finalLight = innerLight;
			if (shineIdx > 0.85f) finalLight = GetColor(255, 255, 255);
			DrawQuadrangle(x1 + 2, y1 + 2, x2 - 1, y2 + 2, x3 - 2, y3 - 2, x4 + 1, y4 - 2, finalLight, FALSE);
		}
	}
}

void StatusEnhancement::SaveTo(BinaryWriter& w) {
	// g_instance がない場合は空データを書いておく（互換性維持）
	if (!g_instance) {
		uint32_t zero = 0;
		w.WritePOD(zero);
		// allMax, selectedIndex を書いておく
		bool am = false;
		int si = 0;
		w.WritePOD(am);
		w.WritePOD(si);
		return;
	}

	// stats の数
	uint32_t count = static_cast<uint32_t>(g_instance->stats.size());
	w.WritePOD(count);
	for (const auto& s : g_instance->stats) {
		w.WriteString(s.name);
		w.WritePOD(s.level);
		w.WritePOD(s.totalBonus);
		w.WritePOD(s.color);
	}
	w.WritePOD(g_instance->allMax);
	w.WritePOD(g_instance->selectedIndex);
}

void StatusEnhancement::LoadFrom(BinaryReader& r, uint32_t ver) {
	// バージョン分岐が必要になったら ver を使って対応する
	uint32_t count = 0;
	r.ReadPOD(count);
	g_instance->stats.clear();
	for (uint32_t i = 0; i < count; ++i) {
		StatData sd;
		sd.name = r.ReadString();
		r.ReadPOD(sd.level);
		r.ReadPOD(sd.totalBonus);
		r.ReadPOD(sd.color);
		g_instance->stats.push_back(sd);
	}
	r.ReadPOD(g_instance->allMax);
	r.ReadPOD(g_instance->selectedIndex);

	// 確保されているデータを元に必要なら Player へ反映（HPやATK等）
	Player* player = Player::GetInstance();
	if (player && !g_instance->stats.empty()) {
		// HP
		player->SetMaxHp(100 + g_instance->stats[0].totalBonus);
		player->SetHp(player->GetMaxHp());
		// 攻撃
		player->SetAtk(10 + g_instance->stats[1].totalBonus);
		// 防御
		player->SetDef(g_instance->stats[2].totalBonus);
		// 会心率
		player->SetCriticalHitRate((float)(5 + g_instance->stats[3].totalBonus));
		// 会心ダメ
		player->SetCriticalDamage(1.5f + (g_instance->stats[4].totalBonus / 100.0f));
	}
}

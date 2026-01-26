#include "ArtifactSelectUI.h"
#include"../../Manager/InputManager.h"
#include"../../Definition.h"
#include "../../Manager/AudioManager.h"

void ArtifactSelectUI::StartSelection() {
	//真ん中を0にするため
	selectIndex = 0;
	isActive = true;
	// 出現演出初期化
	animTimer = 0.0f;
	isAppearing = true;


	// 花火初期化
	fireworks.clear();
	for (int i = 0; i < 5; ++i) {
		FireworkParticle fw;
		fw.x = 300 + i * 250; // スキルカードの下あたり
		fw.y = 600;           // 画面下
		fw.vx = 0;
		fw.vy = -6.0f - i;    // 上方向へ打ち上げ
		fw.color = GetColor(255, 200 - i * 30, 100 + i * 20);
		fw.life = 0;
		fw.maxLife = 60 + i * 10;
		fw.exploded = false;
		fireworks.push_back(fw);
	}
	AudioManager::GetInstance().Load("Res/SE/決定ボタンを押す2.mp3", "SelectSkill", false);
	AudioManager::GetInstance().Load("Res/SE/決定ボタンを押す38.mp3", "DecisionSkill", false);
}

int ArtifactSelectUI::UpdateSelection(const std::vector < std::shared_ptr<ArtifactBase>>& artifact) {
	InputManager* input = &InputManager::GetInstance();
	const int artifactCount = (int)artifact.size();  // ← ※適切な変数に置き換えてください
	if (!isActive) return -1;

	// 出現アニメ中は無効
	if (isAppearing)
	{
		animTimer++;
		if (animTimer >= animDuration)
		{
			animTimer = animDuration;
			isAppearing = false;
		}
		return -1;
	}
	//-------------------------------------
	// ▼ マウスによるカード選択（NEW）
	//-------------------------------------
	int mx, my;
	GetMousePoint(&mx, &my);

	hoverIndex = -1;


	const int startX = 200;
	const int startY = 180;
	const int gap = 300;

	// 出現アニメ補間（Render と同じ計算）
	float t = animTimer / animDuration;
	if (t > 1.0f) t = 1.0f;
	float easeOut = 1 - pow(1 - t, 3);
	float scale = 0.5f + easeOut * 0.5f;
	float offsetY = -200 + easeOut * 200;

	//マウス選択
	for (int i = 0; i < artifactCount; i++)
	{

		const int centerX = 640; // 画面中央（解像度1280x720の場合）
		const int centerY = 180;
		// index = 0 を中央、1を左、2を右にする
		int offsetX = 0;
		if (i == 0) offsetX = 0;
		else if (i == 1) offsetX = -gap;
		else if (i == 2) offsetX = gap;

		int x = centerX + offsetX - cardWidth / 2;
		int y = centerY;
		
		int cx = x + cardWidth / 2;
		int cy = y + cardHeight / 2;
		int halfW = (int)(cardWidth * 0.5f * scale);
		int halfH = (int)(cardHeight * 0.5f * scale);

		int left = cx - halfW;
		int right = cx + halfW;
		int top = cy - halfH;
		int bottom = cy + halfH;

		if (mx >= left && mx <= right && my >= top && my <= bottom)
		{
			hoverIndex = i;
		}
	}

	// ★ マウスがカードの上にある場合、選択カーソルを移動
	if (hoverIndex != -1)
	{
		if (selectIndex != hoverIndex)
		{
			selectIndex = hoverIndex;
			AudioManager::GetInstance().PlayOneShot("SelectSkill");
		}

		// ★ 左クリックで決定
		if (input->IsMouseDown(MOUSE_INPUT_LEFT))
		{
			isActive = false;
			AudioManager::GetInstance().PlayOneShot("DecisionSkill");
			SetMouseDispFlag(FALSE);
			return selectIndex;
		}
	}

	// 出現演出中は入力無効
	if (isAppearing)
	{
		animTimer++;
		if (animTimer >= animDuration)
		{
			animTimer = animDuration;
			isAppearing = false;
		}
		return -1;
	}

	if (inputCooldown > 0) {
		inputCooldown--;
	}
	// ▼ カーソル操作はアーティファクトが2つ以上あるときのみ
	else if (artifactCount > 1 && inputCooldown<=0) 
	{
		// 左キー → 左回り (0 -> 1 -> 2 -> 0)
		if (input->IsKeyDown(KEY_INPUT_LEFT) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)) {
			selectIndex = (selectIndex + 1) % artifactCount;
			inputCooldown = 10;
			AudioManager::GetInstance().PlayOneShot("SelectSkill");
		}
		// 右キー → 右回り
		else if (input->IsKeyDown(KEY_INPUT_RIGHT) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) {
			selectIndex = (selectIndex + artifactCount - 1) % artifactCount;
			inputCooldown = 10;
			AudioManager::GetInstance().PlayOneShot("SelectSkill");
		}
	}

	// ▼ 決定
	if (input->IsKeyDown(KEY_INPUT_RETURN) || input->IsButtonUp(XINPUT_GAMEPAD_B)) {
		isActive = false;
		AudioManager::GetInstance().PlayOneShot("DecisionSkill");
		SetMouseDispFlag(FALSE);

		return selectIndex;
	}

	return -1;
}

void ArtifactSelectUI::Render(const std::vector<std::shared_ptr<ArtifactBase>>& artifact) {
	if (!isActive) return;
	if (artifact.empty()) return;

	const int centerX = 640; // 画面中央（解像度1280x720の場合）
	const int centerY = 180;
	const int gap = 300;

	// 出現アニメ補間
	float t = animTimer / animDuration;
	if (t > 1.0f) t = 1.0f;
	float easeOut = 1 - pow(1 - t, 3);
	float scale = 0.5f + easeOut * 0.5f;
	float offsetY = -200 + easeOut * 200;
	int alpha = (int)(easeOut * 255);

	for (int i = 0; i < (int)artifact.size(); ++i) {
		// index = 0 を中央、1を左、2を右にする
		int offsetX = 0;
		if (i == 0) offsetX = 0;
		else if (i == 1) offsetX = -gap;
		else if (i == 2) offsetX = gap;

		int x = centerX + offsetX - cardWidth / 2;
		int y = centerY;

		int color = (i == selectIndex) ? yellow : GetColor(50, 50, 50);

		// 背景
		DrawBox(x, y, x + cardWidth, y + cardHeight, GetColor(100, 100, 100), TRUE);

		// 枠線（選択中は黄色）
		DrawBox(x, y, x + cardWidth, y + cardHeight, color, FALSE);

		// アイコン
		int iconHandle = LoadGraph(artifact[i]->GetIconPath().c_str());
		DrawExtendGraph(x + 50, y + 50, x + 210, y + 210, iconHandle, TRUE);
		DeleteGraph(iconHandle);

		// テキスト
		DrawFormatStringToHandle(x + 20, y + 20, GetColor(255, 255, 255), MainFont, artifact[i]->GetName().c_str());
		DrawFormatStringToHandle(x + 20, y + 240, GetColor(255, 255, 255), MainFont, artifact[i]->GetDescription().c_str());
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//-------------------------------------
	//  花火の上限チェック
	//-------------------------------------
	// 花火の継続生成処理
	static int fireworkTimer = 0;
	fireworkTimer++;
	const int MAX_FIREWORK = 250;
	if (fireworks.size() < MAX_FIREWORK) {
		if (fireworkTimer > 40) {
			fireworkTimer = 0;
			FireworkParticle fw;
			fw.x = 300 + GetRand(600);
			fw.y = 600;
			fw.vx = 0;
			fw.vy = -6.0f - GetRand(4);
			fw.color = GetColor(255, 200 - GetRand(50), 100 + GetRand(100));
			fw.life = 0;
			fw.maxLife = 60 + GetRand(30);
			fw.exploded = false;
			fireworks.push_back(fw);
		}
	}


	//-------------------------------------
	//  描画（BlendMode はまとめて1回）
	//-------------------------------------
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	for (auto& fw : fireworks)
	{
		if (!fw.exploded) {
			// 上へ
			fw.y += fw.vy;
			fw.vy += 0.15f;
			DrawCircle((int)fw.x, (int)fw.y, 3, fw.color, TRUE);

			fw.life++;

			// 爆発
			if (fw.life > fw.maxLife / 2 && fireworks.size() < MAX_FIREWORK) {
				fw.exploded = true;

				for (int j = 0; j < 16; ++j) { // 40 → 16 に減らす
					FireworkParticle p;
					p.x = fw.x;
					p.y = fw.y;
					float angle = (float)(DX_PI * 2 * j / 16);
					float s = 2.0f + GetRand(20) / 20.0f;
					p.vx = cosf(angle) * s;
					p.vy = sinf(angle) * s;
					p.color = fw.color;
					p.life = 0;
					p.maxLife = fw.maxLife / 2;
					p.exploded = true;
					fireworks.push_back(p);
				}
			}
		}
		else {
			fw.x += fw.vx;
			fw.y += fw.vy;
			fw.vy += 0.05f;
			fw.life++;

			float a = 1.0f - (float)fw.life / fw.maxLife;
			if (a < 0) a = 0;

			DrawCircle((int)fw.x, (int)fw.y, 2, fw.color, TRUE);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
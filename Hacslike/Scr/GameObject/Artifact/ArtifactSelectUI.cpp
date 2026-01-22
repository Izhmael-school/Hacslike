#include "ArtifactSelectUI.h"
#include"../../Manager/InputManager.h"
#include"../../Definition.h"
#include "../../Manager/AudioManager.h"

void ArtifactSelectUI::StartSelection() {
	selectIndex = 0;
	isActive = true;
	AudioManager::GetInstance().Load("Res/SE/決定ボタンを押す2.mp3", "SelectSkill", false);
	AudioManager::GetInstance().Load("Res/SE/決定ボタンを押す38.mp3", "DecisionSkill", false);
}

int ArtifactSelectUI::UpdateSelection(const std::vector < std::shared_ptr<ArtifactBase>>& artifact) {
	InputManager* input = &InputManager::GetInstance();

	if (!isActive) return -1;

	const int artifactCount = (int)artifact.size();  // ← ※適切な変数に置き換えてください

	// ▼ カーソル操作はアーティファクトが2つ以上あるときのみ
	if (artifactCount > 1) {
		// 左キー → 左回り (0 -> 1 -> 2 -> 0)
		if (input->IsKeyDown(KEY_INPUT_LEFT) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)) {
			selectIndex = (selectIndex + 1) % artifactCount;
			WaitTimer(150);
			AudioManager::GetInstance().PlayOneShot("SelectSkill");
		}
		// 右キー → 右回り
		else if (input->IsKeyDown(KEY_INPUT_RIGHT) || input->IsButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) {
			selectIndex = (selectIndex + artifactCount - 1) % artifactCount;
			WaitTimer(150);
			AudioManager::GetInstance().PlayOneShot("SelectSkill");
		}
	}

	// ▼ 決定
	if (input->IsKeyDown(KEY_INPUT_RETURN) || input->IsButtonUp(XINPUT_GAMEPAD_B)) {
		isActive = false;
		AudioManager::GetInstance().PlayOneShot("DecisionSkill");
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

}
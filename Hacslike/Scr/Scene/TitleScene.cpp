#include "TitleScene.h"
#include "../Manager/SceneManager.h"
#include "../Manager/AudioManager.h"

TitleScene::TitleScene()
	:teamLogoHandle(-1) {
	Start();
}

TitleScene::~TitleScene() {
	DeleteGraph(teamLogoHandle);
}

void TitleScene::Start() {
	AudioManager::GetInstance().Load("Res/Audio/BGM/Title/TitleBGM.mp3", "Title", false);
	teamLogoHandle = LoadGraph("Res/Logo/TeamLogo.png");
}

void TitleScene::Update() {
	if (InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_A) || InputManager::GetInstance().IsMouseDown(MOUSE_INPUT_LEFT)) {

		SceneManager::GetInstance().ChangeScene(SceneType::Sekino);
	}
}

void TitleScene::Render() {
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, black, true);
	DrawString(600, 200, "Hacslike", red);
	DrawString(600, 600, "左クリックかAボタン", red);
	DrawExtendGraph(WINDOW_WIDTH - 150, WINDOW_HEIGHT - 60, WINDOW_WIDTH - 10, WINDOW_HEIGHT - 10, teamLogoHandle, true);
}

void TitleScene::Setup() {
	AudioManager::GetInstance().PlayBGM("Title");
}

void TitleScene::Teardown() {
	AudioManager::GetInstance().Stop("Title");
}

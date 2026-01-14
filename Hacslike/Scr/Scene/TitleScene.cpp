#include "TitleScene.h"
#include "../Manager/SceneManager.h"
#include "../Manager/AudioManager.h"
#include "../Manager/SaveManager.h"

TitleScene::TitleScene() 
{
	Start();
}

TitleScene::~TitleScene() {}

void TitleScene::Start() {
	AudioManager::GetInstance().Load("Res/Audio/BGM/Title/TitleBGM.mp3", "Title", false);
	SaveManager::GetInstance().RegisterSavers();
}

void TitleScene::Update() {
	if (InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_A) || InputManager::GetInstance().IsMouseDown(MOUSE_INPUT_LEFT)) {

		SceneManager::GetInstance().ChangeScene(SceneType::Game);
	}
}

void TitleScene::Render() {
	DrawString(600,200,"Hacslike",red);
	DrawString(600, 600, "左クリックかAボタン",red);
}

void TitleScene::Setup() {
	AudioManager::GetInstance().PlayBGM("Title");
}

void TitleScene::Teardown() {
	AudioManager::GetInstance().Stop("Title");
}

#include "TitleScene.h"
#include "../Manager/SceneManager.h"
#include "../Manager/AudioManager.h"

TitleScene::TitleScene() 
{
	Start();
}

TitleScene::~TitleScene() {}

void TitleScene::Start() {
	AudioManager::GetInstance().Load("Res/Audio/BGM/Title/TitleBGM.mp3", "Title", false);
	AudioManager::GetInstance().PlayBGM("Title");
}

void TitleScene::Update() {
	if (InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_A) || InputManager::GetInstance().IsMouseDown(MOUSE_INPUT_LEFT)) {
		AudioManager::GetInstance().Stop("Title");
		SceneManager::GetInstance().SetNext(SceneType::Game);
	}
}

void TitleScene::Render() {
	DrawString(600,400,"Hacslike",black);
}

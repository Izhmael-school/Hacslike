#include "TitleScene.h"
#include "../Manager/SceneManager.h"

TitleScene::TitleScene() 
{
	Start();
}

TitleScene::~TitleScene() {}

void TitleScene::Start() {}

void TitleScene::Update() {
	if (InputManager::GetInstance().IsButtonDown(XINPUT_GAMEPAD_A) || InputManager::GetInstance().IsMouseDown(MOUSE_INPUT_LEFT))
		SceneManager::GetInstance().SetNext(SceneType::Game);
}

void TitleScene::Render() {
	DrawString(600,400,"Hacslike",black);
}

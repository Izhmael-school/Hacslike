#include "TitleScene.h"
#include "../Manager/SceneManager.h"
#include "../Manager/AudioManager.h"
#include "../Manager/SaveManager.h"
#include "../GameObject/Item/ItemFactory.h"

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
	//アイテムのセット
	ItemFactory::Instance().InitializeDefaultItems();
}

void TitleScene::Teardown() {
	AudioManager::GetInstance().Stop("Title");
}

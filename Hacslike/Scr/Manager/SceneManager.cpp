#include "SceneManager.h"
#include "../Manager/FadeManager.h"
#include "../Scene/SekinoScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/TitleScene.h"

SceneManager::SceneManager() 
	:pCurrentScene(nullptr)
	,next(SceneType::Title)
	,Current((SceneType)INVALID)
	,changed(false)
{
	switch (next) {
	case SceneType::Title:
		pCurrentScene = new TitleScene();
		break;

	case SceneType::Game:
		pCurrentScene = new GameScene();
		break;

	default:
		pCurrentScene = nullptr;
		break;
	}

	Current = next;
}


void SceneManager::Update() {

	//if (FadeManager::GetInstance().GetFadeState() != FadeState::FadeEnd) return;

	// シーンの切り替えがあれば遷移を行う
	if (Current != next)
		LoadScene();

	if (pCurrentScene == nullptr)
		return;

	pCurrentScene->Update();
}

void SceneManager::Render() {
	if (pCurrentScene == nullptr)
		return;

	pCurrentScene->Render();
}

void SceneManager::LoadScene() {
	// 現在のシーン番号を書き換える
	Current = next;

	delete pCurrentScene;
	pCurrentScene = nullptr;

	switch (next) {
	case SceneType::Title:
		pCurrentScene = new TitleScene();
		break;

	case SceneType::Game:
		pCurrentScene = new GameScene();
		break;
	default:
		pCurrentScene = nullptr;
		break;
	}

}

void SceneManager::SetNext(SceneType _next) {
	next = _next;
	//changed = true;

	//FadeManager::GetInstance()->FadeIn();
}

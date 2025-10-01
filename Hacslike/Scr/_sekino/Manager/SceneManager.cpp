#include "SceneManager.h"
#include "../Scene/GameScene.h"
#include "../Scene/TitleScene.h"
#include "../Scene/ClearScene.h"
#include "../Scene/HowToScene.h"
#include "../Manager/FadeManager.h"

// 静的メンバ変数の初期化
SceneManager* SceneManager::pInstance = nullptr;

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

void SceneManager::CreateInstance() {
	pInstance = new SceneManager();
}

SceneManager* SceneManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

void SceneManager::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

void SceneManager::Update() {
	

	//// シーンの切り替えの最初の1回だけフェード処理を行う
	//if (changed) {
	//	changed = false;
	//	FadeManager::GetInstance()->FadeOut();
	//}

	// シーンの切り替えがあれば遷移を行う
	if (Current != next)
		LoadScene();

	if (pCurrentScene == nullptr)
		return;

	pCurrentScene->Update();
}

void SceneManager::Render() {
	// フェード処理が終了してなければ更新しない
	// ※Fade中にチラ見せさせる場合は記述しない
	if (FadeManager::GetInstance()->GetFadeState() != FadeState::FadeEnd)
		return;

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

	case SceneType::Clear:
		pCurrentScene = new ClearScene();
		break;

	case SceneType::HowTo:
		pCurrentScene = new HowToScene();
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

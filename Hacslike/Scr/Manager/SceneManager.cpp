#include "SceneManager.h"
#include "../Scene/GameScene.h"
#include "../Scene/TitleScene.h"

SceneManager::SceneManager()
	:pCurrentScene(nullptr)
	, pSceneList() {
	Start();
}

SceneManager::~SceneManager() {
	pCurrentScene = nullptr;

	for (auto scene : pSceneList) {
		delete scene;
		scene = nullptr;
	}
}

void SceneManager::Start() {
	pSceneList[(int)SceneType::Title] = new TitleScene();
	pSceneList[(int)SceneType::Game] = new GameScene();
}


void SceneManager::Update() {
	if (pCurrentScene == nullptr)
		return;

	pCurrentScene->Update();
}

void SceneManager::Render() {
	if (pCurrentScene == nullptr)
		return;

	pCurrentScene->Render();
}

void SceneManager::ChangeScene(SceneType _next) {
	FadeManager::GetInstance().FadeOut(0.5f);
	if (pCurrentScene != nullptr)
		pCurrentScene->Teardown();
	pCurrentScene = pSceneList[(int)_next];
	pCurrentScene->Setup();
	FadeManager::GetInstance().FadeIn(0.5f);
}

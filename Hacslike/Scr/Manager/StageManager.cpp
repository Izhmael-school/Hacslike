#include "StageManager.h"

StageManager* StageManager::pInstance = nullptr;

StageManager::StageManager() {
	generator = new StageGenerator();
}

StageManager::~StageManager() 
{
	delete generator;
	generator = nullptr;
}

void StageManager::CreateInstance() {
	pInstance = new StageManager();
}

StageManager* StageManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

void StageManager::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

void StageManager::Update() {
	generator->Update();
}

void StageManager::Render() {
	generator->Render();
}

int StageManager::GetMapData(int x, int y) {
	return generator->map[x][y];
}

void StageManager::GenerateStage() {
	generator->StageGenerate();
	SetObject(Character::player);
}

void StageManager::SetObject(GameObject* obj) {
	generator->SetGameObjectRandomPos(obj);
}



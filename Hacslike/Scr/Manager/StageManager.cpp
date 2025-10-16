#include "StageManager.h"

StageManager* StageManager::pInstance = nullptr;

StageManager::StageManager() {
	generator = new StageGenerator();
}

StageManager::~StageManager() 
{
	delete generator;
	generator = nullptr;

	for (auto t : floorDifTexture) {
		DeleteGraph(t);
	}

	for (auto t : floorNormalTexture) {
		DeleteGraph(t);
	}
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
	generator->ClearStage();

	floorCount++;
	
	ChangeTexture(floorDifTexture[floor(floorCount / textureChangeFloor)], Room);

	generator->StageGenerate();
	SetObject(Character::player);
}

void StageManager::SetObject(GameObject* obj) {
	generator->SetGameObjectRandomPos(obj);
}

void StageManager::ChangeTexture(int textureHandle, ObjectType changeObject) {
	generator->ChangeObjectTexture(textureHandle, changeObject);
}



#include "GameSystem.h"

GameSystem* GameSystem::pInstance = nullptr;

void GameSystem::CreateInstance()
{
	pInstance = new GameSystem();
}

GameSystem* GameSystem::GetInstance()
{
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

void GameSystem::DestroyInstance()
{
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

GameSystem::GameSystem()
	:currentGameStatus(GameStatus::Playing){
}

GameSystem::~GameSystem()
{
}

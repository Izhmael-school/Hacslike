#include "EnemyManager.h"
#include "../GameObject/Character/Enemy/Goblin/EnemyGoblin.h"
#include "../GameObject/Character/Enemy/Enemy.h"

EnemyManager::EnemyManager() 
{
	Start();
}

EnemyManager::~EnemyManager() {
	DeleteAllEnemy();
}

void EnemyManager::Start()
{
	originGoblinMHandle = MV1LoadModel("Res/Model/Enemy/Goblin/goblin.mv1");
	pEnemyArray.clear();
}

void EnemyManager::Update() {
	for (auto e : pEnemyArray) {
		if (e == nullptr) continue;
		e->Update();
	}
}

void EnemyManager::Render() {
	for (auto e : pEnemyArray) {
		if (e == nullptr) continue;
		e->Render();
	}
}

void EnemyManager::SpawnEnemy(EnemyType type,VECTOR pos) {
	switch (type) {
	case Goblin:
		EnemyGoblin* g = new EnemyGoblin();
		pEnemyArray.push_back(g);
		break;
	}

	pEnemyArray[pEnemyArray.size() - 1]->SetPosition(pos);
}

void EnemyManager::DeleteEnemy(Enemy* enemy) {
	for (int i = 0, max = pEnemyArray.size(); i < max; i++) {
		if (pEnemyArray[i] != enemy) continue;

		pEnemyArray.erase(pEnemyArray.begin() + i);
		delete enemy;
		enemy = nullptr;
	}
}

void EnemyManager::DeleteAllEnemy() {
	for (auto e : pEnemyArray) {
		pEnemyArray.erase(pEnemyArray.begin());
		delete e;
		e = nullptr;
	}

	pEnemyArray.clear();
	pEnemyArray.shrink_to_fit();
}


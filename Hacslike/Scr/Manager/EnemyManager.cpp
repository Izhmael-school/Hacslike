#include "EnemyManager.h"
#include "../GameObject/Character/Enemy/Goblin/EnemyGoblin.h"
#include "../GameObject/Character/Enemy/Enemy.h"

EnemyManager::EnemyManager() {
	Start();
}

EnemyManager::~EnemyManager() {
	DeleteAllEnemy();
	MV1DeleteModel(originGoblinMHandle);

	for (auto g : unuseGoblinArray) {
		delete g;
	}
}

void EnemyManager::Start() {
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

void EnemyManager::SpawnEnemy(EnemyType type, VECTOR pos,bool _isBoss) {
	switch (type) {
	case Goblin:
		if (unuseGoblinArray.size() > 0) {
			Enemy* g = UseEnemy(Goblin);
			pEnemyArray.push_back(g);
			break;
		}
		else {
			EnemyGoblin* g = new EnemyGoblin();
			g->SetModelHandleDup(MV1DuplicateModel(originGoblinMHandle));
			pEnemyArray.push_back(g);
			break;
		}
	}

	pEnemyArray[pEnemyArray.size() - 1]->SetPosition(pos);
	pEnemyArray[pEnemyArray.size() - 1]->SetBoss(_isBoss);
}

Enemy* EnemyManager::UseEnemy(EnemyType type) {
	switch (type) {
	case Goblin:
		Enemy* g = unuseGoblinArray[0];
		unuseGoblinArray.erase(unuseGoblinArray.begin());
		return g;
	}
}

void EnemyManager::UnuseEnemy(Enemy* enemy) {
	switch (enemy->GetType()) {
	case Goblin:
		unuseGoblinArray.push_back(enemy);
		break;
	}
}

void EnemyManager::UnuseAllEnemy() {
	while (pEnemyArray.size() > 0) {
		UnuseEnemy(pEnemyArray[0]);
		pEnemyArray.erase(pEnemyArray.begin());
	}
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


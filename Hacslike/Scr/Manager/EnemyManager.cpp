#include "EnemyManager.h"
#include "../GameObject/Character/Enemy/Goblin/EnemyGoblin.h"
#include "../GameObject/Character/Enemy/Spider/EnemySpider.h"
#include "../GameObject/Character/Enemy/Wolf/EnemyWolf.h"
#include "../GameObject/Character/Enemy/Enemy.h"
#include "../GameObject/Character/Enemy/Boss/BossBase.h"
#include "../GameObject/Character/Enemy/Boss/Goblin/BossGoblin.h"
#include "../Manager/AudioManager.h"

EnemyManager::EnemyManager() {
	Start();
}

EnemyManager::~EnemyManager() {
	DeleteAllEnemy();
	MV1DeleteModel(originGoblinMHandle);
	MV1DeleteModel(originSpiderMHandle);
	MV1DeleteModel(originWolfMHandle);

	for (auto g : unuseGoblinArray) {
		delete g;
	}

	for (auto s : unuseSpiderArray) {
		delete s;
	}

	for (auto w : unuseWolfArray) {
		delete w;
	}

	for (auto e : pEnemyArray) {
		delete e;
	}
}

void EnemyManager::Start() {
	originGoblinMHandle = MV1LoadModel("Res/Model/Enemy/Goblin/model.mv1");
	originSpiderMHandle = MV1LoadModel("Res/Model/Enemy/Spider/model.mv1");
	originWolfMHandle = MV1LoadModel("Res/Model/Enemy/Wolf/model.mv1");
	pEnemyArray.clear();

	AudioManager* manager = &AudioManager::GetInstance();
	manager->Load(audioFilePath + "SwordSwing.mp3", "SwordSwing", false);
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

void EnemyManager::SpawnEnemy(EnemyType type, VECTOR pos) {
	Enemy* e = nullptr;

	switch (type) {
	case Goblin:
		if (unuseGoblinArray.size() > 0) {
			e = UseEnemy(Goblin);
			break;
		}
		else {
			e = new EnemyGoblin(MV1DuplicateModel(originGoblinMHandle));
			break;
		}
	case Spider:
		if (unuseSpiderArray.size() > 0) {
			e = UseEnemy(Spider);
			break;
		}
		else {
			e = new EnemySpider(MV1DuplicateModel(originSpiderMHandle));
			break;
		}
	case Wolf:
		if (unuseWolfArray.size() > 0) {
			e = UseEnemy(Wolf);
			break;
		}
		else {
			e = new EnemyWolf(MV1DuplicateModel(originWolfMHandle));
			break;
		}
	}

	pEnemyArray.push_back(e);
	pEnemyArray[pEnemyArray.size() - 1]->SetPosition(pos);
}


void EnemyManager::SpawnBoss(EnemyType type, VECTOR pos) {
	BossBase* boss = nullptr;
	switch (type) {
	case Goblin:
		boss = new BossGoblin();
		break;
	}
	boss->SetAppearPos(pos);
	boss->SetPosition(VGet(pos.x * CellSize, 0, pos.z * CellSize));
	boss->SetVisible(true);
	pEnemyArray.push_back(boss);
}

Enemy* EnemyManager::UseEnemy(EnemyType type) {
	Enemy* e = nullptr;
	switch (type) {
	case Goblin:
		e = unuseGoblinArray[0];
		unuseGoblinArray.erase(unuseGoblinArray.begin());
		break;
	case Spider:
		e = unuseSpiderArray[0];
		unuseSpiderArray.erase(unuseSpiderArray.begin());
		break;

	case Wolf:
		e = unuseWolfArray[0];
		unuseWolfArray.erase(unuseWolfArray.begin());
		break;
	}


	if (e == nullptr) return nullptr;

	CollisionManager::GetInstance().Register(e->GetCollider());
	e->SetVisible(true);
	e->Setup();
	return e;
}

void EnemyManager::UnuseEnemy(Enemy* enemy) {

	enemy->SetVisible(false);
	CollisionManager::GetInstance().UnRegister(enemy->GetCollider());

	switch (enemy->GetType()) {
	case Goblin:
		unuseGoblinArray.push_back(enemy);
		break;
	case Spider:
		unuseSpiderArray.push_back(enemy);
		break;
	case Wolf:
		unuseWolfArray.push_back(enemy);
		break;
	}
}

void EnemyManager::UnuseAllEnemy() {
	while (pEnemyArray.size() > 0) {
		UnuseEnemy(pEnemyArray[0]);
		CollisionManager::GetInstance().UnRegister(pEnemyArray[0]->GetCollider());
		pEnemyArray.erase(pEnemyArray.begin());
	}
}

void EnemyManager::DeleteEnemy(Enemy* enemy) {
	for (int i = 0, max = pEnemyArray.size(); i < max; i++) {
		if (pEnemyArray[i] != enemy) continue;
		CollisionManager::GetInstance().UnRegister(enemy->GetCollider());
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


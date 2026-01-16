#include "EnemyManager.h"
#include "../GameObject/Character/Enemy/Goblin/EnemyGoblin.h"
#include "../GameObject/Character/Enemy/Spider/EnemySpider.h"
#include "../GameObject/Character/Enemy/Wolf/EnemyWolf.h"
#include "../GameObject/Character/Enemy/Troll/EnemyTroll.h"
#include "../GameObject/Character/Enemy/Zombie/EnemyZombie.h"
#include "../GameObject/Character/Enemy/HellHound/EnemyHellHound.h"
#include "../GameObject/Character/Enemy/Ouger/EnemyOuger.h"
#include "../GameObject/Character/Enemy/Ketbleperz/EnemyKetbleperz.h"
#include "../GameObject/Character/Enemy/Durahan/EnemyDurahan.h"
#include "../GameObject/Character/Enemy/HobGoblin/EnemyHobGoblin.h"
#include "../GameObject/Character/Enemy/Enemy.h"
#include "../GameObject/Character/Enemy/Boss/BossBase.h"
#include "../GameObject/Character/Enemy/Boss/Goblin/BossGoblin.h"
#include "../Manager/AudioManager.h"
#include"../Save/SaveIO.h"

EnemyManager::EnemyManager() {
	Start();
}

EnemyManager::~EnemyManager() {
	DeleteAllEnemy();
	MV1DeleteModel(originGoblinMHandle);
	MV1DeleteModel(originSpiderMHandle);
	MV1DeleteModel(originWolfMHandle);
	MV1DeleteModel(originTrollMHandle);
	MV1DeleteModel(originZombieMHandle);
	MV1DeleteModel(originHellHoundMHandle);
	MV1DeleteModel(originOugerMHandle);
	MV1DeleteModel(originKetbleperzMHandle);
	MV1DeleteModel(originDurahanMHandle);
	MV1DeleteModel(originHobGoblinMHandle);

	for (auto g : unuseGoblinArray) {
		delete g;
	}

	for (auto s : unuseSpiderArray) {
		delete s;
	}

	for (auto w : unuseWolfArray) {
		delete w;
	}

	for (auto t : unuseTrollArray) {
		delete t;
	}

	for (auto z : unuseZombieArray) {
		delete z;
	}

	for (auto h : unuseHellHoundArray) {
		delete h;
	}

	for (auto o : unuseOugerArray) {
		delete o;
	}

	for (auto k : unuseKetbleperzArray) {
		delete k;
	}

	for (auto d : unuseDurahanArray) {
		delete d;
	}

	for (auto h : unuseHobGoblinArray) {
		delete h;
	}

	for (auto e : pEnemyArray) {
		delete e;
	}
}

void EnemyManager::Start() {
	originGoblinMHandle = MV1LoadModel("Res/Model/Enemy/Goblin/model.mv1");
	originSpiderMHandle = MV1LoadModel("Res/Model/Enemy/Spider/model.mv1");
	originWolfMHandle = MV1LoadModel("Res/Model/Enemy/Wolf/model.mv1");
	originTrollMHandle = MV1LoadModel("Res/Model/Enemy/Troll/model.mv1");
	originZombieMHandle = MV1LoadModel("Res/Model/Enemy/Zombie/model.mv1");
	originHellHoundMHandle = MV1LoadModel("Res/Model/Enemy/HellHound/model.mv1");
	originOugerMHandle = MV1LoadModel("Res/Model/Enemy/Ouger/model.mv1");
	originKetbleperzMHandle = MV1LoadModel("Res/Model/Enemy/Ketbleperz/model.mv1");
	originDurahanMHandle = MV1LoadModel("Res/Model/Enemy/Durahan/model.mv1");
	originHobGoblinMHandle = MV1LoadModel("Res/Model/Enemy/HobGoblin/model.mv1");

	pEnemyArray.clear();

	AudioManager* manager = &AudioManager::GetInstance();
	manager->Load(audioFilePath + "SwordSwing.mp3", "SwordSwing", false);
	manager->Load(audioFilePath + "Impact.mp3", "Impact", false);
	manager->Load(audioFilePath + "Dawn.mp3", "Dawn", false);
	manager->Load(audioFilePath + "SpiderAttack.mp3", "SpiderAttack", false);
	manager->Load(audioFilePath + "Bite1.mp3", "Bite1", false);
	manager->Load(audioFilePath + "Bite2.mp3", "Bite2", false);
}

void EnemyManager::Update() {
	for (auto e : pEnemyArray) {
		if (e == nullptr || !e->IsVisible()) continue;
		e->Update();
	}
}

void EnemyManager::Render() {
	for (auto e : pEnemyArray) {
		if (e == nullptr) continue;
		e->Render();
	}

	DrawFormatString(100, 300, red, "残敵数:%d体", pEnemyArray.size());
}

void EnemyManager::SpawnEnemy(EnemyType type, VECTOR pos) {
	Enemy* e = nullptr;

	e = UseEnemy(type);

	if (e == nullptr) return;

	pEnemyArray.push_back(e);
	e->Setup();
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
	boss->Setup();
	pEnemyArray.push_back(boss);
}

Enemy* EnemyManager::UseEnemy(EnemyType type) {
	Enemy* e = nullptr;
	std::vector<Enemy*> tmp = { nullptr };
	std::vector<Enemy*>& enemyArray = tmp;

	switch (type) {
	case Goblin:
		enemyArray = unuseGoblinArray;
		if (enemyArray.size() <= 0)
			enemyArray.push_back(new EnemyGoblin(MV1DuplicateModel(originGoblinMHandle)));
		break;
	case Spider:
		enemyArray = unuseSpiderArray;
		if (enemyArray.size() <= 0)
			enemyArray.push_back(new EnemySpider(MV1DuplicateModel(originSpiderMHandle)));
		break;
	case Wolf:
		enemyArray = unuseWolfArray;
		if (enemyArray.size() <= 0)
			enemyArray.push_back(new EnemyWolf(MV1DuplicateModel(originWolfMHandle)));
		break;
	case Troll:
		enemyArray = unuseTrollArray;
		if (enemyArray.size() <= 0)
			enemyArray.push_back(new EnemyTroll(MV1DuplicateModel(originTrollMHandle)));
		break;
	case Zombie:
		enemyArray = unuseZombieArray;
		if (enemyArray.size() <= 0)
			enemyArray.push_back(new EnemyZombie(MV1DuplicateModel(originZombieMHandle)));
		break;
	case HellHound:
		enemyArray = unuseHellHoundArray;
		if (enemyArray.size() <= 0)
			enemyArray.push_back(new EnemyHellHound(MV1DuplicateModel(originHellHoundMHandle)));
		break;
	case Ouger:
		enemyArray = unuseOugerArray;
		if (enemyArray.size() <= 0)
			enemyArray.push_back(new EnemyOuger(MV1DuplicateModel(originOugerMHandle)));
		break;
	case Ketbleperz:
		enemyArray = unuseKetbleperzArray;
		if (enemyArray.size() <= 0)
			enemyArray.push_back(new EnemyKetbleperz(MV1DuplicateModel(originKetbleperzMHandle)));
		break;
	case Durahan:
		enemyArray = unuseDurahanArray;
		if (enemyArray.size() <= 0)
			enemyArray.push_back(new EnemyDurahan(MV1DuplicateModel(originDurahanMHandle)));
		break;
	case HobGoblin:
		enemyArray = unuseHobGoblinArray;
		if (enemyArray.size() <= 0)
			enemyArray.push_back(new EnemyHobGoblin(MV1DuplicateModel(originHobGoblinMHandle)));
		break;
	}

	e = enemyArray[0];
	enemyArray.erase(enemyArray.begin());

	if (e == nullptr) return nullptr;

	CollisionManager::GetInstance().Register(e->GetCollider());
	e->Setup();
	e->SetVisible(true);

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
	case Troll:
		unuseTrollArray.push_back(enemy);
		break;
	case Zombie:
		unuseZombieArray.push_back(enemy);
		break;
	case HellHound:
		unuseHellHoundArray.push_back(enemy);
		break;
	case Ouger:
		unuseOugerArray.push_back(enemy);
		break;
	case Ketbleperz:
		unuseKetbleperzArray.push_back(enemy);
		break;
	case Durahan:
		unuseDurahanArray.push_back(enemy);
		break;
	case HobGoblin:
		unuseHobGoblinArray.push_back(enemy);
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

void EnemyManager::SaveTo(BinaryWriter& w)
{
	// active 敵数
	uint32_t count = static_cast<uint32_t>(pEnemyArray.size());
	w.WritePOD(count);
	for (auto e : pEnemyArray) {
		// 必要な getter を Enemy 側で用意しておくこと（GetType, GetPosition, GetRotationY, GetHP, IsDead）
		uint32_t type = static_cast<uint32_t>(e->GetType());
		w.WritePOD(type);

		VECTOR pos = e->GetPosition();
		w.WritePOD(pos.x);
		w.WritePOD(pos.y);
		w.WritePOD(pos.z);

		

		float hp = e->GetHP();
		w.WritePOD(hp);

		uint8_t dead = e->IsDead() ? 1u : 0u;
		w.WritePOD(dead);
	}
}

void EnemyManager::LoadFrom(BinaryReader& r, uint32_t ver)
{
	// 既存の敵を一旦クリアする
	UnuseAllEnemy();
	DeleteAllEnemy();

	uint32_t count = 0;
	r.ReadPOD(count);
	for (uint32_t i = 0; i < count; ++i) {
		uint32_t type;
		r.ReadPOD(type);

		float x, y, z;
		r.ReadPOD(x); r.ReadPOD(y); r.ReadPOD(z);

		float rotY;
		r.ReadPOD(rotY);

		float hp;
		r.ReadPOD(hp);

		uint8_t dead;
		r.ReadPOD(dead);

		// UseEnemy でプールから取り出して pEnemyArray に push される（UseEnemy が内部で Setup/SetVisible を行う）
		Enemy* e = UseEnemy(static_cast<EnemyType>(type));
		if (!e) continue;

		e->SetPosition(VGet(x, y, z));
		e->SetHP(hp); // Enemy 側で実装

		if (dead) {
			e->SetDeadState(true); // ダメージ配布や経験値付与を発生させない専用処理を作ること
			e->SetVisible(false);
			// 衝突判定なども適切に解除する（必要なら）
			CollisionManager::GetInstance().UnRegister(e->GetCollider());
		}
	}
}


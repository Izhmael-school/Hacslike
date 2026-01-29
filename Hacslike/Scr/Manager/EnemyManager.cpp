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
#include "../GameObject/Character/Enemy/Boss/HellHound/BossHellHound.h"
#include "../GameObject/Character/Enemy/Boss/Ouger/BossOuger.h"
#include "../GameObject/Character/Enemy/Boss/Ketbleperz/BossKetbleperz.h"
#include "../GameObject/Character/Enemy/Boss/Durahan/BossDurahan.h"
#include "../Manager/AudioManager.h"
#include"../Save/SaveIO.h"

EnemyManager::EnemyManager() {
	Start();
}

EnemyManager::~EnemyManager() {

	DeleteAllEnemy();
}

void EnemyManager::Start() {
	AudioManager* manager = &AudioManager::GetInstance();
	manager->Load(audioFilePath + "SwordSwing.mp3", "SwordSwing", false);
	manager->Load(audioFilePath + "Impact.mp3", "Impact", false);
	manager->Load(audioFilePath + "Dawn.mp3", "Dawn", false);
	manager->Load(audioFilePath + "SpiderAttack.mp3", "SpiderAttack", false);
	manager->Load(audioFilePath + "Bite1.mp3", "Bite1", false);
	manager->Load(audioFilePath + "Bite2.mp3", "Bite2", false);
	manager->Load(audioFilePath + "Axe.mp3", "Axe", false);
	manager->Load(audioFilePath + "Punch1.mp3", "Punch1", false);
	manager->Load(audioFilePath + "Punch2.mp3", "Punch2", false);
	manager->Load(audioFilePath + "HeadBang.mp3", "HeadBang", false);

	goblin = new EnemyUtility([this]() {return new EnemyGoblin(MV1DuplicateModel(goblin->originModelHandle)); },MV1LoadModel("Res/Model/Enemy/Goblin/model.mv1"));
	spider = new EnemyUtility([this]() {return new EnemySpider(MV1DuplicateModel(spider->originModelHandle)); },MV1LoadModel("Res/Model/Enemy/Spider/model.mv1"));
	wolf = new EnemyUtility([this]() {return new EnemyWolf(MV1DuplicateModel(wolf->originModelHandle)); },MV1LoadModel("Res/Model/Enemy/Wolf/model.mv1"));
	troll = new EnemyUtility([this]() {return new EnemyTroll(MV1DuplicateModel(troll->originModelHandle)); },MV1LoadModel("Res/Model/Enemy/Troll/model.mv1"));
	zombie = new EnemyUtility([this]() {return new EnemyZombie(MV1DuplicateModel(zombie->originModelHandle)); },MV1LoadModel("Res/Model/Enemy/Zombie/model.mv1"));
	hellhound = new EnemyUtility([this]() {return new EnemyHellHound(MV1DuplicateModel(hellhound->originModelHandle)); },MV1LoadModel("Res/Model/Enemy/HellHound/model.mv1"));
	ouger = new EnemyUtility([this]() {return new EnemyOuger(MV1DuplicateModel(ouger->originModelHandle)); },MV1LoadModel("Res/Model/Enemy/Ouger/model.mv1"));
	ketbleperz = new EnemyUtility([this]() {return new EnemyKetbleperz(MV1DuplicateModel(ketbleperz->originModelHandle)); },MV1LoadModel("Res/Model/Enemy/Ketbleperz/model.mv1"));
	durahan = new EnemyUtility([this]() {return new EnemyDurahan(MV1DuplicateModel(durahan->originModelHandle)); },MV1LoadModel("Res/Model/Enemy/Durahan/model.mv1"));
	hobgoblin = new EnemyUtility([this]() {return new EnemyHobGoblin(MV1DuplicateModel(hobgoblin->originModelHandle)); },MV1LoadModel("Res/Model/Enemy/HobGoblin/model.mv1"));
	
	pUnuseEnemiesArray.push_back(goblin);
	pUnuseEnemiesArray.push_back(spider);
	pUnuseEnemiesArray.push_back(wolf);
	pUnuseEnemiesArray.push_back(troll);
	pUnuseEnemiesArray.push_back(zombie);
	pUnuseEnemiesArray.push_back(hellhound);
	pUnuseEnemiesArray.push_back(ouger);
	pUnuseEnemiesArray.push_back(ketbleperz);
	pUnuseEnemiesArray.push_back(durahan);
	pUnuseEnemiesArray.push_back(hobgoblin);

	EffectManager::GetInstance().Load("Res/Effect/Death.efk", "Dead", 10.0f);
}

void EnemyManager::Update() {
	for (auto e : pEnemyArray) {
		if (e == nullptr || !e->IsVisible()) continue;
		e->Update();
	}

	// pEnemyArrayから安全に消すため
	for (int i = 0; i < unuseEnemy.size();) {
		Enemy* e = unuseEnemy.front();
		pEnemyArray.remove(e);
		unuseEnemy.remove(e);
	}
}

void EnemyManager::Render() {
	for (auto e : pEnemyArray) {
		if (e == nullptr) continue;
		e->Render();
	}

	DrawFormatStringToHandle(100, 300, red, MainFont, "残敵数:%d体", pEnemyArray.size());
}

void EnemyManager::SpawnEnemy(EnemyType type, VECTOR pos) {
	Enemy* e = nullptr;

	e = UseEnemy(type);

	if (e == nullptr) return;

	pEnemyArray.push_back(e);

	e->SetPosition(pos);
}


void EnemyManager::SpawnBoss(EnemyType type, VECTOR pos) {
	BossBase* boss = nullptr;
	switch (type) {
	case Goblin:
		boss = new BossGoblin(pos);
		break;
	case HellHound:
		boss = new BossHellHound(pos);
		break;
	case Ouger:
		boss = new BossOuger(pos);
		break;
	case Ketbleperz:
		boss = new BossKetbleperz(pos);
		break;
	case Durahan:
		boss = new BossDurahan(pos);
		break;
	default:
		return;
	}
	boss->SetPosition(VGet(pos.x * CellSize, 0, pos.z * CellSize));
	boss->SetVisible(true);
	boss->SetType(type);
	pEnemyArray.push_back(boss);
}

Enemy* EnemyManager::UseEnemy(EnemyType type) {
	Enemy* e = nullptr;

	// 指定の敵の未使用が無ければ
	if (pUnuseEnemiesArray[(int)type]->unuseArray.size() == 0) {
		// 敵の生成
		e = pUnuseEnemiesArray[(int)type]->CreateEnemy();
	}
	else {
		e = pUnuseEnemiesArray[(int)type]->unuseArray.front();
		pUnuseEnemiesArray[(int)type]->unuseArray.pop_front();
	}

	CollisionManager::GetInstance().CheckRegister(e->GetCollider());
	e->SetType(type);
	e->Setup();
	return e;
}

void EnemyManager::UnuseEnemy(Enemy* enemy) {
	// 未使用状態化
	enemy->Teardown();

	pUnuseEnemiesArray[(int)enemy->GetType()]->unuseArray.push_back(enemy);

	unuseEnemy.push_back(enemy);
}

void EnemyManager::UnuseAllEnemy() {

	while (pEnemyArray.size() > 0) {
		pEnemyArray.front()->Teardown();
		UnuseEnemy(pEnemyArray.front());
		pEnemyArray.erase(pEnemyArray.begin());
	}
}

void EnemyManager::DeleteEnemy(Enemy* enemy) {
	for (int i = 0, max = pEnemyArray.size(); i < max; i++) {
		CollisionManager::GetInstance().UnRegister(enemy->GetCollider());
		unuseEnemy.push_back(enemy);
		delete enemy;
		enemy = nullptr;
	}
}

void EnemyManager::DeleteAllEnemy() {
	for (auto e : pEnemyArray) {
		pEnemyArray.remove(e);
		if (e == nullptr) continue;
		delete e;
		e = nullptr;
	}

	pEnemyArray.clear();

	for (auto list : pUnuseEnemiesArray) {
		list->DeleteAllEnemy();
	}
}

void EnemyManager::SaveTo(BinaryWriter& w) {
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

		// 追加: 回転（Y）を保存（LoadFrom が期待している順序に合わせる）
		float rotY = e->GetRotationY();
		w.WritePOD(rotY);

		float hp = e->GetHP();
		w.WritePOD(hp);

		uint8_t dead = e->IsDead() ? 1u : 0u;
		w.WritePOD(dead);
	}
}

void EnemyManager::LoadFrom(BinaryReader& r, uint32_t ver) {
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

		// 追加: 回転の復元
		e->SetRotationY(rotY);

		e->SetHP(hp); // Enemy 側で実装

		if (dead) {
			e->SetDeadState(true); // ダメージ配布や経験値付与を発生させない専用処理を作ること
			e->SetVisible(false);
			// 衝突判定なども適切に解除する（必要なら）
			CollisionManager::GetInstance().UnRegister(e->GetCollider());
		}
	}
}


#pragma once
#include "../Component/Singleton.h"
#include <vector>
#include "../Definition.h"

class Enemy;

class EnemyManager : public Singleton<EnemyManager>{
private:

	// ìGÇÃàÍå≥ä«óù
	std::vector<Enemy*> pEnemyArray;
	std::vector<Enemy*> unuseGoblinArray;
	std::vector<Enemy*> unuseSpiderArray;
	std::vector<Enemy*> unuseWolfArray;
	std::vector<Enemy*> unuseTrollArray;	
	std::vector<Enemy*> unuseZombieArray;
	std::vector<Enemy*> unuseHellHoundArray;
	std::vector<Enemy*> unuseOugerArray;

	const std::string audioFilePath = "Res/Audio/SE/Enemy/";

private:
	int originGoblinMHandle;
	int originSpiderMHandle;
	int originWolfMHandle;
	int originTrollMHandle;
	int originZombieMHandle;
	int originHellHoundMHandle;
	int originOugerMHandle;

public:
	EnemyManager();
	~EnemyManager();

	void Start();
	void Update();
	void Render();

	void SpawnEnemy(EnemyType type, VECTOR pos);
	void SpawnBoss(EnemyType type, VECTOR pos);
	Enemy* UseEnemy(EnemyType type);
	void UnuseEnemy(Enemy* enemy);
	void UnuseAllEnemy();

	void DeleteEnemy(Enemy* enemy);
	void DeleteAllEnemy();
};


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
	
	const std::string audioFilePath = "Res/Audio/SE/Enemy/";

private:
	int originGoblinMHandle;
	int originSpiderMHandle;
	int originWolfMHandle;

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
	void RemoveEnemy(Enemy* enemy);

	void DeleteEnemy(Enemy* enemy);
	void DeleteAllEnemy();
};


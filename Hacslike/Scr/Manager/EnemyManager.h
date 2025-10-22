#pragma once
#include "../Component/Singleton.h"
#include <vector>
#include "../Definition.h"

class Enemy;

class EnemyManager : public Singleton<EnemyManager>{
private:

	// “G‚ÌˆêŒ³ŠÇ—
	std::vector<Enemy*> pEnemyArray;

	
public:
	int originGoblinMHandle;
public:
	EnemyManager();
	~EnemyManager();

	void Start();
	void Update();
	void Render();

	void SpawnEnemy(EnemyType type, VECTOR pos);

	void DeleteEnemy(Enemy* enemy);
	void DeleteAllEnemy();
};


#pragma once
#include "../Component/Singleton.h"
#include <vector>
#include "../Definition.h"

class Enemy;

class EnemyManager : public Singleton<EnemyManager>{
private:

	// “G‚ÌˆêŒ³ŠÇ—
	std::vector<Enemy*> pEnemyArray;
	std::vector<Enemy*> unuseGoblinArray;
	
public:
	int originGoblinMHandle;
public:
	EnemyManager();
	~EnemyManager();

	void Start();
	void Update();
	void Render();

	void SpawnEnemy(EnemyType type, VECTOR pos);
	Enemy* UseEnemy(EnemyType type);
	void UnuseEnemy(Enemy* enemy);
	void UnuseAllEnemy();

	void DeleteEnemy(Enemy* enemy);
	void DeleteAllEnemy();
};


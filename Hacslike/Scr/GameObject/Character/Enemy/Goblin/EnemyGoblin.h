#pragma once
#include "../Enemy.h"

class EnemyGoblin : public Enemy {
public:
	EnemyGoblin(int mHandle);
	~EnemyGoblin();

	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;

	void OnTriggerEnter(Collider* _pOther) override;

private: // UŒ‚”»’èŒn	
	class SphereHitBox* attackCol;
	const float spawnColTimeForAttack01 = 12;
	const float deleteColTimeForAttack01 = 15;
};


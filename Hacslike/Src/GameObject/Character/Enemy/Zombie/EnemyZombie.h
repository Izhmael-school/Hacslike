#pragma once
#include "../Enemy.h"
class EnemyZombie : public Enemy {
public:
	EnemyZombie(int mHandle);
	~EnemyZombie();

	void Start() override;
	void Update() override;
	void Render() override;

	void OnTriggerEnter(Collider* _pOther) override;
};


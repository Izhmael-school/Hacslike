#pragma once
#include "../Enemy.h"
class EnemyGoblin : public Enemy {
public:
	EnemyGoblin();
	~EnemyGoblin();

	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;

	void OnTriggerEnter(Collider* _pOther) override;
};


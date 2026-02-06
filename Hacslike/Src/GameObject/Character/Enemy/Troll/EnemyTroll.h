#pragma once
#include "../Enemy.h"
class EnemyTroll : public Enemy {
public:
	EnemyTroll(int modelHandle);
	~EnemyTroll();

	void Start() override;
	void Update() override;
	void Render() override;

public:
	void OnTriggerEnter(Collider* _pOther) override;
};


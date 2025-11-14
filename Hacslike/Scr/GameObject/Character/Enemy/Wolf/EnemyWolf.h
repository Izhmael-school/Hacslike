#pragma once
#include "../Enemy.h"
class EnemyWolf : public Enemy {
public:
	EnemyWolf(int mHandle);
	~EnemyWolf();

public:
	void Start() override;
	void Update() override;
	void Render() override;

public:
	void OnTriggerEnter(Collider* _pOther) override;

};


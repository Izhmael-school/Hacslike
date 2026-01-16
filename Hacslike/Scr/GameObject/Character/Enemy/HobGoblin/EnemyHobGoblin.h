#pragma once
#include "../Enemy.h"
class EnemyHobGoblin : public Enemy {
public:
	EnemyHobGoblin(int mHandle);
	~EnemyHobGoblin();

	void Start() override;
	void Update() override;
	void Render() override;
};


#pragma once
#include "../Enemy.h"
class EnemyDurahan : public Enemy {
public:
	EnemyDurahan(int mHandle);
	~EnemyDurahan() = default;

	void Start() override;
	void Update() override;
	void Render() override;
};


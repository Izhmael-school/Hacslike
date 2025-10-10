#pragma once
#include "../Enemy.h"
class Goblin : public Enemy {
public:
	Goblin();
	~Goblin();

	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;
};


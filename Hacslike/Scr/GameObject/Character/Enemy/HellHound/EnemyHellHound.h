#pragma once
#include "../Enemy.h"
class EnemyHellHound : public Enemy {
public:
	EnemyHellHound(int mHandle);
	~EnemyHellHound();

	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;

	void OnTriggerEnter(Collider* _pOther) override;
};


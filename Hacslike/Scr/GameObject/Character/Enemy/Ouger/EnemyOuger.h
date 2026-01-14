#pragma once
#include "../Enemy.h"
class EnemyOuger : public Enemy {
public:
	EnemyOuger(int mHandle);
	~EnemyOuger();

	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;

	void OnTriggerEnter(Collider* _pOther) override;
};


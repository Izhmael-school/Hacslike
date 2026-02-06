#pragma once
#include "../Enemy.h"
class EnemyKetbleperz : public Enemy {
public:
	EnemyKetbleperz(int mHandle);
	~EnemyKetbleperz();

	void Start() override;
	void Update() override;
	void Render() override;

	void OnTriggerEnter(Collider* _pOther) override;

};


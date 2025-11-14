#pragma once
#include "../Enemy.h"
class EnemySpider : public Enemy {
public:
	EnemySpider(int mHandle);
	~EnemySpider();

public:
	void Start() override;
	void Update() override;
	void Render() override;

public:
	void OnTriggerEnter(Collider* _pOther) override;


};
#pragma once
#include "../Character.h"

class Enemy : public Character {
private:

public:
	Enemy();
	~Enemy();

	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;
	
	virtual void IsDead();
	virtual void Vision();
};


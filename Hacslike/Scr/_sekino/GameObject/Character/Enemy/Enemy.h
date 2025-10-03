#pragma once
#include "../Character.h"

class Enemy : public Character {
protected:
	// Ray系
	float rayLenght;	// レイの長さ
	float rayAngle;		// レイの角度
	int rayCount;			// レイの数
	float raySpan;		// レイが更新される間隔
	float rayTime;			// レイが更新される時間

public:
	Enemy();
	~Enemy();

	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;
	
	virtual void IsDead();
	virtual void Vision();
};


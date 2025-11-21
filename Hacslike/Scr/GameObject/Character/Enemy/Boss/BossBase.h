#pragma once
#include "../Enemy.h"

class BossBase : public Enemy {
protected:
	BossBase();
	~BossBase();

protected:
	void AppearStair();

public:
public:
	void Start() override;
	void Update() override;
	void Render() override;

	void DeadExecute() override;

private:



	// ŠK’i‚ªoŒ»‚·‚éˆÊ’u
	VECTOR appearPos;

public:
	inline void SetAppearPos(VECTOR pos) { appearPos = pos; }
};


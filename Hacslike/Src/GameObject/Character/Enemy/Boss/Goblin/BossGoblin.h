#pragma once
#include "../BossBase.h"
class BossGoblin : public BossBase {
public:
	BossGoblin(VECTOR _appearPos);
	~BossGoblin();

public:
	void Start() override;
	void Update() override;
	void Render() override;
};


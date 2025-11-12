#pragma once
#include "../BossBase.h"
class BossGoblin : public BossBase {
public:
	BossGoblin();
	~BossGoblin();

public:
	void Start() override;
	void Update() override;
	void Render() override;
};


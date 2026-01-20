#pragma once
#include "../BossBase.h"

class BossHellHound : public BossBase {
public:
	BossHellHound();
	~BossHellHound();

	void Start() override;
	void Update() override;
	void Render() override;

};


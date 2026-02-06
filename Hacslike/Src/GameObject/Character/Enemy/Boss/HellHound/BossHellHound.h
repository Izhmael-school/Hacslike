#pragma once
#include "../BossBase.h"

class BossHellHound : public BossBase {
public:
	BossHellHound(VECTOR _appearPos);
	~BossHellHound();

	void Start() override;
	void Update() override;
	void Render() override;

};


#pragma once
#include "../BossBase.h"
class BossOuger : public BossBase {
public:
	BossOuger(VECTOR _appearPos);
	~BossOuger();

	void Start() override;
	void Update() override;
	void Render() override;
};


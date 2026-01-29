#pragma once
#include "../BossBase.h"
class BossKetbleperz : public BossBase {
public:
	BossKetbleperz(VECTOR _appearPos);
	~BossKetbleperz();

	void Start() override;
	void Update() override;
	void Render() override;
};


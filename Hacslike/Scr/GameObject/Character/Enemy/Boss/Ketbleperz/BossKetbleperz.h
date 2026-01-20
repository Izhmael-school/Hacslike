#pragma once
#include "../BossBase.h"
class BossKetbleperz : public BossBase {
public:
	BossKetbleperz();
	~BossKetbleperz();

	void Start() override;
	void Update() override;
	void Render() override;
};


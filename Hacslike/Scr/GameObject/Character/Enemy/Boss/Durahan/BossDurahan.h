#pragma once
#include "../BossBase.h"
class BossDurahan : public BossBase {
public:
	BossDurahan(VECTOR _appearPos);
	~BossDurahan();

public:
	void Start() override;
	void Update() override;
	void Render() override;
};


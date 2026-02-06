#pragma once
#include "../../../GameObject.h"
#include <vector>

class SpiderBullet : public GameObject {
public:
	SpiderBullet(Collider* _pCol , VECTOR _moveDir);
	~SpiderBullet();

	void Start() override;
	void Update() override;
	void Render() override;

private:
	std::vector<VECTOR> webPoint;

	VECTOR moveDir;

	const int pointMax = 50;
public:
	static float maxDis;
private:
	void CreateWeb();
};


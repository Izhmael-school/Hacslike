#include "SpiderBullet.h"
#include "../../../../Component/Collider/SphereHitBox.h"
#include "../../../../CommonModule.h"
#include "../../../../Manager/TimeManager.h"

float SpiderBullet::maxDis = 100.0f;

SpiderBullet::SpiderBullet(Collider* _pCol, VECTOR _moveDir)
	:moveDir(_moveDir)
{
	pCollider = _pCol;
	Start();
}

SpiderBullet::~SpiderBullet()
{
}

void SpiderBullet::Start()
{
	CreateWeb();
}

void SpiderBullet::Update()
{
	GameObject::Update();
	//position += moveDir * TimeManager::GetInstance().deltaTime;
}

void SpiderBullet::Render()
{
	float colorValue = pointMax / 255;
	float c = 0;

	for (int i = 0; i < pointMax; i++) {

		c += colorValue;

		if (i == pointMax - 1) {
			DrawLine3D(webPoint[pointMax - 1], webPoint[0], GetColor(c, c, c));
			return;
		}

		DrawLine3D(webPoint[i], webPoint[i + 1], GetColor(c, c, c));
	}
}

void SpiderBullet::CreateWeb()
{
	webPoint.clear();

	for (int i = 0; i < pointMax; i++) {
		float rx, ry, rz;
		rx = GetRand(maxDis) - (maxDis / 2);
		ry = GetRand(maxDis) - (maxDis / 2);
		rz = GetRand(maxDis) - (maxDis / 2);
		webPoint.push_back(VGet(rx, ry, rz));
	}
}


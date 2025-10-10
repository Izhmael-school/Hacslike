#pragma once
#include "../Hacslike/Scr/_sekino/Component/Collider.h"
#include <vector>
#include <DxLib.h>

class SphereHitBox {
private:
	SphereCollider* pCollider;
	float timer;
	float lifeTime;

public:
	SphereHitBox(GameObject* ownwr, VECTOR _offset,float _radius, float _lifeTime);
	~SphereHitBox();

	void Update();
	void Render();
	bool IsDead() const;


};


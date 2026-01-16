#pragma once
#include <functional>
#include "../../../Character/Character.h"
#include "../../../../Manager/TimeManager.h"

template <class T = void, class... R>
struct AttackAreaObject : public GameObject {
	float maxRadius;
	float radius;
	float limitTime;
	float time;
	float speed;
	std::function<T(R...)> spawnColliderFunction;
	bool useFunc;

	AttackAreaObject(float _rad, float _time,VECTOR pos,float _speed, std::function<T(R...)> spawnFunc)
		:maxRadius(_rad)
		,radius(0)
		,limitTime(_time)
		,spawnColliderFunction(spawnFunc)
		,time(0)
		,speed(0.3f)
		,useFunc(false)
	{
		SetPosition(pos);
	}

public:
	void Start(){}

	void Update() {
		if (useFunc) return;

		GameObject::Update();

		if (time >= limitTime) {
			spawnColliderFunction();
			useFunc = true;
		}
		else {
			time += speed;
			float progress = time / limitTime;
			radius = progress * maxRadius;
		}
	} 

	void Render() {
		if (useFunc) return;

		DrawSphere3D(position, maxRadius, 16, red, red, false);
		
		unsigned int c = GetColor(0, 0, 0);

		if (time / limitTime >= 0.7)
			c = red;
		else if (time / limitTime >= 0.4)
			c = yellow;
		else
			c = green;

		DrawSphere3D(position, radius, 16, c, c, false);

	}
};

class AttackArea{

	Character* owner;

	std::list<AttackAreaObject<void>*> areaObjectArray;

public:
	AttackArea() = default;
	~AttackArea() = default;

	void CreateArea(float _rad, float _time, VECTOR pos,float speed, std::function<void()> spawnFunc);

	void Update();
	void Render();
	void SetOwner(Character* chara) { owner = chara; }
};

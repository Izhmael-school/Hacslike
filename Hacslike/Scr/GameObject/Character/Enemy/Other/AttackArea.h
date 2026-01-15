#pragma once
#include <functional>
#include "../../../GameObject.h"

template <class T = void, class... R>
public struct AttackAreaObject {
	float radius;
	float time;
	std::function<T(R...)> spawnColliderFunction;

	AttackAreaObject(float _rad,float _time, std::function<T(R...)> spawnFunc)
		:radius(_rad)
		,time(_time)
		,spawnColliderFunction(spawnFunc)
	{}
};


class AttackArea : public GameObject{

	std::list<AttackAreaObject> 

	AttackArea();
	~AttackArea() = default;

	void Start() override;
	void Update() override;
	void Render() override;
};



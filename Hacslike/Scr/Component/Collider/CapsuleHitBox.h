#pragma once
#include "../../Component/Collider/Collider.h"
#include <vector>
#include <DxLib.h>

class CapsuleHitBox {
private:
    CapsuleCollider* pCollider;
    float timer;
    float lifeTime;

public:
    CapsuleHitBox(GameObject* owner, VECTOR p1, VECTOR p2, float radius, float lifeTime);
    ~CapsuleHitBox();

    void Update();
    void Render();
    bool IsDead() const;

};
// Slash.h
#pragma once
#include "../GameObject.h"
#include "../../Component/Collider/Collider.h"

class Slash : public GameObject {
private:
    CapsuleCollider* collider;
    VECTOR direction;
    float speed;
    float lifeTime;
    float elapsed;
    bool isDead;

public:
    Slash(VECTOR start, VECTOR dir);
    ~Slash();

    void Start() override;
    void Update();
    void Render();
    bool IsDead() const { return isDead; }
};
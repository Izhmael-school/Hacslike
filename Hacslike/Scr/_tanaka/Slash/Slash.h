// Slash.h
#pragma once
#include "../Hacslike/Scr/_sekino/GameObject/GameObject.h"
#include "../Hacslike/Scr/_sekino/Component/Collider.h"

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
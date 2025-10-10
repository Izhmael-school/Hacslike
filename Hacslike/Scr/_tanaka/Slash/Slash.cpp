#include "Slash.h"
#include "DxLib.h"

Slash::Slash(VECTOR start, VECTOR dir)
    : GameObject(start),
    speed(400.0f),
    lifeTime(100.0f),
    elapsed(0.0f),
    isDead(false) {
    direction = VNorm(dir);

    VECTOR p1 = VZero;
    VECTOR p2 = VGet(0, 0, 2000.0f); // ’·‚³
    float radius = 100.0f;

    collider = new CapsuleCollider(this, p1, p2, radius);
    collider->SetMatrix(MGetTranslate(position));
}

Slash::~Slash() {
    delete collider;
}

void Slash::Start() {
}

void Slash::Update() {
    if (isDead) return;

    elapsed += 1.0f / 60.0f;
    if (elapsed >= lifeTime) {
        isDead = true;
        return;
    }

    // ‘O‚Éi‚Ş
    VECTOR move = VScale(direction, speed * (1.0f / 60.0f));
    position = VAdd(position, move);

    collider->SetMatrix(MGetTranslate(position));
    collider->Update();
}

void Slash::Render() {
    if (isDead) return;
    collider->Render();
}
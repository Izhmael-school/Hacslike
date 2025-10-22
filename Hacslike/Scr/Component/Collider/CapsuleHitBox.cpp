#include "CapsuleHitBox.h"
#include "../Hacslike/Scr/Manager/TimeManager.h"

CapsuleHitBox::CapsuleHitBox(GameObject* owner, const VECTOR& p1, const VECTOR& p2, float radius, float life)
    : pCollider(nullptr)
    , timer(0.0f)
    , lifeTime(life) {
    pCollider = new CapsuleCollider(owner, p1, p2, radius);
    pCollider->SetEnable(true);
}

CapsuleHitBox::~CapsuleHitBox() {
    if (pCollider) {
        delete pCollider;
        pCollider = nullptr;
    }
}

void CapsuleHitBox::Update() {
    timer += TimeManager::GetInstance()->deltaTime;
    if (pCollider && pCollider->IsEnable()) {
        pCollider->Update();
    }
}

void CapsuleHitBox::Render() {
    if (pCollider) {
        pCollider->Render();
    }
}

bool CapsuleHitBox::IsDead() const {
    return timer >= lifeTime;
}
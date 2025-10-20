#include "Player.h"
#include "../../../Manager/FadeManager.h"
#include "../../Camera/Camera.h"
#include "../../../Component/Collider/Collider.h"
#include "../../../Definition.h"
#include "../Hacslike/Scr/Manager/CollisionManager.h"
#include "../Hacslike/Scr/Manager/WeaponManager.h"
#include "../Hacslike/Scr/Manager/TimeManager.h"
#include "../Hacslike/Scr/GameObject/Weapon/Weapon.h"

#include "../Hacslike/Scr/Component/Collider/CapsuleHitBox.h"
#include "../Hacslike/Scr/Component/Collider/SphereHitBox.h"
#include "../Hacslike/Scr/GameObject/Slash/Slash.h"

#include <algorithm>

Player::Player(VECTOR _pos)
    : Character(_pos, "Player", Lv, Exp, speed)
    , pWeapon(nullptr)
    , input(InputManager::GetInstance())
    , isAttacking(false)
    , attackIndex(0)
    , attackTimer(0.0f)
    , canNextAttack(false)
    , isBlinking(false)
    , blinkTimer(0.0f)
    , attackInputCooldown(0.0f)
    , attackButtonPressed(false)
    , evasionButtonPressed(false)
    , evasionCooldown(0.0f)
    , evasionSpeed(1.0f)
    , currentWeaponId(0)
    , maxWeaponId(0)
    , changeWeaponButtonPressed(false) {
    Start();
}

Player::~Player() {
    // CollisionManager に登録している HitBox を全て解除してクリア
    for (auto& h : CapsuleHitboxes) {
        if (h) CollisionManager::GetInstance()->UnRegister(h.get());
    }
    CapsuleHitboxes.clear();

    for (auto& h : SphereHitboxes) {
        if (h) CollisionManager::GetInstance()->UnRegister(h.get());
    }
    SphereHitboxes.clear();

    // slashes は外部登録していない想定なので単にクリア
    slashes.clear();

    // 自分のコライダを登録解除して削除（SetCollider で pCollider を持っている想定）
    if (pCollider != nullptr) {
        CollisionManager::GetInstance()->UnRegister(pCollider);
        delete pCollider;
        pCollider = nullptr;
    }

    // Weapon は unique_ptr のため自動開放
    // モデルハンドルの解放（既存コードを踏襲）
    MV1DeleteModel(PLAYER_MODEL_HANDLE);
}

void Player::Start() {
    if (!isVisible) return;

    // 自身のコライダ生成・登録
    SetCollider(new CapsuleCollider(this, VZero, VScale(VUp, 200), 50.0f));
    CollisionManager::GetInstance()->Register(pCollider);

    modelHandle = PLAYER_MODEL_HANDLE;
    pAnimator->SetModelHandle(modelHandle);
    SetPlayer(this);

    // アニメーション読み込み（インデックス管理は別途 enum 等で管理推奨）
    GetAnimator()->Load("Res/PlayerModel/Neutral.mv1", true);
    GetAnimator()->Load("Res/PlayerModel/Walking.mv1", true);
    GetAnimator()->Load("Res/PlayerModel/Attack1.mv1");
    GetAnimator()->Load("Res/PlayerModel/Attack2.mv1");
    GetAnimator()->Load("Res/PlayerModel/Attack3.mv1");
    GetAnimator()->Load("Res/PlayerModel/Run.mv1", true);
    pAnimator->Play(0);

    // 残像データ初期化
    for (int i = 0; i < AFTIMAGENUM; i++) {
        afterImagePos[i] = position;
        afterImageRotY[i] = rotation.y;
    }

    // 武器初期化
    WeaponManager::GetInstance()->LoadWeapons("Scr/Data/WeaponsData.json");
    maxWeaponId = WeaponManager::GetInstance()->GetMaxWeaponId();
    changeWeaponButtonPressed = false;
    currentWeaponId = 1;
    WeaponData* weaponData = WeaponManager::GetInstance()->GetWeapon(currentWeaponId);
    if (weaponData) {
        pWeapon = std::make_unique<Weapon>(weaponData->name, weaponData->modelHandle);
        pWeapon->SetType((WeaponType)weaponData->type);
        pWeapon->attach(modelHandle, pWeapon->GetModelHandle(), "wp", this);
        // Weapon 側がコライダ管理するならそちらで登録する想定
    }

    SetSpeed(1.0f); // speed は毎秒単位を想定して扱う
}

void Player::Update() {
    if (!isVisible) return;

    inputVec = VZero;

    MoveInput();
    AttackInput();
    EvasionInput();
    UpdateBlink();
    CheckWall();
    UpdateMovement();
    UpdateSlash();

    MV1SetMatrix(modelHandle, matrix);

    WeaponInput();

    pAnimator->Update();
    GameObject::Update();

    // 武器の Update（unique_ptr を直接参照）
    if (pWeapon) {
        pWeapon->Update();
    }

    if (pCollider) {
        pCollider->Update();
    }
}

void Player::Render() {
    if (!isVisible) return;

    // 残像描画
    if (isBlinking && !isAttacking) {
        for (int i = AFTIMAGENUM - 1; i >= 0; i -= 4) {
            int alpha = 255 - 255 * i / AFTIMAGENUM;
            MATRIX matRot = MGetRotY(Deg2Rad(afterImageRotY[i]));
            MATRIX matTrans = MGetTranslate(afterImagePos[i]);
            MATRIX mat = MMult(matRot, matTrans);
            MV1SetMatrix(modelHandle, mat);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
            MV1DrawModel(modelHandle);
        }
    }

    // 通常描画
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    MV1SetMatrix(modelHandle, matrix);
    MV1DrawModel(modelHandle);

    // Slash 描画
    for (const auto& s : slashes) {
        if (s) s->Render();
    }

    // Hitbox（デバッグ）
    for (const auto& h : CapsuleHitboxes) {
        if (h) h->Render();
    }
    for (const auto& h : SphereHitboxes) {
        if (h) h->Render();
    }

    // 武器描画
    if (pWeapon) pWeapon->Render();
}

void Player::UpdateMovement() {
    if (isAttacking) return;

    if (VSquareSize(inputVec) >= 0.01f) {
        VECTOR dir = VNorm(inputVec);
        float dt = TimeManager::GetInstance()->deltaTime;
        VECTOR localMove = VScale(dir, speed * evasionSpeed * dt); // speed = units/sec
        MATRIX mRotY = MGetRotY(Deg2Rad(Camera::main->GetRotation().y));
        VECTOR moveDirection = VTransform(localMove, mRotY);

        // 回転は移動方向の向きに合わせる（XZ平面）
        rotation.y = Rad2Deg(atan2f(moveDirection.x, moveDirection.z)) + 180.0f;

        position = VAdd(position, moveDirection);

        if (evasionSpeed >= 1.2f) {
            pAnimator->Play(5, 0.5f);
        }
        else {
            pAnimator->Play(1, 1.3f);
        }
    }
    else {
        pAnimator->Play(0);
        evasionSpeed = 1.0f;
    }
}

void Player::MoveInput() {
    GetJoypadXInputState(DX_INPUT_PAD1, &XY);

    float ix = 0.0f, iz = 0.0f;
    if (XY.ThumbLY >= 1000 || input->IsKey(KEY_INPUT_W)) iz += 1.0f;
    if (XY.ThumbLY <= -1000 || input->IsKey(KEY_INPUT_S)) iz -= 1.0f;
    if (XY.ThumbLX >= 1000 || input->IsKey(KEY_INPUT_D)) ix += 1.0f;
    if (XY.ThumbLX <= -1000 || input->IsKey(KEY_INPUT_A)) ix -= 1.0f;

    inputVec.x = ix;
    inputVec.z = iz;

    if (input->IsKey(KEY_INPUT_Q)) inputVec = VAdd(inputVec, VUp);
    if (input->IsKey(KEY_INPUT_E)) inputVec = VAdd(inputVec, VDown);
}

void Player::CreateAttackHitbox(float length, float radius) {
    VECTOR forward = VNorm(VGet(
        -sinf(Deg2Rad(rotation.y)),
        0.0f,
        -cosf(Deg2Rad(rotation.y))
    ));

    float life = 0.20f;

    if (attackIndex < 3) {
        VECTOR start = VAdd(position, VScale(forward, 20.0f));
        VECTOR end = VAdd(start, VScale(forward, length));
        auto hit = std::make_unique<CapsuleHitBox>(this, start, end, radius, life);
        CollisionManager::GetInstance()->Register(hit.get());
        CapsuleHitboxes.emplace_back(std::move(hit));
    }
    else {
        // 3段目：周囲攻撃（球）
        life = 0.25f;
        VECTOR center = VAdd(position, VScale(forward, 70.0f));
        center.y += 50.0f;
        auto hit = std::make_unique<SphereHitBox>(this, center, radius, life);
        CollisionManager::GetInstance()->Register(hit.get());
        SphereHitboxes.emplace_back(std::move(hit));
    }
}

void Player::EvasionInput() {
    bool isEvasionButtonDown = input->IsKeyDown(KEY_INPUT_LSHIFT) ||
        InputManager::GetInstance()->IsButtonDown(XINPUT_BUTTON_A) ||
        input->IsMouseDown(MOUSE_INPUT_MIDDLE);

    if (isEvasionButtonDown && !evasionButtonPressed && evasionCooldown <= 0.0f && VSize(inputVec) != 0) {
        evasionButtonPressed = true;
        evasionCooldown = EVASION_COOLDOWN_TIME;
        Evasion();
    }
    else if (!isEvasionButtonDown) {
        evasionButtonPressed = false;
    }

    if (evasionCooldown > 0.0f)
        evasionCooldown -= TimeManager::GetInstance()->deltaTime;
}

void Player::Evasion() {
    if (pCollider) pCollider->SetEnable(false);
    evasionSpeed = 6.0f;
    isBlinking = true;
    blinkTimer = 0.15f;

    for (int i = 0; i < AFTIMAGENUM; i++) {
        afterImagePos[i] = position;
        afterImageRotY[i] = rotation.y;
    }
}

void Player::UpdateBlink() {
    if (isBlinking && !isAttacking) {
        blinkTimer -= TimeManager::GetInstance()->deltaTime;
        if (blinkTimer <= 0.0f) {
            isBlinking = false;
            Dash();
            if (pCollider) pCollider->SetEnable(true);
        }

        afterImagePos[0] = position;
        afterImageRotY[0] = rotation.y;
        for (int i = AFTIMAGENUM - 1; i > 0; --i) {
            afterImagePos[i] = afterImagePos[i - 1];
            afterImageRotY[i] = afterImageRotY[i - 1];
        }
    }
}

void Player::AttackInput() {
    // 注意：演算子優先に注意 ( (Ctrl && MouseLeft) || XButton )
    bool isButtonDown = (input->IsKey(KEY_INPUT_LCONTROL) && input->IsMouseDown(MOUSE_INPUT_LEFT)) ||
        InputManager::GetInstance()->IsButtonDown(XINPUT_BUTTON_X);

    if (isButtonDown && !attackButtonPressed) {
        attackButtonPressed = true;

        if (!isAttacking && !isBlinking) {
            isAttacking = true;
            attackIndex = 1;
            attackTimer = 0.0f;
            canNextAttack = false;
            pAnimator->Play(2); // Attack1
        }
        else if (canNextAttack && attackIndex < 3) {
            attackIndex++;
            attackTimer = 0.0f;
            canNextAttack = false;
            pAnimator->Play(1 + attackIndex); // Attack2/3
        }
    }
    else if (!isButtonDown) {
        attackButtonPressed = false;
    }

    if (isAttacking) {
        attackTimer += TimeManager::GetInstance()->deltaTime;

        if (attackTimer > 0.2f && attackTimer < 0.6f) canNextAttack = true;

        if (attackIndex == 1 && attackTimer > 0.18f && attackTimer < 0.22f)
            CreateAttackHitbox(30.0f, 80.0f);
        if (attackIndex == 2 && attackTimer > 0.22f && attackTimer < 0.28f)
            CreateAttackHitbox(40.0f, 110.0f);
        if (attackIndex == 3 && attackTimer > 0.25f && attackTimer < 0.33f)
            CreateAttackHitbox(0.0f, 150.0f);

        if (attackTimer > 0.8f) {
            isAttacking = false;
            canNextAttack = false;
            attackIndex = 0;
        }
    }

    // ヒットボックス更新（unique_ptr 対応）
    for (auto it = CapsuleHitboxes.begin(); it != CapsuleHitboxes.end();) {
        auto& h = *it;
        h->Update();
        if (h->IsDead()) {
            CollisionManager::GetInstance()->UnRegister(h.get());
            it = CapsuleHitboxes.erase(it);
        }
        else ++it;
    }

    for (auto it = SphereHitboxes.begin(); it != SphereHitboxes.end();) {
        auto& h = *it;
        h->Update();
        if (h->IsDead()) {
            CollisionManager::GetInstance()->UnRegister(h.get());
            it = SphereHitboxes.erase(it);
        }
        else ++it;
    }
}

void Player::UpdateSlash() {
    for (auto it = slashes.begin(); it != slashes.end();) {
        auto& s = *it;
        s->Update();
        if (s->IsDead()) {
            it = slashes.erase(it);
        }
        else ++it;
    }
}

void Player::Dash() {
    evasionSpeed = 1.2f;
}

void Player::ChangeWeapon(int weaponId) {
    WeaponData* weaponData = WeaponManager::GetInstance()->GetWeapon(weaponId);
    if (!weaponData) return;

    pWeapon = std::make_unique<Weapon>(weaponData->name, weaponData->modelHandle);
    pWeapon->SetType((WeaponType)weaponData->type);
    pWeapon->attach(modelHandle, pWeapon->GetModelHandle(), "wp", this);

    // Weapon にコライダをセットする場合、Weapon 側で Register する設計が望ましい
    pWeapon->SetCollider(new CapsuleCollider(pWeapon.get(), VZero, VScale(VDown, 0), 8.0f));
}

void Player::WeaponInput() {
    if (input->IsKeyDown(KEY_INPUT_C) && !changeWeaponButtonPressed) {
        changeWeaponButtonPressed = true;
        currentWeaponId++;
        if (currentWeaponId > maxWeaponId) currentWeaponId = 1;
        ChangeWeapon(currentWeaponId);
    }
    else if (!input->IsKeyDown(KEY_INPUT_C)) {
        changeWeaponButtonPressed = false;
    }
}

void Player::OnTriggerEnter(Collider* _pCol) {
    if (_pCol == nullptr || _pCol->GetGameObject() == nullptr) return;

    if (_pCol->GetGameObject()->GetTag() == "Goblin") {
        FadeManager::GetInstance()->FadeOut();
    }
}

void Player::OnTriggerStay(Collider* _pCol) {
    // 特になし
}

void Player::OnTriggerExit(Collider* _pCol) {
    // 特になし
}
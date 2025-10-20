#pragma once
#include "../GameObject.h"
#include "../Character/Character.h"

class Weapon : public GameObject {
private:
    int modelHandle = -1;          // このWeaponが持つモデル
    int attachModelHandle = -1;    // 武器を装備するキャラクターのモデル
    int attachFrameIndex = -1;     // 装備フレーム番号
    Character* User = nullptr;     // 武器を装備しているキャラクター

    WeaponType type;

public:
    Weapon(const std::string& _tag = "", int handle = -1);

    ~Weapon();

    virtual void Start() override;  
    virtual void Update() override;
    virtual void Render() override;

    // 武器をキャラクターに装備する
    void attach(int& characterModel, int weaponModelHandle, const std::string& frameName, Character* user);

    // ゲッター
    inline int GetModelHandle() const { return modelHandle; }
    inline Character* GetUser() const { return User; }

    inline WeaponType GetType() const { return type; }
    inline void SetType(WeaponType _type) { type = _type; }

        void OnTriggerEnter(Collider* _pCol) override;
        void OnTriggerStay(Collider* _pCol) override;
        void OnTriggerExit(Collider* _pCol) override;
};

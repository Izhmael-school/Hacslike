#include "Weapon.h"
#include "../../Component/Collider/Collider.h"
#include "../Character/Character.h"
#include <DxLib.h>
#include <iostream>

// コンストラクタ
Weapon::Weapon(const std::string& _tag, int handle)
    : GameObject({}, _tag), modelHandle(handle), attachModelHandle(-1), attachFrameIndex(-1), User(nullptr), type() {
}

// デストラクタ
Weapon::~Weapon() {
    if (pCollider) {
        delete pCollider;
        pCollider = nullptr;
    }
}

void Weapon::Start() {

}

// 武器をキャラクターに装備する
void Weapon::attach(int& characterModel, int weaponModelHandle, const std::string& frameName, Character* user) {
    User = user;
    attachModelHandle = characterModel;
    modelHandle = weaponModelHandle;
    attachFrameIndex = MV1SearchFrame(attachModelHandle, frameName.c_str());

    if (attachFrameIndex == -1) {
        return;
    }

    // 初期位置合わせ
    auto matrix = MV1GetFrameLocalWorldMatrix(attachModelHandle, attachFrameIndex);
    MV1SetMatrix(modelHandle, matrix);
}

// 更新処理
void Weapon::Update() {
    if (!isVisible || modelHandle == -1 || User == nullptr)
        return;

    // フレーム行列を取得してモデルに適用
    auto matrix = MV1GetFrameLocalWorldMatrix(attachModelHandle, attachFrameIndex);
    MV1SetMatrix(modelHandle, matrix);

    // コライダー更新
    if (pCollider != nullptr) {
        pCollider->SetMatrix(matrix);
        pCollider->Update();
    }
}

// 描画処理
void Weapon::Render() {
    if (!isVisible || modelHandle == -1)
        return;

    MV1DrawModel(modelHandle);

    // デバッグ用：コライダー描画
     if (pCollider != nullptr) pCollider->Render();
}

//
//void Weapon::attach(int& _characterModel, int& _weaponModel, std::string _framename, Character* _User) {
//	// サンプルでは武器の持ち替えはしないので、
//	// 関数内でメンバ変数の初期化も行う
//	if (User != _User) {
//		User = _User;
//	}
//	// 持たせるキャラクターのモデルハンドルを初期化
//	attachModelHandle = _characterModel;
//	// 武器のモデルハンドルを初期化
//	modelHandle = _weaponModel;
//
//	// 持たせるキャラクターの持たせるフレーム番号を検索する 
//	attachFrameIndex = MV1SearchFrame(attachModelHandle, _framename.c_str());
//	//　検索したフレームの情報(座標、回転、拡縮の行列)を取得する
//	matrix = MV1GetFrameLocalWorldMatrix(attachModelHandle, attachFrameIndex);
//	//　自身の情報(座標、回転、拡縮の行列)を更新する
//	MV1SetMatrix(modelHandle, matrix);
//
//}
//
//void Weapon::attachDup(int& _characterModel, int _weaponDup, std::string _framename, Character* _User) {
//	// サンプルでは武器の持ち替えはしないので、
//	// 関数内でメンバ変数の初期化も行う
//	if (User != _User) {
//		User = _User;
//	}
//	// 持たせるキャラクターのモデルハンドルを初期化
//	attachModelHandle = _characterModel;
//	// 武器のモデルハンドルを初期化
//	modelHandle = _weaponDup;
//
//	// 持たせるキャラクターの持たせるフレーム番号を検索する 
//	attachFrameIndex = MV1SearchFrame(attachModelHandle, _framename.c_str());
//	//　検索したフレームの情報(座標、回転、拡縮の行列)を取得する
//	matrix = MV1GetFrameLocalWorldMatrix(attachModelHandle, attachFrameIndex);
//	//　自身の情報(座標、回転、拡縮の行列)を更新する
//	MV1SetMatrix(modelHandle, matrix);
//}
//
//void Weapon::attach(int& _characterModel, int& _weaponModel, std::string _frameName) {
//	//	サンプルでは武器の持ち替え等は行わないので、
//	//	関数内でメンバ変数の初期化も行う
//
//	//	持たせるキャラクターのモデルハンドルを初期化
//	attachModelHandle = _characterModel;
//	//	武器のモデルハンドルの初期化
//	modelHandle = _weaponModel;
//
//	//	持たせるキャラクターの持たせるフレーム番号を検索する
//	attachFrameIndex = MV1SearchFrame(attachModelHandle, _frameName.c_str());
//	//	検索したフレームの情報 (座標、回転、拡縮 の行列) を取得する
//	matrix = MV1GetFrameLocalWorldMatrix(attachModelHandle, attachFrameIndex);
//	//	自身の情報 (座標、回転、拡縮 の行列) を更新する
//	MV1SetMatrix(modelHandle, matrix);
//}

void Weapon::OnTriggerEnter(Collider* _pCol) {}

void Weapon::OnTriggerStay(Collider* _pCol) {}

void Weapon::OnTriggerExit(Collider* _pCol) {}

#include "Weapon.h"
#include "../../Component/Collider.h"
#include "../Character/Character.h"
#include "../../Manager/EffectManager.h"
#include "../../Manager/AudioManager.h"


Weapon::Weapon(std::string _tag)
	:GameObject({},_tag)
	, modelHandle(INVALID)
	, attachModelHandle(INVALID)
	, attachFrameIndex(INVALID)
{
	Start();
}

Weapon::~Weapon() {
	MV1DeleteModel(modelHandle);
}

void Weapon::Start() {}

void Weapon::Update() {
	// 非表示なら
	if (!isVisible)
		return;


	//　検索したフレームの情報(座標、回転、拡縮の行列)を取得する
	matrix = MV1GetFrameLocalWorldMatrix(attachModelHandle, attachFrameIndex);
	//　自身の情報(座標、回転、拡縮の行列)を更新する
	MV1SetMatrix(modelHandle, matrix);

	if (!GetCollider()->IsEnable()) {
		GetCollider()->SetEnable(true);
	}

	// 当たり判定の更新
	if (pCollider != nullptr) {
		pCollider->SetMatrix(matrix);
		pCollider->Update();
	}

}

void Weapon::Render() {
	// 非表示なら
	if (!isVisible)
		return;

	MV1DrawModel(modelHandle);

	// 当たり判定の描画
	//if (pCollider != nullptr)
	//	pCollider->Render();

}


void Weapon::attach(int& _characterModel, int& _weaponModel, std::string _framename , Character* _User) {
	// サンプルでは武器の持ち替えはしないので、
	// 関数内でメンバ変数の初期化も行う
	if (User != _User) {
		User = _User;
	}
	// 持たせるキャラクターのモデルハンドルを初期化
	attachModelHandle = _characterModel;
	// 武器のモデルハンドルを初期化
	modelHandle = _weaponModel;

	// 持たせるキャラクターの持たせるフレーム番号を検索する 
	attachFrameIndex = MV1SearchFrame(attachModelHandle, _framename.c_str());
	//　検索したフレームの情報(座標、回転、拡縮の行列)を取得する
	matrix = MV1GetFrameLocalWorldMatrix(attachModelHandle, attachFrameIndex);
	//　自身の情報(座標、回転、拡縮の行列)を更新する
	MV1SetMatrix(modelHandle, matrix);

}

void Weapon::attachDup(int& _characterModel, int _weaponDup, std::string _framename, Character* _User) {
	// サンプルでは武器の持ち替えはしないので、
	// 関数内でメンバ変数の初期化も行う
	if (User != _User) {
		User = _User;
	}
	// 持たせるキャラクターのモデルハンドルを初期化
	attachModelHandle = _characterModel;
	// 武器のモデルハンドルを初期化
	modelHandle = _weaponDup;

	// 持たせるキャラクターの持たせるフレーム番号を検索する 
	attachFrameIndex = MV1SearchFrame(attachModelHandle, _framename.c_str());
	//　検索したフレームの情報(座標、回転、拡縮の行列)を取得する
	matrix = MV1GetFrameLocalWorldMatrix(attachModelHandle, attachFrameIndex);
	//　自身の情報(座標、回転、拡縮の行列)を更新する
	MV1SetMatrix(modelHandle, matrix);
}

void Weapon::OnTriggerEnter(Collider* _pCol) {}

void Weapon::OnTriggerStay(Collider* _pCol) {}

void Weapon::OnTriggerExit(Collider* _pCol) {}

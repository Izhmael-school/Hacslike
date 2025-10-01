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
	, cardinfo()
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

void Weapon::OnTriggerEnter(Collider* _pCol) {
	// プレイヤー側
	// 当たったのがゴブリンでかつ攻撃したやつの攻撃フラグが立ってるかつ攻撃したやつと違うやつ
	if (_pCol->GetGameObject()->GetTag() == "Goblin" && this->GetUser()->isAttack() && this->GetLayer() != _pCol->GetGameObject()->GetLayer()) {

		// 防御フラグが立ってるか
		if (_pCol->GetCharacter()->isBlocking()) {
			AudioManager::GetInstance()->PlayOneShot("Guard");
			_pCol->GetCharacter()->AddHp((this->GetUser()->GetAtk() + this->GetUser()->GetpBuff()->GetBuff(0)) - _pCol->GetCharacter()->GetDef());
			EffectManager::GetInstance()->Instantiate("Guard", this->GetUser()->GetPosition());
		}
		else {
			_pCol->GetCharacter()->AddHp((this->GetUser()->GetAtk() + this->GetUser()->GetpBuff()->GetBuff(0)));
		}

		if (this->GetUser()->GetHand() != nullptr && this != nullptr) {
			this->GetUser()->GetHand()->Hit(this->GetUser(),this->GetUser()->GetHand()->GetNowUseCard());
		}

		if (_pCol->GetCharacter()->GetHp() <= 0) {
			this->GetUser()->AddExp(_pCol->GetCharacter()->GetExp());
		}
		//if (_pCol->GetCharacter()->GetHand() != nullptr && this != nullptr) {
		//	_pCol->GetCharacter()->GetHand()->Hit(_pCol->GetCharacter(), _pCol->GetCharacter()->GetHand()->GetNowUseCard()->GetAbirity());
		//}

		if (this->GetUser()->GetHand()->GetCardPool() == "Durandal") {
			EffectManager::GetInstance()->Instantiate("sHit",_pCol->GetGameObject()->GetPosition());
		}
		else if (this->GetUser()->GetHand()->GetCardPool() == "Mase") {
			EffectManager::GetInstance()->Instantiate("mHit", _pCol->GetGameObject()->GetPosition());
		}
	}

	//　敵側
	// 当たったのがプレイヤーでかつ攻撃したやつの攻撃フラグが立ってるかつ攻撃したやつと違うやつ
	if (_pCol->GetGameObject()->GetTag() == "Player" && this->GetUser()->isAttack() && this->GetLayer() != _pCol->GetGameObject()->GetLayer()) {

		// 防御フラグが立ってるか
		if (_pCol->GetCharacter()->isBlocking()) {
			AudioManager::GetInstance()->PlayOneShot("Guard");
			_pCol->GetCharacter()->AddHp(this->GetUser()->GetAtk() - _pCol->GetCharacter()->GetDef());
			EffectManager::GetInstance()->Instantiate("Guard", this->GetUser()->GetPosition());
		}
		else {
			_pCol->GetCharacter()->AddHp(this->GetUser()->GetAtk());
			AudioManager::GetInstance()->PlayOneShot("Damage");
		}


		/*if (this->GetUser()->GetHand() != nullptr && this != nullptr) {
			this->GetUser()->GetHand()->Hit(this->GetUser(), this->GetUser()->GetHand()->GetNowUseCard()->GetAbirity());
		}*/

		if (_pCol->GetCharacter()->GetHand() != nullptr && this != nullptr && _pCol->GetCharacter()->isBlocking()) {
			_pCol->GetCharacter()->GetHand()->Hit(_pCol->GetCharacter(), _pCol->GetCharacter()->GetHand()->GetNowUseCard());
		}

		EffectManager::GetInstance()->Instantiate("eHit", _pCol->GetGameObject()->GetPosition());
		AudioManager::GetInstance()->PlayOneShot("GW");
	}
}

void Weapon::OnTriggerStay(Collider* _pCol) {}

void Weapon::OnTriggerExit(Collider* _pCol) {}

#include "ItemHeal.h"
#include <DxLib.h>   // DxLibキー入力を使う場合
#include <iostream>
#include"../../Character/Player/Player.h"
#include "../../../Manager/TimeManager.h"
#include "../../../Manager/AudioManager.h"
#include"../../../Component/Collider/SphereHitBox.h"
#include"../../../Component/Collider/CapsuleHitBox.h"
#include"../../../Manager/EffectManager.h"


ItemHeal::ItemHeal(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue)
	:ItemBase(VZero, "item","" , _name, _desc, "Consumable", _value, _effectValue, "Res/ItemIcon/potion.png")
	, healValue(_effectValue) {
}

void ItemHeal::Start()
{
}

void ItemHeal::Render()
{
}

void ItemHeal::Use()
{

	Character::player->AddHp(healValue);
}
void ItemHeal::SaveTo(BinaryWriter& w)
{
}
void ItemHeal::LoadFrom(BinaryReader& r)
{
}
#pragma region 回復系
/// <summary>
/// 回復（小）
/// </summary>
/// <param name="_pos"></param>
/// <param name="_name"></param>
/// <param name="_desc"></param>
/// <param name="_value"></param>
/// <param name="_effectValue"></param>
SmallHealItem::SmallHealItem(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue)
	:ItemBase(VZero, "item","Potion_Small", _name, _desc, "Consumable", _value, _effectValue, "Res/ItemIcon/potion.png")
	, healValue(_effectValue) {
	Start();
}

void SmallHealItem::Start()
{
	isEffectFinished = false;
	AudioManager::GetInstance().Load("Res/SE/ゲージ回復3.mp3", "Heal", false);
}

void SmallHealItem::Update()
{
	
}

void SmallHealItem::Render()
{
}

void SmallHealItem::SaveTo(BinaryWriter& w)
{
}

void SmallHealItem::LoadFrom(BinaryReader& r)
{
}

void SmallHealItem::Use()
{
	Character::player->AddHp(healValue);
	if(Character::player->GetHp()>= Character::player->GetMaxHp()){
		Character::player->SetHp(Character::player->GetMaxHp());
	}
	AudioManager::GetInstance().PlayOneShot("Heal");
	isEffectFinished = true;  // ★Playerが削除できるように
}



/// <summary>
/// 回復（中）
/// </summary>
/// <param name="_pos"></param>
/// <param name="_name"></param>
/// <param name="_desc"></param>
/// <param name="_value"></param>
/// <param name="_effectValue"></param>
MiddleHealItem::MiddleHealItem(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue)
	:ItemBase(VZero, "item","Potion_Middle", _name, _desc, "Consumable", _value, _effectValue, "Res/ItemIcon/potion.png")
	, healValue(_effectValue) {
	Start();
}

void MiddleHealItem::Start()
{
	isEffectFinished = false;
	AudioManager::GetInstance().Load("Res/SE/ゲージ回復3.mp3", "Heal", false);

}

void MiddleHealItem::Render()
{
}

void MiddleHealItem::Update()
{
	
}

void MiddleHealItem::SaveTo(BinaryWriter& w)
{
}

void MiddleHealItem::LoadFrom(BinaryReader& r)
{
}

void MiddleHealItem::Use()
{
	Character::player->AddHp(healValue);
	if (Character::player->GetHp() >= Character::player->GetMaxHp()) {
		Character::player->SetHp(Character::player->GetMaxHp());
	}
	AudioManager::GetInstance().PlayOneShot("Heal");
	isEffectFinished = true;  // ★Playerが削除できるように
}



/// <summary>
/// 回復(大)
/// </summary>
/// <param name="_pos"></param>
/// <param name="_name"></param>
/// <param name="_desc"></param>
/// <param name="_value"></param>
/// <param name="_effectValue"></param>
LargeHealItem::LargeHealItem(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue)
	:ItemBase(VZero, "item", "Potion_Large", _name, _desc, "Consumable", _value, _effectValue, "Res/ItemIcon/potion.png")
	, healValue(_effectValue) {
	Start();
}

void LargeHealItem::Start()
{
	isEffectFinished = false;
	AudioManager::GetInstance().Load("Res/SE/ゲージ回復3.mp3", "Heal", false);

}

void LargeHealItem::Render()
{
}

void LargeHealItem::Update()
{
	
}

void LargeHealItem::SaveTo(BinaryWriter& w)
{
}

void LargeHealItem::LoadFrom(BinaryReader& r)
{
}

void LargeHealItem::Use()
{
	Character::player->AddHp(healValue);
	if (Character::player->GetHp() >= Character::player->GetMaxHp()) {
		Character::player->SetHp(Character::player->GetMaxHp());
	}
	AudioManager::GetInstance().PlayOneShot("Heal");
	isEffectFinished = true;  // ★Playerが削除できるように
}

#pragma endregion

#pragma region バフ系
AttactPotion::AttactPotion(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue,float _time)
	:ItemBase(VZero, "item","AttactPotion", _name, _desc, "Consumable", _value, _effectValue, "Res/ItemIcon/attackPotion.png")
	, attactValue(_effectValue)
	,originAttack(0.0f)
	,isBoost(false)
	, duration(_time)
	, timer() {
	Start();
}

void AttactPotion::Start()
{
	isEffectFinished = false;
	AudioManager::GetInstance().Load("Res/SE/ゲージ回復3.mp3", "Heal", false);

}

void AttactPotion::Render()
{
}

void AttactPotion::Update()
{
	
	TimeManager* time = &TimeManager::GetInstance();

	if (!isBoost) return;

	timer -= time->deltaTime;  // 経過時間を減らす
	if (timer <= 0.0f) {
		// 効果終了
		Character::player->SetAtk(originAttack);
		isBoost = false;
		timer = 0.0f;
		isEffectFinished = true;  // ★Playerが削除できるように
		

	}
}

void AttactPotion::SaveTo(BinaryWriter& w)
{
}

void AttactPotion::LoadFrom(BinaryReader& r)
{
}

void AttactPotion::Use()
{
	AudioManager::GetInstance().PlayOneShot("Heal");
	// 既に上昇中ならタイマーをリセットする（重ねがけ防止）
	if (isBoost) {
		timer = duration;
		return;
	}
	originAttack = Character::player->GetAtk();
	Character::player->SetAtk(originAttack + attactValue);
	timer = duration;
	isEffectFinished = false;
	isBoost = true;


}

DefensePotion::DefensePotion(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue, float _time)
	:ItemBase(VZero, "item", "DefensePotion", _name, _desc, "Consumable", _value, _effectValue, "Res/ItemIcon/defensePotion.png")
	, defenseValue(_effectValue)
	, originDefense(0.0f)
	, isBoost(false)
	, duration(_time)
	, timer() {
	Start();
}

void DefensePotion::Start()
{
	isEffectFinished = false;
	AudioManager::GetInstance().Load("Res/SE/ゲージ回復3.mp3", "Heal", false);

}

void DefensePotion::Render()
{
}

void DefensePotion::Update()
{
	TimeManager* time = &TimeManager::GetInstance();

	if (!isBoost) return;

	timer -= time->deltaTime;  // 経過時間を減らす

	if (timer <= 0.0f) {
		// 効果終了
		Character::player->SetDef(originDefense);
		isBoost = false;
		timer = 0.0f;
		isEffectFinished = true;  // ★Playerが削除できるように


	}
}

void DefensePotion::SaveTo(BinaryWriter& w)
{
}

void DefensePotion::LoadFrom(BinaryReader& r)
{
}

void DefensePotion::Use()
{
	AudioManager::GetInstance().PlayOneShot("Heal");

	// 既に上昇中ならタイマーをリセットする（重ねがけ防止）
	if (isBoost) {
		timer = duration;
		return;
	}
	originDefense = Character::player->GetDef();
	Character::player->SetDef(originDefense + defenseValue);
	timer = duration;
	isEffectFinished = false;
	isBoost = true;
}
#pragma endregion

#pragma region グレネード
Grenade::Grenade(VECTOR _pos, const std::string& _name, const std::string& _desc, int _value, int _effectValue)
	:ItemBase(VZero, "item", "Grenade", _name, _desc, "Consumable", _value, _effectValue, "Res/ItemIcon/grenade.png") 
	,timer(0.0f)
	,damage(_effectValue)
	,grenadeModel(INVALID)
	,isExploded(false)
	,pEffe(){
	Start();
}

void Grenade::Start()
{
	SetTag("Grenade");
	AudioManager::GetInstance().Load("Res/Audio/SE/Item/打撃3.mp3", "explosion", false);
	grenadeModel = MV1LoadModel("Res/Model/DropObject/Grenade.mv1");
}

void Grenade::Render()
{
	if (grenadeModel == INVALID) return;

	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetTranslate(position));

	MV1SetMatrix(grenadeModel, mat);
	MV1DrawModel(grenadeModel);
}

void Grenade::Update()
{
	TimeManager* time = &TimeManager::GetInstance();
	if (isExploded) return;

	// 経過時間
	timer += time->deltaTime;   // ← 毎フレームの経過秒数
	// 3秒経過で爆発
	if (timer >= 3.0f)
	{
		Explode();
		
	}
}

void Grenade::Explode()
{
	isExploded = true;

	// 爆発エフェクトを表示
	auto* eff = EffectManager::GetInstance().Instantiate("Explosion", position);


	// 爆発音
	AudioManager::GetInstance().PlayOneShot("explosion");

	  // --- 爆発ダメージの当たり判定を一瞬生成（0.1秒） ---
	float explosionRadius = 100.0f;
	float lifeTime = 0.1f;
	Player::GetInstance()->GetPlayerAttack()->CreateHitBox(position, explosionRadius);
	
	isEffectFinished = true;  // ★Playerが削除できるように
	
}

void Grenade::SaveTo(BinaryWriter& w)
{
}

void Grenade::LoadFrom(BinaryReader& r)
{
}

void Grenade::Use()
{
	isExploded = false;
	timer = 0.0f;
	isEffectFinished = false;
	VECTOR p = Player::GetInstance()->GetPosition();

	// 少しだけ地面に落とす
	position = VGet(p.x, p.y + 2.0f, p.z);

	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetTranslate(position)); // 現在のGrenadeの位置へ
	MV1SetMatrix(grenadeModel, mat);

}
#pragma endregion

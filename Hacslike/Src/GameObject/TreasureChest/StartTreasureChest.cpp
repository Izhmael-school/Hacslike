#include "StartTreasureChest.h"
#include <algorithm>
#include"../../Manager/CollisionManager.h"
#include"../../Component/Collider/Collider.h"
#include"../Character/Player/Player.h"
#include "../../Manager/AudioManager.h"

StartTreasureChest* StartTreasureChest::pInstance = nullptr;

StartTreasureChest::StartTreasureChest(VECTOR _pos, std::string _tag)
	:GameObject(_pos, "TreasureChest")
	, TreasureChestModel(INVALID)
	, active(false) {
	// NOTE: singleton 登録しない（シーンで new したオブジェクトと競合するため）
	Start();
}

StartTreasureChest::~StartTreasureChest()
{
	if (TreasureChestModel != INVALID) {
		MV1DeleteModel(TreasureChestModel);
		TreasureChestModel = INVALID;
	}
}

void StartTreasureChest::CreateInstance()
{
	if (pInstance == nullptr) {
		pInstance = new StartTreasureChest();
	}
}

StartTreasureChest* StartTreasureChest::GetInstance()
{
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

void StartTreasureChest::DestroyInstance()
{
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

void StartTreasureChest::Start()
{
	// コライダーを作る
	SetCollider(new SphereCollider(this, VZero, 100));
	// 生成時に CollisionManager に登録（OpenTreasureChest で UnRegister する対）
	//CollisionManager::GetInstance().Register(pCollider);

	TreasureChestModel = MV1LoadModel("Res/Model/DropObject/TreasureChest.mv1");
	active = true;
}

void StartTreasureChest::Update()
{
	if (!active) return;
	// コライダー更新
	if (pCollider != nullptr)
	{
		pCollider->SetMatrix(matrix);
		pCollider->Update();
	}

	GameObject::Update();
}

void StartTreasureChest::Render()
{
	if (!active || !isVisible) return;
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	MV1SetMatrix(TreasureChestModel, matrix);
	MV1DrawModel(TreasureChestModel);

#if _DEBUG
	if (pCollider != nullptr) {
		pCollider->Render();
	}
#endif
}

void StartTreasureChest::OpenTreasureChest()
{
	// 既に無効なら何もしない
	if (!active) return;

	active = false;
	isVisible = false;

	// コライダー無効化 & CollisionManager から登録解除
	if (pCollider != nullptr) {
		pCollider->SetEnable(false);
		CollisionManager::GetInstance().UnRegister(pCollider);
	}

	// モデル解放
	if (TreasureChestModel != INVALID) {
		MV1DeleteModel(TreasureChestModel);
		TreasureChestModel = INVALID;
	}

	// 効果音
	AudioManager::GetInstance().PlayOneShot("Decision");
}
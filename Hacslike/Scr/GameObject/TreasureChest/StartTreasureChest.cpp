#include "StartTreasureChest.h"
#include <algorithm>
#include"../../Manager/CollisionManager.h"
#include"../../Component/Collider/Collider.h"
#include"../Character/Player/Player.h"
#include "../../Manager/AudioManager.h"

StartTreasureChest* StartTreasureChest::pInstance = nullptr;

StartTreasureChest::StartTreasureChest(VECTOR _pos, std::string _tag)
	:GameObject(_pos,"TreasureChest")
	, TreasureChestModel(INVALID)
	,active(false){
	Start();
}

StartTreasureChest::~StartTreasureChest()
{
	if(TreasureChestModel != INVALID){
		MV1DeleteModel(TreasureChestModel);
		TreasureChestModel = INVALID;
	}
}

void StartTreasureChest::CreateInstance()
{
	pInstance = new StartTreasureChest();
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
	SetCollider(new SphereCollider(this, VZero, 100));
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
	// active または isVisible のどちらかが false なら描画しない
	if (!active || !isVisible) return;
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	MV1SetMatrix(TreasureChestModel, matrix);
	MV1DrawModel(TreasureChestModel);

	// 当たり判定の描画（デバッグ用）
#if _DEBUG
   if (pCollider != nullptr) {
	   pCollider->Render();
   }
#endif
}

void StartTreasureChest::OpenTreasureChest()
{

}

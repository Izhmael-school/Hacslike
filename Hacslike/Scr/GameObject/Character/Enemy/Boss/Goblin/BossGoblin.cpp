#include "BossGoblin.h"
#include "../../../../../Component/Collider/Collider.h"

BossGoblin::BossGoblin() {
	Start();
}

BossGoblin::~BossGoblin() {
}

void BossGoblin::Start() {
	BossBase::Start();

	SetStatusData(1);
	SetModelHandleDup(MV1LoadModel((mPath + "boss.mv1").c_str()));
	int i = modelHandle;

	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 150, 0), 30);
	SetScale(VGet(0.1f, 0.1f, 0.1f));
	type = Goblin;

	// アニメーションの設定
	pAnimator->SetModelHandle(modelHandle);
	// ステータスの設定
	SetStatusData(1);
	// アニメーションのロード
	LoadAnimation();
	// アニメーションイベントの設定
	pAnimator->GetAnimation("dead")->SetEvent([this]() {EnemyManager::GetInstance().DeleteEnemy(this); }, pAnimator->GetTotalTime("dead"));
}

void BossGoblin::Update() {
	BossBase::Update();
}

void BossGoblin::Render() {
	BossBase::Render();
}


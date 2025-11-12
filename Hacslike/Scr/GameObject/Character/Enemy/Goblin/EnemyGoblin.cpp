#include "EnemyGoblin.h"
#include "../../../../Component/Collider/Collider.h"


EnemyGoblin::EnemyGoblin(int mHandle) {
	SetModelHandle(mHandle);
	Start();
}

EnemyGoblin::~EnemyGoblin() {}

void EnemyGoblin::Start() {
	Enemy::Start();
	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 150, 0), 30);
	SetScale(VGet(0.1f, 0.1f, 0.1f));
	type = Goblin;

	// アニメーションの設定
	pAnimator->SetModelHandle(modelHandle);
	// ステータスの設定
	SetStatusData(0);
	// アニメーションのロード
	LoadAnimation();
	// アニメーションイベントの設定
	pAnimator->GetAnimation("dead")->SetEvent([this]() { SetVisible(false); }, pAnimator->GetTotalTime("dead"));
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 2)); }, 14);

}

void EnemyGoblin::Update() {
	Enemy::Update();
	// 死んでたら更新しない
	if (isDead) return;
	// レイの更新
	Vision_Fan(GetPlayer()->GetPosition());
	// 追跡行動
	Tracking();





	if (!rayAnswer)
		pAnimator->Play("idle01");
	if (rayAnswer && !isTouch)
		pAnimator->Play("run");
	if (rayAnswer && isTouch)
		pAnimator->Play("attack01");

}

void EnemyGoblin::Render() {
	Enemy::Render();
}

void EnemyGoblin::OnTriggerEnter(Collider* _pOther) {
	Enemy::OnTriggerEnter(_pOther);
}

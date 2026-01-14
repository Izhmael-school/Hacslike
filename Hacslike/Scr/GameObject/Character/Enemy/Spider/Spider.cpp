#include "EnemySpider.h"


EnemySpider::EnemySpider(int mHandle)
	:bullet(nullptr)
{
	SetModelHandle(mHandle);
	// ステータスの設定
	SetStatusData(2);
	Start();
}

EnemySpider::~EnemySpider() {}

void EnemySpider::Start() {
	Enemy::Start();
	// 当たり判定の設定
	pCollider = new SphereCollider(this,position,100);
	SetScale(VGet(0.01f, 0.01f, 0.01f));
	type = EnemyType::Spider;

	// アニメーションの設定
	pAnimator->SetModelHandle(modelHandle);
	// アニメーションのロード
	LoadAnimation();
	// アニメーションイベントの設定
	pAnimator->GetAnimation("dead")->SetEvent([this]() { EnemyManager::GetInstance().UnuseEnemy(this); }, pAnimator->GetTotalTime("dead"));
	// 攻撃の当たり判定
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 2 / GetFPS())); }, 14);
	//pAnimator->GetAnimation("attack02")->SetEvent([this]() { attackColliderList.push_back(new SphereHitBox(this, VZero, 200, 3)); }, 12);
	// 攻撃中の移動制御
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(true); }, 8);
	pAnimator->GetAnimation("attack01")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack01"));
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(true); }, 8);
	pAnimator->GetAnimation("attack02")->SetEvent([this]() { SetAttacking(false); }, pAnimator->GetTotalTime("attack02"));
	// 攻撃の効果音
	pAnimator->GetAnimation("attack01")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SpiderAttack"); }, 14);
	pAnimator->GetAnimation("dead")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, 9);

}

void EnemySpider::Update() {
	Enemy::Update();

	if (bullet != nullptr)
		bullet->Update();
}

void EnemySpider::Render() {
	Enemy::Render();

	if (bullet != nullptr)
		bullet->Render();
}

void EnemySpider::OnTriggerEnter(Collider* _pOther) {
	if (IsDead()) return;
	Enemy::OnTriggerEnter(_pOther);
}

void EnemySpider::ShotBullet()
{
	if (isAttack()) return;

	// 当たり判定の準備
	SphereHitBox* col = new SphereHitBox(this, VZero, SpiderBullet::maxDis, 3);
	attackColliderList.push_back(col);

	// 方向の指定
	VECTOR dir = NormDir(position,player->GetPosition());

	bullet = new SpiderBullet(col->GetCollider(),dir);

}

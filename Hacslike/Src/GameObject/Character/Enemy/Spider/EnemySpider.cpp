#include "EnemySpider.h"


EnemySpider::EnemySpider(int mHandle)
{
	SetModelHandle(mHandle);
	// ステータスの設定
	SetStatusData(2);
	Start();
}

EnemySpider::~EnemySpider() {}

void EnemySpider::Start() {
	Enemy::Start();
	SetScale(VScale(VOne,0.01f));
	// 当たり判定の設定
	pCollider = new SphereCollider(this,position,100);

	// 攻撃関連変数の代入
	attack01ColliderRadius = 100;
	attack01ColliderSpawnTime = 14;

	deadAnimationTime = 9;

	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", attack01ColliderSpawnTime, colliderLifeTime, attack01ColliderRadius, 100);
	// 攻撃の効果音
	pAnimator->GetAnimation("attack01")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("SpiderAttack"); }, attack01ColliderSpawnTime);
	pAnimator->GetAnimation("dead")->SetEvent([this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, deadAnimationTime);

}

void EnemySpider::Update() {
	Enemy::Update();
}

void EnemySpider::Render() {
	Enemy::Render();
}

void EnemySpider::OnTriggerEnter(Collider* _pOther) {
	if (IsDead()) return;
	Enemy::OnTriggerEnter(_pOther);
}

void EnemySpider::ShotBullet()
{
	//// 当たり判定の準備
	//SphereHitBox* col = new SphereHitBox(this, VZero, SpiderBullet::maxDis, 3);
	//attackColliderList.push_back(col);

	//// 方向の指定
	//VECTOR dir = NormDir(position,player->GetPosition());

	//bullet = new SpiderBullet(col->GetCollider(),dir);

}

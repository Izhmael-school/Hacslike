#include "BossKetbleperz.h"

BossKetbleperz::BossKetbleperz() {
	Start();
}

BossKetbleperz::~BossKetbleperz() {}

void BossKetbleperz::Start() {

	// ステータスの設定
	SetStatusData(12);
	BossBase::Start();

	// 当たり判定の設定
	pCollider = new CapsuleCollider(this, VGet(0, 30, 0), VGet(0, 800, 0), 50);

	// 攻撃の当たり判定
	SetAnimEventForAttackCollider("attack01", 23, 2, 200, 200);
	SetAnimEventForAttackCollider("attack02", 13, 2, 200, 200);
	SetAnimEventForAttackCollider("attack03", 18, 2, 200, 200);
	// 効果音
	SetAnimEvent("attack01", [this]() {AudioManager::GetInstance().PlayOneShot("HeadBang"); }, 23);
	SetAnimEvent("attack02", [this]() {AudioManager::GetInstance().PlayOneShot("HeadBang"); }, 13);
	SetAnimEvent("attack03", [this]() {AudioManager::GetInstance().PlayOneShot("HeadBang"); }, 18);
	SetAnimEvent("dead", [this]() {AudioManager::GetInstance().PlayOneShot("Dawn"); }, 40);

}

void BossKetbleperz::Update() {
	BossBase::Update();
}

void BossKetbleperz::Render() {
	BossBase::Render();
}

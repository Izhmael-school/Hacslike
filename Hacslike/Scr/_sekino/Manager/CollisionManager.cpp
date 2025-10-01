#include "CollisionManager.h"
#include "../Component/Collider.h"
/*
 * @function	CheckHit
 * @brief		当たり判定の衝突検知
 * @param[in]	Collider* _pCol1	当たり判定1
 * @param[in]	Collider* _pCol2	当たり判定2
 * @return		bool
 */
bool CollisionManager::CheckHit(Collider* _pCol1, Collider* _pCol2) {
	// ダウンキャストを利用して当たり判定を振り分ける
	// アップキャスト : 派生は基底として扱う
	// ダウンキャスト : 基底は派生として扱う
	// ダウンキャストを行う場合は dynamic_cast<>()を利用する
	// static_cast	  : 従来のC言語のキャストとほぼ一緒で、書かれていればどんな型でもやる。できなければエラー
	// dynamic_cast	  : 従来のC言語のキャストとほぼ一緒で、書かれていればどんな型でもやる。できなければエラー

	// Box-00
	{
		// _pCol1がBoxCollider*かどうか調べる
		auto tmp1 = dynamic_cast<BoxCollider*>(_pCol1);

		if (tmp1 != nullptr) {
			// pCol1はBoxCollider*

		// Box-Box
			{
				// _pCol2がBoxCollider*かどうか調べる
				auto tmp2 = dynamic_cast<BoxCollider*>(_pCol2);
				if (tmp2 != nullptr) {
					// pCol2はBoxCollider*

					// 当たり判定の計算

					// AABB 計算 → 回転は考慮しない
					VECTOR minPoint1 = tmp1->GetMinPoint();
					VECTOR minPoint2 = tmp2->GetMinPoint();
					VECTOR maxPoint1 = tmp1->GetMaxPoint();
					VECTOR maxPoint2 = tmp2->GetMaxPoint();

					// 当たり判定の検知
					if ((minPoint1.x <= maxPoint2.x && minPoint2.x <= maxPoint1.x) &&
						(minPoint1.y <= maxPoint2.y && minPoint2.y <= maxPoint1.y) &&
						(minPoint1.z <= maxPoint2.z && minPoint2.z <= maxPoint1.z)) {

						return true;
					}
				}
			}

			// Box-Sphere
			{
				auto tmp2 = dynamic_cast<SphereCollider*>(_pCol2);
				if (tmp2 != nullptr) {
					// pCol2はSphereCollider*

					// 当たり判定の計算
				}
			}

			// Box-Capsule
			{
				auto tmp2 = dynamic_cast<CapsuleCollider*>(_pCol2);
				if (tmp2 != nullptr) {
					// pCol2はCapsuleCollider*

					// 当たり判定の計算
				}
			}
		}
	}

	// Cap-00
	{
		// _pCol1がBoxCollider*かどうか調べる
		auto tmp1 = dynamic_cast<CapsuleCollider*>(_pCol1);

		if (tmp1 != nullptr) {
			// pCol1はCapsuleCollider*
		// Cap-Box
			{
				// _pCol2がBoxCollider*かどうか調べる
				auto tmp2 = dynamic_cast<BoxCollider*>(_pCol2);
				if (tmp2 != nullptr) {
					// pCol2はBoxCollider*

					// 当たり判定の計算
				}
			}

			// Cap-Sphere
			{
				auto tmp2 = dynamic_cast<SphereCollider*>(_pCol2);
				if (tmp2 != nullptr) {
					// pCol2はSphereCollider*

					// 当たり判定の計算
				}
			}

			// Cap-Capsule
			{
				auto tmp2 = dynamic_cast<CapsuleCollider*>(_pCol2);
				if (tmp2 != nullptr) {
					// pCol2はCapsuleCollider*

					// 当たり判定の計算
					float distance = Segment_Segment_MinLength(tmp1->GetworldPoint1(),
						tmp1->GetworldPoint2(),
						tmp2->GetworldPoint1(),
						tmp2->GetworldPoint2());
					// 当たり判定の検知
					if (distance <= tmp1->GetRadius() + tmp2->GetRadius()) {
						return true;
					}

				}
			}
		}
	}

	// Sph-00
	{
		// _pCol1がBoxCollider*かどうか調べる
		auto tmp1 = dynamic_cast<SphereCollider*>(_pCol1);

		if (tmp1 != nullptr) {

			// Sph-Box
			{
				// _pCol2がBoxCollider*かどうか調べる
				auto tmp2 = dynamic_cast<BoxCollider*>(_pCol2);
				if (tmp2 != nullptr) {
					// pCol2はBoxCollider*

					// 当たり判定の計算
				}
			}

			// Sph-Sphere
			{
				auto tmp2 = dynamic_cast<SphereCollider*>(_pCol2);
				if (tmp2 != nullptr) {
					// pCol2はSphereCollider*

					// 当たり判定の計算
					VECTOR center1 = tmp1->GetCenter();
					VECTOR center2 = tmp2->GetCenter();

					// 2点間の差分を求める
					VECTOR delta = VSub(center1, center2);

					// 差分の長さ < 半径1 + 半径2
					if (VSize(delta) < tmp1->GetRadius() + tmp2->GetRadius()) {
						return true;
					}
				}
			}

			// Sph-Capsule
			{
				auto tmp2 = dynamic_cast<CapsuleCollider*>(_pCol2);
				if (tmp2 != nullptr) {
					// pCol2はCapsuleCollider*

					// 当たり判定の計算
				}
			}
		}
	}
	return false;
}

CollisionManager* CollisionManager::pInstance = nullptr;

CollisionManager::CollisionManager()
	:pColliderArray()
	, prevs()
	, currents() {}

CollisionManager::~CollisionManager() {
	for (auto pCol : pColliderArray) {
		if (pCol != nullptr) {
			delete pCol;
			pCol = nullptr;
		}
	}

	pColliderArray.clear();
	pColliderArray.shrink_to_fit();
	prevs.clear();
	prevs.shrink_to_fit();
	currents.clear();
	currents.shrink_to_fit();
}

void CollisionManager::CreateInstance() {
	pInstance = new CollisionManager();
}

CollisionManager* CollisionManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

void CollisionManager::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

void CollisionManager::Update() {

	for (int i = 0; i < currents.size(); i++) {
		for (int j = i + 1; j < currents[i].size(); j++) {

			// 現在の衝突結果を更新
			currents[i][j] = CheckHit(pColliderArray[i], pColliderArray[j]);

			// 当たった瞬間
			if (!prevs[i][j] && currents[i][j]) {

				pColliderArray[i]->GetGameObject()->OnTriggerEnter(pColliderArray[j]);
				pColliderArray[j]->GetGameObject()->OnTriggerEnter(pColliderArray[i]);

			}
			// あたっている間
			else if (currents[i][j]) {

				pColliderArray[i]->GetGameObject()->OnTriggerStay(pColliderArray[j]);
				pColliderArray[j]->GetGameObject()->OnTriggerStay(pColliderArray[i]);
			}
			// 離れた瞬間
			else if (prevs[i][j] && !currents[i][j]) {

				pColliderArray[i]->GetGameObject()->OnTriggerExit(pColliderArray[j]);
				pColliderArray[j]->GetGameObject()->OnTriggerExit(pColliderArray[i]);
			}

			// 1フレーム前の衝突結果を更新
			prevs[i][j] = currents[i][j];
		}
	}
}

void CollisionManager::Render() {
	for (auto pCol : pColliderArray) {
		if (pCol != nullptr) {
			pCol->Render();
		}
	}
}

void CollisionManager::Register(Collider* _pCol) {
	pColliderArray.push_back(_pCol);

	// 衝突結果配列にも追加
	//prevs.push_back(false);
	//currents.push_back(false);

	prevs.push_back(std::vector<bool>());
	currents.push_back(std::vector<bool>());

	index++;

	int i = 0;
	while(1){
		if (i == index)
			break;

		while (1) {
			if (prevs[i].size() == index)
				break;

			prevs[i].push_back(false);
			currents[i].push_back(false);
		}
			i++;
	}
}

void CollisionManager::UnRegister(Collider* _pCol) {
	//2種類の考え方を記載

	// イテレータ
	auto itr = std::find(pColliderArray.begin(), pColliderArray.end(), _pCol);

	// イテレータが末尾まで進んだ場合が_pColがなかった時
	if (itr == pColliderArray.end())
		return;

	// 配列
	int index = -1;

	// 配列の中に検索する要素があるかどうか
	for (int i = 0; i < pColliderArray.size(); i++) {
		if (*itr == _pCol) {
			// あったら保存
			index = i;
			break;
		}
	}

	// 指定された要素を削除する
	pColliderArray.erase(itr);
	// 衝突結果配列の要素も削除する
	prevs.erase(prevs.begin() + index);
}

#pragma once
#include <vector>
#include "../Component/Singleton.h"

/*
 * @brief 当たり判定の管理
 * @tips  シングルトン
 */
class CollisionManager : public Singleton<CollisionManager> {

private:	// メンバ変数
	std::vector<class Collider*> pColliderArray;
	std::vector<std::vector<bool>> prevs;
	std::vector<std::vector<bool>> currents;

	int index = 0;
public:		// メンバ関数
	CollisionManager();
	~CollisionManager();

	void Start();

	void Update();

	void Render();

	void Register(class Collider* _pCol);

	void CheckRegister(class Collider* _pCol);

	void UnRegister(class Collider* _pCol);
	void UnRegisterAll();

	/*
	 * @function	CheckHit
	 * @brief		当たり判定の衝突検知
	 * @param[in]	Collider* _pCol1	当たり判定1
	 * @param[in]	Collider* _pCol2	当たり判定2
	 * @return		bool
	 */
	bool CheckHit(class Collider* _pCol1, class Collider* _Col2);
};


#pragma once
#include <DxLib.h>
#include "../../GameObject/GameObject.h"

// 当たり判定の規定クラス
class Collider {
protected:
	bool isEnable;				// 当たり判定の有効無効
	GameObject* pGameObject;	// 当たり判定を付けるオブジェクト
	MATRIX matrix;				// 当たり判定を付けるオブジェクトの行列

public:// トラクタ
	// コンストラクタ
	Collider(GameObject* _pObj);
	// デストラクタ
	virtual ~Collider();
	
public:// メンバ関数
	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Render() = 0;

public:// Getter // Setter
	// isEnable
	inline bool IsEnable() const { return isEnable; }
	inline void SetEnable(bool _v) { isEnable = _v; }
	// GameObject
	inline GameObject* GetGameObject() const { return pGameObject; }
	inline void* SetGameObject(GameObject* _pObj) { pGameObject = _pObj; }
	// matrix
	inline void SetMatrix(MATRIX _mat) { matrix = _mat; }

};

/*
 * @brief 当たり判定 Box
 * @tips  BoxCollider には軸中心のものとそうでないものの2種類がある
 *		  軸中心…Axis Aligned Bounding Box 通称 AABB
 *		  じゃないの…Oriented Bounding Box 通称 OBB
 * 
 *		  今回はAABB + 当たり判定は回転しない
 *		  計算方法は２次元と同じ、軸が１つ増えただけ
 */
class BoxCollider : public Collider {
private:
	VECTOR originMinPoint;	// AABBを構成する最小の点
	VECTOR originMaxPoint;	// AABBを構成する最大の点
	
	VECTOR originVertex[8];	// 頂点データ
	VECTOR localVertex[8];	// 頂点データ
	VECTOR worldVertex[8];	// 頂点データ

public:	// トラクタ
	// コンストラクタ
	BoxCollider(GameObject* _pObj,VECTOR _min,VECTOR _max);
	// デストラクタ
	~BoxCollider() = default;

public: // オーバーライドしたメンバ関数
	// 更新
	void Update() override;
	// 描画
	void Render() override;
	
public: // Getter // Setter
	inline VECTOR GetMinPoint() const { return worldVertex[0]; }
	inline VECTOR GetMaxPoint() const { return worldVertex[7]; }

};

/*
 * @brief 当たり判定 Sphere
 * @tips
 * 
 */
class SphereCollider : public Collider {
private:
	VECTOR localCenter;
	VECTOR worldCenter;
	float radius;

public: // トラクタ
	// コンストラクタ
	SphereCollider(GameObject* _pObj,VECTOR _offset,float _radius);
	// デストラクタ
	~SphereCollider() = default;

public:// オーバーライドしたメンバ関数
	// 更新
	void Update() override;
	// 描画
	void Render() override;
public:
	inline float GetRadius() const { return radius; }
	inline VECTOR GetCenter() const { return worldCenter; }
};

/*
 * @brief 当たり判定 Sphere
 * @tips  当たり判定の計算はここだけ実装
 * 
 * Capsuleを構成するには、１つの線分と半径が必要
 */
class CapsuleCollider : public Collider {
private:
	VECTOR originPoint1, originPoint2;
	float radius;

	VECTOR offset;
	VECTOR localPoint1, localPoint2;
	VECTOR worldPoint1, worldPoint2;

public: // トラクタ
	// コンストラクタ
	CapsuleCollider(GameObject* _pObj, VECTOR _p1,VECTOR _p2, float _radius);
	// デストラクタ
	~CapsuleCollider() = default;

public:// オーバーライドしたメンバ関数
	// 更新
	void Update() override;
	// 描画
	void Render() override;

public:
	inline VECTOR GetworldPoint1() const { return worldPoint1; }
	inline VECTOR GetworldPoint2() const { return worldPoint2; }

	inline float GetRadius() const { return radius; }
};
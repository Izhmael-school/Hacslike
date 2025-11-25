#include "Collider.h"
#include "../../Definition.h"

#include "../../Manager/CollisionManager.h"

#pragma region Collider
// コンストラクタ
Collider::Collider(GameObject* _pObj) 
	:isEnable(true)
	,pGameObject(_pObj)
	,matrix(MGetIdent())
{
	pCharacter = static_cast<Character*>(pGameObject);
	CollisionManager::GetInstance().Register(this);
}

// デストラクタ
Collider::~Collider() {}
#pragma endregion

#pragma region BoxCollider
// コンストラクタ
BoxCollider::BoxCollider(GameObject* _pObj, VECTOR _min, VECTOR _max)
	:Collider(_pObj)
	,originMinPoint(_min)
	,originMaxPoint(_max)
	,originVertex()
	,localVertex()
	,worldVertex()
{
	// 無回転の状態での各頂点データの初期化
	#pragma region origin
	originVertex[0] = originMaxPoint;

	originVertex[1] = VAdd(originMinPoint, VGet(originMaxPoint.x - originMinPoint.x, 0, 0));
	originVertex[2] = VAdd(originMinPoint, VGet(0, originMaxPoint.y - originMinPoint.y, 0));
	originVertex[3] = VAdd(originMinPoint, VGet(0, 0, originMaxPoint.z - originMinPoint.z));
	originVertex[4] = VAdd(originMinPoint, VGet(0, originMaxPoint.y - originMinPoint.y, originMaxPoint.z - originMinPoint.z));
	originVertex[5] = VAdd(originMinPoint, VGet(originMaxPoint.x - originMinPoint.x, 0, originMaxPoint.z - originMinPoint.z));
	originVertex[6] = VAdd(originMinPoint, VGet(originMaxPoint.x - originMinPoint.x, originMaxPoint.y - originMinPoint.y,0));

	originVertex[7] = originMinPoint;
	#pragma endregion
}

// 更新
void BoxCollider::Update() {
	// 無効なら処理しない
	if (!isEnable)
		return;

	// y軸中心の回転行列を取得する
	MATRIX rotY = MGetRotY(Deg2Rad(pGameObject->GetPosition().y));

	// 原点からの平行移動行列を取得する
	MATRIX trans = MGetTranslate(pGameObject->GetPosition());

	// 各頂点分
	for (int i = 0; i < 8; i++) {
		// 各頂点のローカルを求める
		// VECTOR * MATRIX -> 回転後の座標
		localVertex[i] = VTransform(originVertex[i], rotY);

		// 各頂点のワールド座標を求める
		worldVertex[i] = VTransform(localVertex[i], trans);
	}
	// 見た目だけだから当たり判定の計算は開店前のAABBで行う 
}

// 描画
void BoxCollider::Render() {
	// 無効なら処理しない
	if (!isEnable)
		return;

	// 線分の描画
	DrawLine3D(worldVertex[0], worldVertex[5],blue);
	DrawLine3D(worldVertex[1], worldVertex[6],yellow);
	DrawLine3D(worldVertex[2], worldVertex[7],green);
	DrawLine3D(worldVertex[3], worldVertex[4],red);

	DrawLine3D(worldVertex[0], worldVertex[4],red);
	DrawLine3D(worldVertex[2], worldVertex[4],red);
	DrawLine3D(worldVertex[0], worldVertex[6],yellow);
	DrawLine3D(worldVertex[2], worldVertex[6],yellow);

	DrawLine3D(worldVertex[1], worldVertex[5],blue);
	DrawLine3D(worldVertex[3], worldVertex[5],blue);
	DrawLine3D(worldVertex[1], worldVertex[7],green);
	DrawLine3D(worldVertex[3], worldVertex[7],green);



}
#pragma endregion

#pragma region SphereCollider
SphereCollider::SphereCollider(GameObject* _pObj, VECTOR _offset, float _radius) 
	:Collider(_pObj)
	,localCenter(_offset)
	,worldCenter(VZero)
	,radius(_radius)
{}

void SphereCollider::Update() {
	if (!isEnable)
		return;
	worldCenter = VAdd(pGameObject->GetPosition(), localCenter);
}

void SphereCollider::Render() {
	if (!isEnable)
		return;

	DrawSphere3D(worldCenter, radius, 16, green, black, FALSE);
}
#pragma endregion

#pragma region CapsuleCollider
CapsuleCollider::CapsuleCollider(GameObject* _pObj, VECTOR _p1, VECTOR _p2, float _radius) 
	:Collider(_pObj)
	,originPoint1(_p1)
	,originPoint2(_p2)
	,radius(_radius)
	,offset(VScale(VUp,_radius))
	,localPoint1(VZero)
	,localPoint2(VZero)
	,worldPoint1(VZero)
	,worldPoint2(VZero)
{}

void CapsuleCollider::Update() {
	if (!isEnable)
		return;

	// 回転行列を取得する
	//MATRIX rotY = MGetRotY(Deg2Rad(pGameObject->GetPosition().y));
	MATRIX rotXYZ = MGetRotElem(matrix);

	// 原点からの平行移動行列を取得する
	//MATRIX trans = MGetTranslate(pGameObject->GetPosition());
	VECTOR trans = MGetTranslateElem(matrix);

	// 線分のローカル座標を求める
	localPoint1 = VTransform(originPoint1, rotXYZ);
	localPoint2 = VTransform(originPoint2, rotXYZ);

	// 線分のワールド座標を求める
	worldPoint1 = VAdd(pGameObject->GetPosition(), localPoint1);
	worldPoint2 = VAdd(pGameObject->GetPosition(), localPoint2);

	// 見た目だけだから当たり判定の計算は回転前のAABBで行う 

}

void CapsuleCollider::Render() {
	if (!isEnable)
		return;

	// カプセルを描画
	DrawCapsule3D(worldPoint1,worldPoint2,radius,16,green,green,FALSE);
}
#pragma endregion

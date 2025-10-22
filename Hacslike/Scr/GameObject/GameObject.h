#pragma once
#include "../Definition.h"

class GameObject {
protected:
	bool isVisible;		// 表示非表示
	VECTOR position;	// 座標
	VECTOR rotation;	// オイラー角の回転
	VECTOR scale;		// 拡縮

	MATRIX matrix;		// 座標回転拡縮を併せ持つ4*4行列

	std::string tag;

	class Collider* pCollider;

	int layer;
	VECTOR forward, right, up; //ローカルのベクトル

public: // トラクタ
	GameObject(VECTOR _pos = VZero, std::string _tag = "");


	virtual ~GameObject();

public:	// メンバ関数

	// 初期化処理
	virtual void Start() = 0;
	//　更新処理
	virtual void Update();
	// 描画処理
	virtual void Render() = 0;

public:	// ゲッターとセッター
#pragma region Visible
	// 表示フラグの取得
	inline bool IsVisible() const { return isVisible; }
	// 表示フラグの変更
	inline void SetVisible(bool _v) { isVisible = _v; }
#pragma endregion

#pragma region Position
	// 座標の取得
	inline VECTOR GetPosition() { 
		return position; }
	// VECTORによる座標変更
	inline virtual void SetPosition(VECTOR _v) { position = _v; }
	// floatによる座標変更
	inline virtual void SetPosition(float _x, float _y, float _z) { position = VGet(_x, _y, _z); }
#pragma endregion

#pragma region Rotation
	// 回転の取得
	inline VECTOR GetRotation() { return rotation; }
	// VECTORによる回転変更
	inline void SetRotation(VECTOR _v) { rotation = _v; }
	// floatによる回転変更
	inline void SetRotation(float _x, float _y, float _z) { rotation = VGet(_x, _y, _z); }
#pragma endregion

#pragma region Scale
	// 拡縮の取得
	inline VECTOR GetScale() { return scale; }
	// VECTORによる拡縮変更
	inline void SetScale(VECTOR _v) { scale = _v; }
	// floatによる拡縮変更
	inline void SetScale(float _x, float _y, float _z) { scale = VGet(_x, _y, _z); }
#pragma endregion

#pragma region Matrix
	// マトリクスの取得
	inline MATRIX GetMatrix() const { return matrix; }
#pragma endregion

#pragma region Collider
	// 当たり判定の取得
	inline Collider* GetCollider() const { return pCollider; }
	// 当たり判定の設定
	inline void SetCollider(Collider* _pColli) { pCollider = _pColli; }
#pragma endregion

#pragma region Tag
	// タグの取得
	inline std::string GetTag() const { return tag; }
	// タグの設定
	inline void SetTag(std::string _tag) { tag = _tag; }

	bool CompareTag(std::string _tagName);
#pragma endregion

#pragma region Layer
	// レイヤーの取得
	inline int GetLayer() const { return layer; }
	// レイヤーの設定
	inline void SetLayer(int _layer) { layer = _layer; }
#pragma endregion

public:	// 衝突検知
	// 入ったとき
	virtual void OnTriggerEnter(Collider* _pOther);
	// 入っているとき
	virtual void OnTriggerStay(Collider* _pOther);
	// 出たとき
	virtual void OnTriggerExit(Collider* _pOther);

};


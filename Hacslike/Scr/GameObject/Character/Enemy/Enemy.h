#pragma once
#include "../Character.h"
#include "../../../Manager/CollisionManager.h"
#include "../../../Manager/EffectManager.h"
#include "../../../Component/Collider/SphereHitBox.h"
#include "../../../Manager/AudioManager.h"

class Enemy : public Character {
protected:
	// Ray系
	float rayLenght;	// レイの長さ
	float rayAngle;		// レイの角度
	int rayCount;			// レイの数
	float raySpan;		// レイが更新される間隔
	float rayTime;			// レイが更新される時間

	struct Point {
		VECTOR position;
	};

	struct Fan {
		VECTOR position;		// 中心
		float rangeDegree;		// 範囲
		float length;			// 長さ
		float directionDegree;	// 方向
	};

	Point point;
	Fan fan;

	enum HostileState {
		Unaware,	// 無警戒
		Hostile		// 敵対
	};
protected:
	// 敵の種類
	EnemyType type;
	// 名前
	std::string name;
	// モデルやアニメーションのファイルパス
	std::string mPath;
	// 動きの速さ
	float moveSpeed;
	// レイに入ってるかどうか
	bool rayAnswer;
	// プレイヤーと接触しているか
	bool isTouch;
	// 死んでいるかどうか
	bool isDead;
	// 攻撃判定更新用
	std::vector<SphereHitBox*> attackColliderList;
	// 攻撃アニメーションIDリスト
	std::vector<int> attackAnimationList;
	// 次攻撃するまでの時間
	float atkTime;
	// 攻撃する間隔
	float atkSpan;

public:
	Enemy();
	~Enemy();

	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;
	
	// 再使用時の初期化
	void Setup();
	void LookTarget(VECTOR targetPos, VECTOR axis = VUp);
	// 追跡行動
	virtual void Tracking();
	// 移動
	virtual void Move(VECTOR targetPos);
	// 攻撃行動
	void Attack();
	// Jsonファイルからステータスを持ってくる
	void SetStatusData(int enemyID);
	// アニメーションのロード
	void LoadAnimation();
	// 死んだかどうか
	void IsDead() override;
	// 視界
	virtual bool Vision_Ray();
	virtual bool Vision_Circle(float r);
	virtual bool Vision_Fan(VECTOR targetPos);

	inline EnemyType GetType() const { return type; }

	// 入ったとき
	virtual void OnTriggerEnter(Collider* _pOther) override;
	// 入っているとき
	virtual void OnTriggerStay(Collider* _pOther) override;
	// 出たとき
	virtual void OnTriggerExit(Collider* _pOther) override;
};


#pragma once
#include "../Character.h"
#include "../../../Manager/CollisionManager.h"

class Enemy : public Character {
protected:
	// Ray系
	float rayLenght;	// レイの長さ
	float rayAngle;		// レイの角度
	int rayCount;			// レイの数
	float raySpan;		// レイが更新される間隔
	float rayTime;			// レイが更新される時間

	float moveSpeed;
protected:
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
public:
	EnemyType type;
	bool isBoss;

public:
	Enemy();
	~Enemy();

	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() override;
	
	virtual void IsDead();
	virtual bool Vision_Ray();
	virtual bool Vision_Circle(float r);
	virtual bool Vision_Fan(VECTOR targetPos);

	void LookTarget(VECTOR targetPos , VECTOR axis = VUp);
	virtual void Tracking();
	virtual void Move(VECTOR targetPos);

	inline EnemyType GetType() const { return type; }

	inline void SetBoss(bool _isBoss) { isBoss = _isBoss; }
	inline bool IsBoss() const { return isBoss; }
};


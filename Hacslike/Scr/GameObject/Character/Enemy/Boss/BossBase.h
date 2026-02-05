#pragma once
#include "../Enemy.h"
#include "../../../Artifact/ArtifactSelectUI.h"
#include "../../../../UI/Gauge.h"

class BossBase : public Enemy {
public:
	

	// 追加フラグ：
	// 死亡アニメーションが終了したかどうか（アニメ終了時のイベントで true にする）
	bool deadAnimEnded = false;
	// アーティファクト選択後に削除保留中かどうか
	bool pendingDelete = false;

	bool firstRayHit = false;
protected:
	BossBase(VECTOR _appearPos);
	~BossBase();

public:


public:
public:
	void Start() override;
	void Update() override;
	void Render() override;

	void DeadExecute() override;

	// 現在存在しているボスを取得
	static BossBase* GetInstance() { return instance; }

	bool WallDetectionVision_Fan(VECTOR targetPos);

private:

	Gauge<int>* hpBar;
	Gauge<float>* attackSpanBar;

protected:
	// 階段が出現する位置
	VECTOR appearPos;
	// 魔法陣が出る位置
	VECTOR circlePos;

public:
	inline void SetAppearPos(VECTOR pos) { appearPos = pos; }
	inline void SetReturnerPos(VECTOR pos) { circlePos = pos; }
private:
	static BossBase* instance;
};


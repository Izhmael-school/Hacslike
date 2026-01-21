#pragma once
#include "../Enemy.h"
#include "../../../Artifact/ArtifactSelectUI.h"
#include "../../../../UI/Gauge.h"

class BossBase : public Enemy {
public:
	ArtifactSelectUI artifactSelectUI;
	std::vector<std::shared_ptr<ArtifactBase>> artifactChioces;
	bool isSelectArtifact = false;

	// 追加フラグ：
	// 死亡アニメーションが終了したかどうか（アニメ終了時のイベントで true にする）
	bool deadAnimEnded = false;
	// アーティファクト選択後に削除保留中かどうか
	bool pendingDelete = false;
protected:
	BossBase();
	~BossBase();

protected:
	void AppearStair();

public:
public:
	void Start() override;
	void Update() override;
	void Render() override;

	void DeadExecute() override;


private:

	Gauge* hpBar;


	// 階段が出現する位置
	VECTOR appearPos;

public:
	inline void SetAppearPos(VECTOR pos) { appearPos = pos; }
};


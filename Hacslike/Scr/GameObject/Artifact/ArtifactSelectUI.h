#pragma once
#include"ArtifactBase.h"
#include <memory>

class ArtifactSelectUI
{
struct FireworkParticle {
	float x, y;
	float vx, vy;
	int color;
	float life;
	float maxLife;
	bool exploded;
};

private:
	std::vector<FireworkParticle> fireworks;
	int selectIndex = 0;
	bool isActive = false;
	int cardWidth = 260;
	int cardHeight = 360;
	int inputCooldown = 0;   // 矢印キー連打防止クールタイム
	int hoverIndex = -1;


	// --- 出現アニメーション関連 ---
	bool isAppearing = false;  // アニメーション中かどうか
	float animTimer = 0.0f;    // 経過フレーム
	float animDuration = 30.0f; // 演出の総フレーム数（30で約0.5秒）

public:
	void StartSelection();
	int UpdateSelection(const std::vector < std::shared_ptr<ArtifactBase>>& artifact);
	void Render(const std::vector < std::shared_ptr<ArtifactBase>>& artifact);
	bool IsActive() const { return isActive; }

};


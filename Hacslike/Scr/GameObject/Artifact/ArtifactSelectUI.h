#pragma once
#include"ArtifactBase.h"
#include <memory>

class ArtifactSelectUI
{
private:
	int selectIndex = 0;
	bool isActive = false;
	int cardWidth = 260;
	int cardHeight = 360;

public:
	void StartSelection();
	int UpdateSelection(const std::vector < std::shared_ptr<ArtifactBase>>& artifact);
	void Render(const std::vector < std::shared_ptr<ArtifactBase>>& artifact);
	bool IsActive() const { return isActive; }
};


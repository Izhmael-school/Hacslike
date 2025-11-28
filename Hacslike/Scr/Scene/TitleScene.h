#pragma once
#include "BaseScene.h"
class TitleScene : public BaseScene {
	int teamLogoHandle;

public:
	TitleScene();
	~TitleScene();

	void Start() override;
	void Update() override;
	void Render() override;
	void Setup() override;
	void Teardown() override;
};


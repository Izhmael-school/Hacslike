#pragma once
#include "BaseScene.h"
#include "../Manager/FontManager.h"

class TitleScene : public BaseScene {
	int teamLogoHandle;

	int titleHandle;

	int titleMenuIndex = 0;      // 0: New Game, 1: Load, 2: Exit
	bool inLoadMenu = false;
	int selectedSlot = 0;        // 0..9
	int messageFramesLeft = 0;   // for temporary messages
	char messageBuf[256] = { 0 };

	int fontHandle;
public:
	TitleScene();
	~TitleScene();

	void Start() override;
	void Update() override;
	void Render() override;
	void Setup() override;
	void Teardown() override;

	
};


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

	int logoHandle;

	int fontHandle;

	int descFontHandle;  // 追加: 説明文用フォント

	// 追加: メニュー項目の説明文
	struct MenuDescription {
		const char* title;
		const char* description[4];  // 最大4行の説明
		int lineCount;
	};
	MenuDescription menuDescriptions[4];

	// 追加: 説明文の表示アニメーション用
	float descriptionAlpha;
	int previousMenuIndex;

	// 追加: ゲーム開始モードを保持するフラグ
	enum class GameStartMode {
		None,
		NewGame,
		StrongNewGame,
		Load
	};
	GameStartMode pendingStartMode = GameStartMode::None;

public:
	TitleScene();
	~TitleScene();

	void Start() override;
	void Update() override;
	void Render() override;
	void Setup() override;
	void Teardown() override;

	// 追加: ゲーム開始モードを取得
	GameStartMode GetPendingStartMode() const { return pendingStartMode; }
	void ClearPendingStartMode() { pendingStartMode = GameStartMode::None; }
	
private:
	// 追加: 説明文の初期化
	void InitializeMenuDescriptions();
	// 追加: 説明文の描画
	void RenderMenuDescription();
};


#include "FadeManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include <thread>

void FadeManager::Update() {
	// フェード処理が完了していたら更新しない
	if (fadeState == FadeState::FadeEnd) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		return;
	}

	//	だんだんとフェード処理を行うための計算
	alpha += 255 * (int)fadeState * TimeManager::GetInstance().deltaTime / time;

	if (alpha <= 0) {
		alpha = 0;
		fadeState = FadeState::FadeEnd;
	}
	if (alpha >= 255) {
		alpha = 255;
		fadeState = FadeState::FadeEnd;
	}
}

/*
*	@function	Render
*	@breif		描画処理
*/
void FadeManager::Render() {
	SceneManager::GetInstance().Render();
	//	透明度を変化させてフェード処理とする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha);
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, black, TRUE);
	// ブレンドモードを元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, -1);
#if _DEBUG
	DrawFormatStringToHandle(100, 200, red, MainFont,"blend : %f", alpha);
	DrawFormatStringToHandle(100, 220, red, MainFont," time : %f", TimeManager::GetInstance().deltaTime);
#endif
}

void FadeManager::FadeStart(FadeState state, float _t) {
	fadeState = state;
	time = _t;
	alpha = fadeState == FadeState::FadeIn ? BlendMax : 0.0f;
}

void FadeManager::Fade(FadeState state, float _t) {
	if (state == FadeState::FadeEnd) return;
	fadeState = state;
	time = _t;
	alpha = state == FadeState::FadeIn ? BlendMax : 0;

	if (fadeState == FadeState::FadeIn) {
		SceneManager::GetInstance().Update();
		SceneManager::GetInstance().Render();
	}

	while (1) {
		TimeManager::GetInstance().Update();
		//	だんだんとフェード処理を行うための計算
		alpha += (BlendMax * TimeManager::GetInstance().deltaTime / time) * (int)fadeState;

		if (fadeState == FadeState::FadeOut && alpha >= BlendMax) {
			alpha = BlendMax;
			break;
		}

		if (fadeState == FadeState::FadeIn && alpha <= 0) {
			alpha = 0;
			break;
		}

		ClearDrawScreen();
		Render();
		ScreenFlip();

		// 処理が速すぎたら待つ
		while (1) {
			if (GetNowCount() - TimeManager::GetInstance().GetCurrent() >= 1000 / FPS)
				break;
		}
	}

	ClearDrawScreen();
	Render();
	ScreenFlip();

	DeleteGraph(screenGraph);
	screenGraph = -1;
	fadeState = FadeState::FadeEnd;
}

void FadeManager::FadeIn(float _t) {
	Fade(FadeState::FadeIn, _t);
}

void FadeManager::FadeOut(float _t) {
	Fade(FadeState::FadeOut,_t);
}

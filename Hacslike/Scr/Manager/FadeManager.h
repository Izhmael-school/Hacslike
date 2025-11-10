#pragma once
#include "../Definition.h"
#include "../Component/Singleton.h"

class FadeManager : public Singleton<FadeManager> {
private:
	const int BlendMax = 255;
	float time = 0;
	float alpha = 0;
	FadeState fadeState = FadeState::FadeEnd;
	int screenGraph = -1;
public:
	void Update();
	void Render();
	void FadeStart(FadeState state, float _t = 1.0f);

	void Fade(FadeState state,float _t = 1.0f);
	void FadeIn(float _t);
	void FadeOut(float _t);
public:
	inline FadeState GetFadeState() const { return fadeState; }
};


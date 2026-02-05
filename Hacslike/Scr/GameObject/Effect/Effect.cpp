#include "Effect.h"
#include <EffekseerForDXLib.h>

Effect::Effect(int& _resourceHandle)
	:GameObject()
	, resourceHandle(_resourceHandle)
	, playingHandle(INVALID) {

}

Effect::~Effect() {
	// オブジェクトが消えるときはエフェクトも強制停止させる
	if (playingHandle != INVALID) {
		StopEffekseer3DEffect(playingHandle);
		playingHandle = INVALID;
	}
}

void Effect::Start() {
	
}

void Effect::Update() {
	//非表示なら更新しない
	if (!isVisible)
		return;

	//エフェクトの再生
	if (playingHandle == INVALID)
		playingHandle = PlayEffekseer3DEffect(resourceHandle);

	//再生が終わったら非表示に
	if (!IsEffekseer3DEffectPlaying(playingHandle))
		isVisible = false;

	//エフェクトの座標変更
	SetPosPlayingEffekseer3DEffect(playingHandle, position.x, position.y, position.z);
}

void Effect::Stop()
{
	if (playingHandle != INVALID) {
		StopEffekseer3DEffect(playingHandle);
		playingHandle = INVALID;
	}
	isVisible = false;
}

void Effect::Render() {
	//非表示なら描画しない
	if (!isVisible)
		return;

}
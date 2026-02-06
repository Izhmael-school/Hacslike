#include "Audio.h"

Audio::Audio(int _soundHandle, bool _is3D)
	:is3Dsound(_is3D)
	, isLoop(false)
	, soundHandle(_soundHandle)
	, volume(255 * 0.7f)
	,distance(800)
{
	//Start();
}

Audio::~Audio() {}

void Audio::Start() {

	ChangeVolumeSoundMem((int)volume, soundHandle);
	SetVolumeSoundMem((int)volume, soundHandle);
	if (isLoop) {
		PlaySoundMem(soundHandle, DX_PLAYTYPE_LOOP);
	}
	else {
		PlaySoundMem(soundHandle, DX_PLAYTYPE_BACK);
	}
}

void Audio::Update() {
	if (!isVisible)
		return;
	
	// Ä¶’†‚©
	if (CheckSoundMem(soundHandle) == 0)
		isVisible = false;

	// ‰¹—Ê‚Ìİ’è
	ChangeVolumeSoundMem((int)volume, soundHandle);

	// 3d‚È‚ç
	if (is3Dsound) {
		// ‰¹Œ¹‚ÌˆÊ’u‚Ìİ’è
		Set3DPositionSoundMem(position, soundHandle);
		// ‰¹‚ª•·‚±‚¦‚é”ÍˆÍ‚Ìİ’è
		Set3DRadiusSoundMem(distance, soundHandle);
	}

}

void Audio::Render() {}

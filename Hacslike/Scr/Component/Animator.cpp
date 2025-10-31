#include "Animator.h"
#include "../Definition.h"

Animator::Animator()
	:animationModelHandle(INVALID)
	, pAnimations()
	, currentAnimation(INVALID)
	, isPlaying(false) {}

Animator::~Animator() {
	for (auto anim : pAnimations) {
		delete anim;
	}
	pAnimations.shrink_to_fit();
}

void Animator::Update() {
	if (currentAnimation == INVALID)
		return;

	// 現在のアニメーションを進める
	AnimationClip<void, void>* pCurrentAnim = GetAnimation(currentAnimation);

	if (pCurrentAnim == nullptr)
		return;

	// アニメーションを進める
	pCurrentAnim->playTime += pCurrentAnim->playSpeed;

	// イベントの処理
	if (pCurrentAnim->event != nullptr) {
		if (pCurrentAnim->playTime >= pCurrentAnim->eventTime && !pCurrentAnim->isAction) {
			pCurrentAnim->isAction = true;
			pCurrentAnim->event();
		}
	}

	// 終了時間を超えたら
	if (pCurrentAnim->playTime > pCurrentAnim->totalTime) {
		// 再生フラグを落とす
		isPlaying = false;
		pCurrentAnim->playTime = 0.0f;

		// 再生していたアニメーションがループ再生するかどうか
		if (pCurrentAnim->isLoop) {
			isPlaying = true;
		}
		else {
			Play(pCurrentAnim->transition, pAnimations[pCurrentAnim->transition]->playSpeed);
		}
	}

	// アニメーションを再生
	MV1SetAttachAnimTime(animationModelHandle, 0, pCurrentAnim->playTime);
}

void Animator::Load(std::string _filePath, std::string _name, bool _isLoop, int _transition) {
	// アニメーションの動的確保 + 読み込み
	AnimationClip<>* pAnimClip = new AnimationClip<>(MV1LoadModel(_filePath.c_str()), _name, _isLoop, _transition);
	// アニメーション群に追加
	pAnimations.push_back(pAnimClip);

}

void Animator::Play(int _index, float _speed) {
	// 現在再生中のアニメーションの場合は処理しない
	if (_index == currentAnimation)
		return;

	// 再生するアニメーション番号に書き換える
	currentAnimation = _index;

	// 再生中だったアニメーションをデタッチする
	MV1DetachAnim(animationModelHandle, 0);
	// 適応するアニメーションの再生時間の初期化
	pAnimations[_index]->playTime = 0.0f;
	// 適応するアニメーションの再生速度の初期化
	pAnimations[_index]->playSpeed = _speed;
	// 適応するアニメーションをアタッチする
	int attachIndex = MV1AttachAnim(animationModelHandle, 0, pAnimations[_index]->animationHandle, FALSE);
	// 適応したアニメーションの終了時間を初期化する
	pAnimations[_index]->totalTime = MV1GetAttachAnimTotalTime(animationModelHandle, attachIndex);
	// 再生フラグを建てる
	isPlaying = true;
	// イベントフラグの初期化
	pAnimations[_index]->isAction = false;
}

void Animator::Play(std::string _name, float _speed) {
	for (int i = 0, max = pAnimations.size(); i < max; i++) {
		if (pAnimations[i]->name != _name) continue;

		Play(i, _speed);
		break;
	}
	return;
}

AnimationClip<>* Animator::GetAnimation(std::string _name) const {
	for (int i = 0, max = pAnimations.size(); i < max; i++) {
		if (pAnimations[i]->name != _name) continue;
		return pAnimations[i];
	}
}

float Animator::GetTotalTime(std::string animName) {
	int handle = GetAnimation(animName)->animationHandle;
	int t = MV1GetAnimTotalTime(handle, 0);
	return t;
}

int Animator::GetAnimationIndex(std::string animName) {
	for (int i = 0, max = pAnimations.size(); i < max; i++) {
		if (pAnimations[i]->name != animName) continue;
		return i;
	}
}


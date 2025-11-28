#include "Animator.h"
#include "../Definition.h"

Animator::Animator()
	:animationModelHandle(INVALID)
	, pAnimations()
	, currentAnimation(INVALID)
	, isPlaying(false)
	, rootFrameIndex()
	, prevRootPos() {}

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
	AnimationClip<void>* pCurrentAnim = GetAnimation(currentAnimation);

	if (pCurrentAnim == nullptr)
		return;

	// アニメーションを進める
	pCurrentAnim->playTime += pCurrentAnim->playSpeed;

	// イベントの処理
	if (pCurrentAnim->events.size() > 0) {

		for (auto& e : pCurrentAnim->events) {
			if (pCurrentAnim->playTime < e.eventTime || e.isAction) continue;
			e.isAction = true;
			e.animEvent();
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
			for (auto& e : pCurrentAnim->events) {
				e.isAction = false;
			}
			return;
		}
	}

	// アニメーションを再生
	MV1SetAttachAnimTime(animationModelHandle, 0, pCurrentAnim->playTime);
}

void Animator::Load(std::string _filePath, std::string _name, bool _canInterrupt, bool _isLoop) {
	// アニメーションの動的確保 + 読み込み
	AnimationClip<>* pAnimClip = new AnimationClip<>(MV1LoadModel(_filePath.c_str()), _name, _canInterrupt, _isLoop);
	// 読み込めてなかったらリターン
	if (pAnimClip->animationHandle == -1) return;
	// アニメーション群に追加
	pAnimations.push_back(pAnimClip);
}

void Animator::Play(int _index, float _speed) {
	// 現在再生中のアニメーションの場合は処理しない
	if (_index == currentAnimation /*|| (currentAnimation != -1 && !pAnimations[currentAnimation]->canInterrupt)*/)
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
	for (auto& e : pAnimations[_index]->events) {
		e.isAction = false;
	}
}

int Animator::Play(std::string _name, float _speed) {
	for (int i = 0, max = pAnimations.size(); i < max; i++) {
		if (pAnimations[i]->name != _name) continue;

		Play(i, _speed);
		return 0;
	}
	return -1;
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
	return -1;
}

void Animator::SetAnimModelHandle(int handle) {
	animationModelHandle = handle;
	// モデルのルートボーンを探す（ボーン名はモデルに合わせて変更）
	rootFrameIndex = MV1SearchFrame(animationModelHandle, "Root");
	if (rootFrameIndex == -1) {
		// Root が見つからない場合、Bip01やPelvisなどに変更
		rootFrameIndex = MV1SearchFrame(animationModelHandle, "Bip01");
	}
	prevRootPos = MV1GetFramePosition(animationModelHandle, rootFrameIndex);
}

// ルートモーション差分を返す
VECTOR Animator::GetRootMotionDelta() {
	if (rootFrameIndex == -1) return VGet(0, 0, 0);

	VECTOR currentPos = MV1GetFramePosition(animationModelHandle, rootFrameIndex);
	VECTOR delta = VSub(currentPos, prevRootPos);

	// 次回用に保存
	prevRootPos = currentPos;

	return delta;
}


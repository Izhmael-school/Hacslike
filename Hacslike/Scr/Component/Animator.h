#include <DxLib.h>
#include <Vector>
#include <string>


// アニメーションクリップ構造体

struct AnimationClip {
	int animationHandle;	// アニメーションハンドル
	int currentAnimationID;	// 現在のアニメーションID
	float playSpeed;
	float totalTime;	// アニメーションの終了時間
	float playTime;		// アニメーションの再生時間
	bool isLoop;		// ループするかどうか
	int transition;		// アニメーションが終了した後にセットする番号


	/*
	 * @brief コンストラクタ
	 * @param[out] int& _animHandle
	 * @param[in]  bool _isLoop = false
	 * @param[in]  int _transition
	 */
	AnimationClip(int _animHandle, bool _isLoop = false, int _transition = 0)
		:animationHandle(_animHandle)
		, currentAnimationID(0)
		, playTime(0.0f)
		, totalTime(0.0f)
		, playSpeed(1.0f)
		, isLoop(_isLoop)
		, transition(_transition) {}

	~AnimationClip() {
		MV1DeleteModel(animationHandle);
	}

};


/*
 * @brief アニメーションを管理するクラス
 * @tips  アニメーション毎にファイルが分かれているモデル用
 *
 * 今回はモデルデータとアニメーションデータだけ
 * アニメーションさせる毎にアタッチとでタッチを繰り替えす
 *
 */
 // 1つのモデルに複数のアニメーションを持たせることはできない
 // → BlenderやMayaなどを利用すればその限りではない
class Animator {
private:
	int animationModelHandle;
	std::vector<AnimationClip*> pAnimations;
	int currentAnimation;
	bool isPlaying;
	int prevAnim;

public:
	Animator();
	~Animator();
	
public:
	void Update();

	void Load(std::string _filePath, bool _isLoop = false, int _transition = 0);

	void Play(int _index, float _speed = 0.3f);

	inline void Reset() {
		pAnimations.erase(pAnimations.begin(), pAnimations.begin() + pAnimations.size());
	}

public:
	inline void SetModelHandle(int& _v) { animationModelHandle = _v; }
	inline AnimationClip* GetAnimation(int _index) const { return pAnimations[_index]; }
	inline std::vector<AnimationClip*> GetAnimation() const { return pAnimations; }
	inline int GetCurrentAnimation() const { return currentAnimation; }
	inline bool IsPlaying() const { return isPlaying; }
	inline int GetPrevAnim() const { return prevAnim; }
	inline void ChangeSpeed(int index, float _speed) { pAnimations[index]->playSpeed = _speed; }
};
#include <DxLib.h>
#include <Vector>
#include <string>
#include <functional>

template <class T = void, class R = void>
// アニメーションクリップ構造体
struct AnimationClip {
	int animationHandle;	// アニメーションハンドル
	int currentAnimationID;	// 現在のアニメーションID
	float playSpeed;
	float totalTime;	// アニメーションの終了時間
	float playTime;		// アニメーションの再生時間
	bool isLoop;		// ループするかどうか
	int transition;		// アニメーションが終了した後にセットする番号

	std::string name;	// アニメーションの名前
	std::function<R(T)> event;	// 指定の時間になった時に発生するイベント
	float eventTime;			// イベントが発生する時間
	bool isAction;				// イベントが発生したかどうか

	

	/*
	 * @brief コンストラクタ
	 * @param[out] int& _animHandle
	 * @param[in]  bool _isLoop = false
	 * @param[in]  int _transition
	 */
	AnimationClip(int _animHandle, std::string _name, bool _isLoop = false, int _transition = 0)
		:animationHandle(_animHandle)
		, currentAnimationID(0)
		, playTime(0.0f)
		, totalTime(0.0f)
		, playSpeed(1.0f)
		, isLoop(_isLoop)
		, transition(_transition)
		, name(_name)
		, event(nullptr)
		, eventTime(0)
		, isAction(false) {}

	~AnimationClip() {
		MV1DeleteModel(animationHandle);
	}

	void SetEvent(std::function<R(T)> _event, float _eventTime) {
		event = _event;
		eventTime = _eventTime;
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
	std::vector<AnimationClip<>*> pAnimations;
	int currentAnimation;
	bool isPlaying;

	int rootFrameIndex;
	VECTOR prevRootPos;

public:
	Animator();
	~Animator();

public:
	void Update();

	void Load(std::string _filePath, std::string _name, bool _isLoop = false, int _transition = 0);

	void Play(int _index, float _speed = 0.3f);
	void Play(std::string _name, float _speed = 0.3f);

	inline void Reset() {
		pAnimations.erase(pAnimations.begin(), pAnimations.begin() + pAnimations.size());
	}

public:
	inline void SetModelHandle(int& _v) { animationModelHandle = _v; }
	inline AnimationClip<>* GetAnimation(int _index) const { return pAnimations[_index]; }
	AnimationClip<>* GetAnimation(std::string _name) const;
	inline std::vector<AnimationClip<>*> GetAnimation() const { return pAnimations; }
	inline int GetCurrentAnimation() const { return currentAnimation; }
	inline bool IsPlaying() const { return isPlaying; }
	inline void ChangeSpeed(int index, float _speed) { pAnimations[index]->playSpeed = _speed; }
	float GetTotalTime(std::string animName);
	int GetAnimationIndex(std::string animName);

	inline void SetAnimModelHandle(int handle);
	VECTOR GetRootMotionDelta();
};
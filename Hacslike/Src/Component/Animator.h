#include <DxLib.h>
#include <Vector>
#include <string>
#include <functional>

template <class T = void, class... R>

struct AnimationEvent {
	std::function<T(R...)> animEvent;	// 指定の時間になった時に発生するイベント
	float eventTime;			// イベントが発生する時間
	bool isAction;				// イベントが発生したかどうか
};

template <class T = void, class... R>
// アニメーションクリップ構造体
struct AnimationClip {
	int animationHandle;	// アニメーションハンドル
	int currentAnimationID;	// 現在のアニメーションID
	float playSpeed;
	float totalTime;	// アニメーションの終了時間
	float playTime;		// アニメーションの再生時間
	bool isLoop;		// ループするかどうか
	bool canInterrupt;	// 割り込み可能か

	std::string name;	// アニメーションの名前
	std::vector<AnimationEvent<void>> events;

	

	/*
	 * @brief コンストラクタ
	 * @param[out] int& _animHandle
	 * @param[in]  bool _isLoop = false
	 * @param[in]  int _transition
	 */
	AnimationClip(int _animHandle, std::string _name,bool _canInterrupt, bool _isLoop = false)
		:animationHandle(_animHandle)
		, currentAnimationID(0)
		, playTime(0.0f)
		, totalTime(0.0f)
		, playSpeed(1.0f)
		, canInterrupt(_canInterrupt)
		, isLoop(_isLoop)
		, name(_name)
		, events() {}

	~AnimationClip() {
		MV1DeleteModel(animationHandle);
	}

	// アニメーションイベントの作成
	void SetEvent(std::function<T(R...)> _event, float _eventTime) {
		AnimationEvent e;
		e.animEvent = _event;
		e.eventTime = _eventTime;
		e.isAction = false;
		events.push_back(e);
	}
};


/*
 * @brief アニメーションを管理するクラス
 * @tips  アニメーション毎にファイルが分かれているモデル用
 *
 * 今回はモデルデータとアニメーションデータだけ
 * アニメーションさせる毎にアタッチとデタッチを繰り返す
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

	void Load(std::string _filePath, std::string _name,bool _canInterrupt, bool _isLoop = false);

	void Play(int _index, float _speed = 0.3f);
	int Play(std::string _name, float _speed = 0.3f);

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
	float GetAnimSpeed(std::string animName);

	inline void SetAnimModelHandle(int handle);
	VECTOR GetRootMotionDelta();
};
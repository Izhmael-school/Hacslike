#pragma once
#include "../GameObject.h"

class Audio : public GameObject {
private:
	bool is3Dsound;		// 3Dサウンドかどうか
	bool isLoop;		// ループするかどうか
	int soundHandle;	// 音のハンドル
	float volume;		// 音量
	float distance;		// 聞こえる範囲

public:
	Audio(int _soundHandle,bool _is3D = false);
	~Audio();

public:	// オーバーライドしたメンバ関数
	/*
	 * @function	Start
	 * @brief		初期化処理
	 */
	void Start() override;

	/*
	 * @function	Update
	 * @brief		更新処理
	 */
	void Update() override;

	/*
	 * @function	Render
	 * @brief		描画処理
	 */
	void Render() override;

public:

public:
	inline int GetSoundHandle() const { return soundHandle; }
	inline void SetLoop(bool _fact) { isLoop = _fact; }
	inline void SetVolume(float _v) { 
		volume = _v * 255; 
		
		if (volume < 0) {
			volume = 0;
		}

		if (volume > 255) {
			volume = 255;
		}
	}

};


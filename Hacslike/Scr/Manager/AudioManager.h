#pragma once
#include <string>
#include <list>
#include <map>
#include <DxLib.h>

class AudioManager {
#pragma region シングルトンのデータ構造
private:	// 静的メンバ変数
	static AudioManager* pInstance;	// 自身のインスタンスのアドレスを格納


private:	// コンストラクタとデストラクタ
	/*
	 * @brief	コンストラクタ
	 * @tip		外部で生成されないようにアクセス指定子をprivateにする
	 */
	AudioManager();

	/*
	 * @brief	デストラクタ
	 */
	~AudioManager();

public:	//コピーと譲渡禁止
	AudioManager(const AudioManager&) = delete;
	AudioManager& operator = (const AudioManager&) = delete;
	AudioManager(AudioManager&&) = delete;
	AudioManager& operator = (AudioManager&&) = delete;

private:	// 静的メンバ関数
	/*
	 * @function	CreateInstance
	 * @brief		自信のインスタンスを生成する
	 */
	static void CreateInstance();

public:	// 静的メンバ関数
	/*
	 * @function	GetInstance
	 * @brief		自信のインスタンスを取得する唯一の手段
	 * @return		InputManager*	自身のインスタンスのアドレス
	 */
	static AudioManager* GetInstance();

	/*
	 * @function	DestroyInstance
	 * @brief		自信のインスタンスを破棄する唯一の手段
	 */
	static void DestroyInstance();
#pragma endregion
private:
	std::map<std::string, int> audioResourceMap;	// リソースの連想配列
	std::list<class Audio*> pAudioList;		// 音の一元管理

public:
	/*
	 * @function Load
	 *  @brief		音の読み込み
	 *  @param[in]	std::string _filepath
	 *  @param[in]	std::string _name
	 *  @param[in]	bool _is3D
	 */
	void Load(std::string _filePath, std::string _name, bool _is3D);

	void ChangeVolume(float _volume,std::string _name = "");

	void PlayOneShot(std::string _name);

	void PlayClipAtPoint(std::string _name, VECTOR _pos, float _volume = 1.0f, bool _isLoop = false);

	void Stop(std::string _name);

	void Update();

};


#pragma once
#include <string>
#include <list>
#include <map>
#include <DxLib.h>
#include "../Component/Singleton.h"

class AudioManager :public Singleton<AudioManager>{
private:
	std::map<std::string, int> audioResourceMap;	// リソースの連想配列
	std::list<class Audio*> pAudioList;		// 音の一元管理
public:
	AudioManager();
	~AudioManager();

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

	void PlayBGM(std::string _name);

	void PlayOneShot(std::string _name);

	void PlayClipAtPoint(std::string _name, VECTOR _pos, float _volume = 1.0f, bool _isLoop = false);

	void Stop(std::string _name);

	void Update();

};


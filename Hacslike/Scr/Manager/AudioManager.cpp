#include "AudioManager.h"
#include "../GameObject/Audio/Audio.h"

AudioManager::AudioManager() 
	:audioResourceMap()
	,pAudioList()
{}

AudioManager::~AudioManager() {
	for (auto itr : audioResourceMap) {
		DeleteSoundMem(itr.second);
	}

	for (auto pA : pAudioList) {
		if (pA != nullptr) {
			delete pA;
			pA = nullptr;
		}
	}

	pAudioList.clear();
}

void AudioManager::Load(std::string _filePath, std::string _name, bool _is3D) {
	SetCreate3DSoundFlag(_is3D);

	int res = LoadSoundMem(_filePath.c_str());

	auto itr = audioResourceMap.find(_filePath.c_str());

	if (itr == audioResourceMap.end()) {
		// 登録
		audioResourceMap.emplace(_name.c_str(), res);
	}
}

void AudioManager::ChangeVolume(float _volume, std::string _name) {
	if (_name == "all" || _name == "All") {
		for (auto pA : pAudioList) {
			pA->SetVolume(_volume);
		}
	}
	else {
		for (auto pA : pAudioList) {
			if (pA->GetTag() == _name) {
				pA->SetVolume(_volume);
			}
		}
	}
}

void AudioManager::PlayOneShot(std::string _name) {
	int sound = INVALID;
	if (GetSoundTotalTime(audioResourceMap[_name]) >= 1000 * 60 * 3) {
		sound = audioResourceMap[_name];
	}
	else {
		sound = DuplicateSoundMem(audioResourceMap[_name]);
	}

	Audio* pAudioObj = new Audio(sound);
	pAudioObj->SetTag(_name);
	pAudioObj->Start();
	pAudioList.push_back(pAudioObj);
}

void AudioManager::PlayClipAtPoint(std::string _name, VECTOR _pos, float _volume, bool _isLoop) {
	Audio* pAudioObj = new Audio(audioResourceMap[_name]);
	pAudioObj->SetVolume(_volume);
	pAudioObj->SetTag(_name);
	pAudioObj->SetPosition(_pos);
	pAudioObj->SetLoop(_isLoop);
	pAudioObj->Start();
	pAudioList.push_back(pAudioObj);
}

void AudioManager::Stop(std::string _name) {
	if (_name == "all" || _name == "All") {
		for (auto pA : pAudioList) {
			StopSoundMem(pA->GetSoundHandle());
		}
	}
	else {
		for (auto pA : pAudioList) {
			if (pA->GetTag() == _name) {
				StopSoundMem(pA->GetSoundHandle());
			}
		}
	}
}

void AudioManager::Update() {
	for (auto pA : pAudioList) {
		if (pA == nullptr || !pA->IsVisible())
			continue;

		pA->Update();
	}

	// STLの要素を削除
	// std::erase_if( コンテナ, ラムダ)	C++20～ ここじゃ使えん
	// isVisible = false のものを消す
	// プロパティのC/C++の全般から変更できる
	std::erase_if(pAudioList, [](Audio* _pE) {return !_pE->IsVisible(); });
}
